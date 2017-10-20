#include "omicron/runtime/game/GameBinding.hpp"

#include <arcanecore/config/Variant.hpp>
#include <arcanecore/config/visitors/Shorthand.hpp>
#include <arcanecore/io/dl/DLOperations.hpp>

#include <omicron/api/GameInterface.hpp>
#include <omicron/api/config/ConfigInline.hpp>

#include "omicron/runtime/RuntimeGlobals.hpp"


namespace omi
{
namespace runtime
{
namespace game
{

//------------------------------------------------------------------------------
//                                 IMPLEMENTATION
//------------------------------------------------------------------------------

class GameBinding::GameBindingImpl
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
private:

    //-------------------P R I V A T E    A T T R I B U T E S-------------------

    // the handle to the dynamic library to bind the game implementation from
    arc::io::dl::Handle m_lib_handle;

    // the ArcaneCore config Variant for game binding configuration.
    arc::config::VariantPtr m_config;

    // the name of the game
    arc::str::UTF8String m_game_name;
    // the version of the game
    arc::str::UTF8String m_game_version;

    omi::GameStartupRoutineFunc* m_startup_routine_func;
    omi::GameStartupRoutineFunc* m_firstframe_routine_func;
    omi::GameShutdownRoutineFunc* m_shutdown_routine_func;
    omi::GameEntityTypeCountFunc* m_entity_count_func;
    omi::GameEntityTypeNamesFunc* m_entity_name_func;


public:

    //--------------------------C O N S T R U C T O R---------------------------

    GameBindingImpl()
        : m_lib_handle             (nullptr)
        , m_startup_routine_func   (nullptr)
        , m_firstframe_routine_func(nullptr)
        , m_shutdown_routine_func  (nullptr)
        , m_entity_count_func      (nullptr)
        , m_entity_name_func       (nullptr)
    {
    }

    //---------------------------D E S T R U C T O R----------------------------

    ~GameBindingImpl()
    {
    }

    //-------------P U B L I C    M E M B E R    F U N C T I O N S--------------

    bool startup_routine()
    {
        global::logger->debug << "GameBinding startup." << std::endl;

        // build the path to the base subsystem document
        arc::io::sys::Path config_path(omi::runtime::global::config_root_dir);
        config_path << "game" << "game.json";

        // build in-memory data
        static const arc::str::UTF8String config_compiled(
            OMICRON_CONFIG_INLINE_RUNTIME_GAME
        );

        // construct the variant
        m_config.reset(new arc::config::Variant(
            config_path,
            &config_compiled
        ));
        // use unix variant?
        #ifdef ARC_OS_UNIX
            m_config->set_variant("linux");
        #endif

        // get the path to the game library
        arc::io::sys::Path game_path = *m_config->get("path", AC_PATHV);
        if(game_path.is_empty())
        {
            global::logger->error
                << "No path to library provided." << std::endl;
            return false;
        }

        global::logger->debug
            << "Binding game from \"" << game_path << "\"" << std::endl;

        // open the game library
        try
        {
            m_lib_handle = arc::io::dl::open_library(
                game_path,
                arc::io::dl::kOpenLazy     |
                arc::io::dl::kOpenLocal    |
                arc::io::dl::kOpenDeepBind
            );
        }
        catch(const std::exception& exc)
        {
            global::logger->error
                << "Failed to open dynamic library for game implementation "
                << "with error \"" << exc.what() << "\"" << std::endl;
            return false;
        }

        // get the name and version function
        omi::GameGetNameFunc* get_name_func = nullptr;
        try
        {
            get_name_func =
                arc::io::dl::bind_symbol<omi::GameGetNameFunc>(
                    m_lib_handle,
                    "OMI_GAME_get_name"
                );
        }
        catch(const std::exception& exc)
        {
            global::logger->error
                << "Failed to bind game with error: " << exc.what()
                << std::endl;
            return false;
        }
        omi::GameGetVersionFunc* get_version_func = nullptr;
        try
        {
            get_version_func =
                arc::io::dl::bind_symbol<omi::GameGetVersionFunc>(
                    m_lib_handle,
                    "OMI_GAME_get_version"
                );
        }
        catch(const std::exception& exc)
        {
            global::logger->error
                << "Failed to bind game with error: " << exc.what()
                << std::endl;
            return false;
        }

        // get the name and version
        m_game_name = get_name_func();
        m_game_version = get_version_func();
        global::logger->debug
            << "Binding game: " << m_game_name << "-" << m_game_version
            << std::endl;

        // attempt to get the startup routine function
        try
        {
            m_startup_routine_func =
                arc::io::dl::bind_symbol<omi::GameStartupRoutineFunc>(
                    m_lib_handle,
                    "OMI_GAME_startup_routine"
                );
        }
        catch(...) {}
        if(m_startup_routine_func == nullptr)
        {
            global::logger->debug
                << m_game_name << " has no startup_routine defined."
                << std::endl;
        }

        // attempt to get the firstframe routine function
        try
        {
            m_firstframe_routine_func =
                arc::io::dl::bind_symbol<omi::GameFirstFrameRoutineFunc>(
                    m_lib_handle,
                    "OMI_GAME_firstframe_routine"
                );
        }
        catch(...) {}
        if(m_firstframe_routine_func == nullptr)
        {
            global::logger->debug
                << m_game_name << " has no firstframe_routine defined."
                << std::endl;
        }

        // attempt to get the startup routine function
        try
        {
            m_shutdown_routine_func =
                arc::io::dl::bind_symbol<omi::GameShutdownRoutineFunc>(
                    m_lib_handle,
                    "OMI_GAME_shutdown_routine"
                );
        }
        catch(...) {}
        if(m_shutdown_routine_func == nullptr)
        {
            global::logger->debug
                << m_game_name << " has no shutdown_routine defined."
                << std::endl;
        }

        // get the entity count and names function
        try
        {
            m_entity_count_func =
                arc::io::dl::bind_symbol<omi::GameEntityTypeCountFunc>(
                    m_lib_handle,
                    "OMI_GAME_entity_type_count"
                );
        }
        catch(const std::exception& exc)
        {
            global::logger->error
                << "Failed to bind game with error: " << exc.what()
                << std::endl;
            return false;
        }
        try
        {
            m_entity_name_func =
                arc::io::dl::bind_symbol<omi::GameEntityTypeNamesFunc>(
                    m_lib_handle,
                    "OMI_GAME_entity_type_names"
                );
        }
        catch(const std::exception& exc)
        {
            global::logger->error
                << "Failed to bind game with error: " << exc.what()
                << std::endl;
            return false;
        }

        // get the number of entities registered
        uint64_t entity_count = m_entity_count_func();
        global::logger->debug
            << entity_count << " entity types registered from " << m_game_name
            << std::endl;

        // get the names of the entities registered
        const char** entity_names = m_entity_name_func();

        // bind their functions into the SceneState
        for(uint64_t i = 0; i < entity_count; ++i)
        {
            arc::str::UTF8String entity_name(entity_names[i]);

            // get the factory function
            omi::GameEntityFactory* factory_func = nullptr;
            try
            {
                factory_func = arc::io::dl::bind_symbol<omi::GameEntityFactory>(
                    m_lib_handle,
                    entity_name + "_factory"
                );
            }
            catch(const std::exception& exc)
            {
                global::logger->error
                    << "Failed to bind factory function for entity \""
                    << entity_name << "\" with error: " << exc.what()
                    << std::endl;
                return false;
            }

            // get the destroy function
            omi::GameEntityDestroy* destroy_func = nullptr;
            try
            {
                destroy_func = arc::io::dl::bind_symbol<omi::GameEntityDestroy>(
                    m_lib_handle,
                    entity_name + "_destroy"
                );
            }
            catch(const std::exception& exc)
            {
                global::logger->error
                    << "Failed to bind destroy function for entity \""
                    << entity_name << "\" with error: " << exc.what()
                    << std::endl;
                return false;
            }

            // pass to the scene state
            // TODO:
        }

        return true;
    }

