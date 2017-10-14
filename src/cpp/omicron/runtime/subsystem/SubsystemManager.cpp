#include "omicron/runtime/subsystem/SubsystemManager.hpp"

#include <unordered_map>
#include <unordered_set>

#include <arcanecore/io/sys/FileSystemOperations.hpp>
#include <arcanecore/config/Variant.hpp>
#include <arcanecore/config/visitors/Shorthand.hpp>

#include "omicron/api/common/Attributes.hpp"
#include <omicron/api/config/ConfigInline.hpp>
#include "omicron/api/report/stats/StatsDatabase.hpp"

#include "omicron/runtime/RuntimeGlobals.hpp"
#include "omicron/runtime/subsystem/ContextSSDL.hpp"


namespace omi
{
namespace runtime
{
namespace ss
{

//------------------------------------------------------------------------------
//                                 IMPLEMENTATION
//------------------------------------------------------------------------------

class SubsystemManager::SubsystemManagerImpl
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
private:

    //-------------------P R I V A T E    A T T R I B U T E S-------------------

    // whether the subsystem manager has been initialised yet or not
    bool m_initialised;

    // the ArcaneCore config Variant for general subsystem configuration.
    arc::config::VariantPtr m_config;

    // the extension that subsystems should have
    arc::str::UTF8String m_extension;

    // the names of the subsystems to use
    arc::str::UTF8String m_context_name;
    // TODO: MORE SUBSYSTEMS

    // mapping from subsystem names to the paths they should be loaded from
    std::unordered_map<arc::str::UTF8String, arc::io::sys::Path> m_paths;

    // the context subsystem DL manager
    ContextSSDL m_context_dl;

public:

    //--------------------------C O N S T R U C T O R---------------------------

    SubsystemManagerImpl()
        : m_initialised(false)
    {
    }

    //---------------------------D E S T R U C T O R----------------------------

    ~SubsystemManagerImpl()
    {
    }

    //-------------P U B L I C    M E M B E R    F U N C T I O N S--------------

