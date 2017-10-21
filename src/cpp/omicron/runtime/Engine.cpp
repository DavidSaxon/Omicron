#include "omicron/runtime/Engine.hpp"

#include <omicron/api/context/ContextSubsystem.hpp>
#include <omicron/api/scene/SceneState.hpp>

#include "omicron/runtime/RuntimeGlobals.hpp"
#include "omicron/runtime/boot/BootRoutines.hpp"


namespace omi
{
namespace runtime
{

//------------------------------------------------------------------------------
//                                 IMPLEMENTATION
//------------------------------------------------------------------------------

class Engine::EngineImpl
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
public:

    //--------------------------C O N S T R U C T O R---------------------------

    EngineImpl()
    {
    }

    //---------------------------D E S T R U C T O R----------------------------

    ~EngineImpl()
    {
    }

    //-------------P U B L I C    M E M B E R    F U N C T I O N S--------------

    int execute()
    {
        if(!omi::runtime::boot::startup_routine())
        {
            omi::runtime::boot::get_critical_stream()
                << "Engine startup failed. Aborting" << std::endl;
            return -1;
        }

        // start the main loop
        global::logger->info << "Starting main loop" << std::endl;
        omi::context::ContextSubsystem::instance()->main_loop(&cycle_static);

        if(!omi::runtime::boot::shutdown_routine())
        {
            omi::runtime::boot::get_critical_stream()
                << "Engine shutdown did not complete successfully" << std::endl;
            return -1;
        }

        return 0;
    }

private:

    //------------P R I V A T E    S T A T I C    F U N C T I O N S-------------

    // callback function to execute a single cycle of the Omicron engine
    static bool cycle_static()
    {
        return Engine::instance()->m_impl->cycle();
    }

    //------------P R I V A T E    M E M B E R    F U N C T I O N S-------------

    // performs a single cycle of the Omicron engine
    bool cycle()
    {
        // first frame setup
        if(!omi::runtime::boot::first_frame_routine())
        {
            return false;
        }

        // TODO: don't update more than 60fps (config based)

        // update the scene state
        omi::scene::SceneState::instance().update();

        // TODO:
        // render the frame
        // m_renderer->render();

        // TODO: delay (if frame cap)

        return true;
    }
};

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//                            PUBLIC STATIC FUNCTIONS
//------------------------------------------------------------------------------

Engine* Engine::instance()
{
    static Engine inst;
    return &inst;
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

int Engine::execute()
{
    return m_impl->execute();
}

//------------------------------------------------------------------------------
//                              PRIVATE CONSTRUCTOR
//------------------------------------------------------------------------------

Engine::Engine()
    : m_impl(new EngineImpl())
{
}

//------------------------------------------------------------------------------
//                               PRIVATE DESTRUCTOR
//------------------------------------------------------------------------------

Engine::~Engine()
{
    delete m_impl;
}

} // namespace runtime
} // namespace omi