    bool shutdown_routine()
    {
        global::logger->debug << "GameBinding shutdown." << std::endl;

        // close the library
        if(m_lib_handle != nullptr)
        {
            arc::io::dl::close_library(m_lib_handle);
            m_lib_handle = nullptr;
        }

        return true;
    }

    const arc::str::UTF8String& get_game_name() const
    {
        return m_game_name;
    }

    const arc::str::UTF8String& get_game_version() const
    {
        return m_game_version;
    }

    bool game_startup_routine()
    {
        if(m_startup_routine_func == nullptr)
        {
            return true;
        }
        try
        {
            // TODO: AVOID THROWING ACROSS DL BOUNDRY
            return m_startup_routine_func();
        }
        catch(const std::exception& exc)
        {
            global::logger->error
                << m_game_name << " startup_routine encountered exception: "
                << exc.what() << std::endl;
        }
        return false;
    }

    bool game_firstframe_routine()
    {
        if(m_firstframe_routine_func == nullptr)
        {
            return true;
        }
        try
        {
            // TODO: AVOID THROWING ACROSS DL BOUNDRY
            return m_firstframe_routine_func();
        }
        catch(const std::exception& exc)
        {
            global::logger->error
                << m_game_name << " firstframe_routine encountered exception: "
                << exc.what() << std::endl;
        }
        return false;
    }

    bool game_shutdown_routine()
    {
        if(m_shutdown_routine_func == nullptr)
        {
            return true;
        }
        try
        {
            // TODO: AVOID THROWING ACROSS DL BOUNDRY
            return m_shutdown_routine_func();
        }
        catch(const std::exception& exc)
        {
            global::logger->error
                << m_game_name << " shutdown_routine encountered exception: "
                << exc.what() << std::endl;
        }
        return false;
    }
};

//------------------------------------------------------------------------------
//                            PUBLIC STATIC FUNCTIONS
//------------------------------------------------------------------------------

GameBinding* GameBinding::instance()
{
    static GameBinding inst;
    return &inst;
}
//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

bool GameBinding::startup_routine()
{
    return m_impl->startup_routine();
}

bool GameBinding::shutdown_routine()
{
    return m_impl->shutdown_routine();
}

const arc::str::UTF8String& GameBinding::get_game_name() const
{
    return m_impl->get_game_name();
}

const arc::str::UTF8String& GameBinding::get_game_version() const
{
    return m_impl->get_game_version();
}

bool GameBinding::game_startup_routine()
{
    return m_impl->game_startup_routine();
}

bool GameBinding::game_firstframe_routine()
{
    return m_impl->game_firstframe_routine();
}

bool GameBinding::game_shutdown_routine()
{
    return m_impl->game_shutdown_routine();
}

//------------------------------------------------------------------------------
//                              PRIVATE CONSTRUCTOR
//------------------------------------------------------------------------------

GameBinding::GameBinding()
    : m_impl(new GameBindingImpl())
{
}

//------------------------------------------------------------------------------
//                               PRIVATE DESTRUCTOR
//------------------------------------------------------------------------------

GameBinding::~GameBinding()
{
    delete m_impl;
}

} // namespace game
} // namespace runtime
} // namespace omi