    bool startup_routine()
    {
        // bail out if we're already initialised
        if(m_initialised)
        {
            return true;
        }

        global::logger->debug << "SubsystemManager startup." << std::endl;

        // build the path to the base subsystem document
        arc::io::sys::Path config_path(omi::runtime::global::config_root_dir);
        config_path << "subsystems" << "subsystems.json";

        // build in-memory data
        static const arc::str::UTF8String config_compiled(
            OMICRON_CONFIG_INLINE_RUNTIME_SUBSYSTEMS
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

        // get the subsystem searchpaths
        std::vector<arc::io::sys::Path> search_path = *m_config->get(
            "search_paths",
            AC_PATHVECV
        );
        if(search_path.empty())
        {
            global::logger->error
                << "No subsystem search paths provided." << std::endl;
        }

        // get the subsystem extension
        m_extension =  "." + *m_config->get(
            "extension",
            AC_U8STRV
        );

        // get the names of the subsystems to use
        m_context_name = *m_config->get(
            "roles.context",
            AC_U8STRV
        );
        // TODO: more subsystems

        collect_paths(search_path);

        bool success = true;
        // bind the subsystems
        if(!bind_context())
        {
            success = false;
        }
        // TODO: more subsystems

        m_initialised = true;
        return success;
    }

    bool shutdown_routine()
    {
        global::logger->debug << "SubsystemManager shutdown." << std::endl;

        m_context_dl.release();

        m_initialised = false;
        return true;
    }

private:

    //------------P R I V A T E    M E M B E R    F U N C T I O N S-------------

    // collects the path to be used for each subsystem
    void collect_paths(const std::vector<arc::io::sys::Path>& search_path)
    {
        // TODO: more subsystems
        std::unordered_set<arc::str::UTF8String> names{m_context_name};

        // iterate over each path
        for(const arc::io::sys::Path& path : search_path)
        {
            global::logger->info
                << "Searching path for potential subsystem libraries: \""
                << path << "\"" << std::endl;

            // handle if the search path is a file
            if(arc::io::sys::is_file(path))
            {
                add_if_potential_path(names, path);
            }
            // handle if the search path is a directory
            else if(arc::io::sys::is_directory(path))
            {
                // evaluate the files in the directory
                for(const arc::io::sys::Path& sub_path :
                    arc::io::sys::list(path))
                {
                    if(arc::io::sys::is_file(sub_path))
                    {
                        add_if_potential_path(names, sub_path);
                    }
                }
            }
            else
            {
                global::logger->warning
                    << "Invalid path used in search path: \"" << path << "\""
                    << std::endl;
            }
        }
    }

    // Checks if the given path is potential path that can be used to load a
    // subsystem from and if so, stores it in the path mapping
    void add_if_potential_path(
            const std::unordered_set<arc::str::UTF8String>& names,
            const arc::io::sys::Path& path)
    {
        // has at least one part and ends with the extension
        if(path.is_empty() || !path.get_back().ends_with(m_extension))
        {
            return;
        }

        // get the name of the potential subsystem library
        arc::str::UTF8String lib_name = path.get_back().substring(
            0,
            path.get_back().get_length() - m_extension.get_length()
        );

        // does the name match any of the expected names
        auto f_name = names.find(lib_name);
        if(f_name != names.end() && m_paths.find(lib_name) == m_paths.end())
        {
            auto f_lib = m_paths.find(lib_name);
            if(m_paths.find(lib_name) != m_paths.end())
            {
                global::logger->warning
                    << "Potential subsystem library at: \"" << path << "\" "
                    << "masked by a file with the same name defined earlier in "
                    << "the search path: \"" << f_lib->second << "\""
                    << std::endl;
                return;
            }

            global::logger->debug
                << "Identified potential subsystem library at \"" << path
                << "\"" << std::endl;
            // store
            m_paths.insert(std::make_pair(lib_name, path));
        }
    }

    // loads and bind the context subsystem
    bool bind_context()
    {
        // check the required subsystem path has been found
        auto f_path = m_paths.find(m_context_name);
        if(f_path == m_paths.end())
        {
            global::logger->error
                << "Failed to load and bind context subsystem as no library "
                << "with the name \"" << m_context_name << "\" was found "
                << "in the search paths." << std::endl;
            return false;
        }

        // record stats
        #ifndef OMI_API_MODE_PRODUCTION
            omi::report::StatsDatabase::instance()->define_entry(
                "Subsystem.Context.Name",
                omi::StringAttribute(m_context_name, false),
                "The name of the implementation being used for the context "
                "subsystem."
            );
            omi::report::StatsDatabase::instance()->define_entry(
                "Subsystem.Context.Path",
                omi::PathAttribute(f_path->second, false),
                "The path to the implementation being used for the context "
                "subsystem."
            );
        #endif

        return m_context_dl.bind(f_path->second);
    }
};

//------------------------------------------------------------------------------
//                            PUBLIC STATIC FUNCTIONS
//------------------------------------------------------------------------------

SubsystemManager* SubsystemManager::instance()
{
    static SubsystemManager inst;
    return &inst;
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

bool SubsystemManager::startup_routine()
{
    return m_impl->startup_routine();
}

bool SubsystemManager::shutdown_routine()
{
    return m_impl->shutdown_routine();
}

//------------------------------------------------------------------------------
//                              PRIVATE CONSTRUCTOR
//------------------------------------------------------------------------------

SubsystemManager::SubsystemManager()
    : m_impl(new SubsystemManagerImpl())
{
}

//------------------------------------------------------------------------------
//                               PRIVATE DESTRUCTOR
//------------------------------------------------------------------------------

SubsystemManager::~SubsystemManager()
{
    delete m_impl;
}

} // namespace ss
} // namespace runtime
} // namespace omi
