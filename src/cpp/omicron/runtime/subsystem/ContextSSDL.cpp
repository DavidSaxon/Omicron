#include "omicron/runtime/subsystem/ContextSSDL.hpp"

#include <arcanecore/io/dl/DLOperations.hpp>

#include <omicron/api/API.hpp>
#include <omicron/api/context/ContextSubsystem.hpp>
#include <omicron/api/context/Surface.hpp>

#include "omicron/runtime/RuntimeGlobals.hpp"


namespace omi
{
namespace runtime
{
namespace ss
{

//------------------------------------------------------------------------------
//                                 IMPLEMENTATION
//------------------------------------------------------------------------------

class ContextSSDL::ContextSSDLImpl
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
private:

    //-------------------P R I V A T E    A T T R I B U T E S-------------------

    // the handle to the dynamic library to bind the subsystem from
    arc::io::dl::Handle m_lib_handle;

    // the version of the context subsystem
    arc::str::UTF8String m_version;

public:

    //--------------------------C O N S T R U C T O R---------------------------

    ContextSSDLImpl()
        : m_lib_handle(nullptr)
    {
    }

    //---------------------------D E S T R U C T O R----------------------------

    ~ContextSSDLImpl()
    {
    }

    //-------------P U B L I C    M E M B E R    F U N C T I O N S--------------

    bool bind(const arc::io::sys::Path& path)
    {
        global::logger->debug << "Binding context subsystem." << std::endl;

        // open the subsystem dl
        try
        {
            m_lib_handle = arc::io::dl::open_library(
                path,
                arc::io::dl::kOpenLazy     |
                arc::io::dl::kOpenLocal    |
                arc::io::dl::kOpenDeepBind
            );
        }
        catch(const std::exception& exc)
        {
            global::logger->error
                << "Failed to open dynamic library for context subsystem with "
                << "error \"" << exc.what() << "\"" << std::endl;
            return false;
        }

        // get the version function
        omi::SubsytemGetVersionFunc* get_version_func = nullptr;
        try
        {
            get_version_func =
                arc::io::dl::bind_symbol<omi::SubsytemGetVersionFunc>(
                    m_lib_handle,
                    "OMI_CONTEXT_get_version"
                );
        }
        catch(const std::exception& exc)
        {
            global::logger->error
                << "Failed to bind context with error: " << exc.what()
                << std::endl;
        }
        // get the version
        m_version = arc::str::UTF8String(get_version_func());

        // get the subsystem functions
        omi::SubsystemObjectFactoryFunc* subsystem_factory_func = nullptr;
        try
        {
            subsystem_factory_func =
                arc::io::dl::bind_symbol<omi::SubsystemObjectFactoryFunc>(
                    m_lib_handle,
                    "OMI_CONTEXT_subsystem_factory"
                );
        }
        catch(const std::exception& exc)
        {
            global::logger->error
                << "Failed to bind context with error: " << exc.what()
                << std::endl;
        }
        omi::SubsystemObjectDestroyFunc* subsystem_destroy_func = nullptr;
        try
        {
            subsystem_destroy_func =
                arc::io::dl::bind_symbol<omi::SubsystemObjectDestroyFunc>(
                    m_lib_handle,
                    "OMI_CONTEXT_subsystem_destroy"
                );
        }
        catch(const std::exception& exc)
        {
            global::logger->error
                << "Failed to bind context with error: " << exc.what()
                << std::endl;
        }

        // get the surface functions
        omi::SubsystemObjectFactoryFunc* surface_factory_func = nullptr;
        try
        {
            surface_factory_func =
                arc::io::dl::bind_symbol<omi::SubsystemObjectFactoryFunc>(
                    m_lib_handle,
                    "OMI_CONTEXT_surface_factory"
                );
        }
        catch(const std::exception& exc)
        {
            global::logger->error
                << "Failed to bind context with error: " << exc.what()
                << std::endl;
        }
        omi::SubsystemObjectDestroyFunc* surface_destroy_func = nullptr;
        try
        {
            surface_destroy_func =
                arc::io::dl::bind_symbol<omi::SubsystemObjectDestroyFunc>(
                    m_lib_handle,
                    "OMI_CONTEXT_surface_destroy"
                );
        }
        catch(const std::exception& exc)
        {
            global::logger->error
                << "Failed to bind context with error: " << exc.what()
                << std::endl;
        }

        // build objects and pass to the respective parts of the engine
        omi::context::ContextSubsystem::set_implementation(
            new omi::SubsytemObject<omi::context::ContextSubsystem>(
                subsystem_factory_func,
                subsystem_destroy_func
            )
        );
        omi::context::Surface::set_implementation(
            new omi::SubsytemObject<omi::context::Surface>(
                surface_factory_func,
                surface_destroy_func
            )
        );

        return true;
    }

    void release()
    {
        // destroy implementation objects
        omi::context::Surface::destroy();
        omi::context::ContextSubsystem::destroy();

        // close the library
        if(m_lib_handle != nullptr)
        {
            arc::io::dl::close_library(m_lib_handle);
            m_lib_handle = nullptr;
        }
    }

    const arc::str::UTF8String& get_version() const
    {
        return m_version;
    }
};

//------------------------------------------------------------------------------
//                                  CONSTRUCTOR
//------------------------------------------------------------------------------

ContextSSDL::ContextSSDL()
    : m_impl(new ContextSSDLImpl())
{
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

ContextSSDL::~ContextSSDL()
{
    delete m_impl;
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

bool ContextSSDL::bind(const arc::io::sys::Path& path)
{
    return m_impl->bind(path);
}

void ContextSSDL::release()
{
    m_impl->release();
}

const arc::str::UTF8String& ContextSSDL::get_version() const
{
    return m_impl->get_version();
}

} // namespace ss
} // namespace runtime
} // namespace omi
