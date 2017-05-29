#include "omicron/runtime/subsystem/SubsystemManager.hpp"

#include <cassert>
#include <unordered_set>

#include <arcanecore/base/Preproc.hpp>
#include <arcanecore/io/sys/FileSystemOperations.hpp>

#include <arcanecore/config/visitors/Shorthand.hpp>

#include <omicron/api/config/ConfigInline.hpp>

#include "omicron/runtime/RuntimeGlobals.hpp"


namespace omi
{
namespace runtime
{
namespace ss
{

namespace
{
//------------------------------------------------------------------------------
//                                TYPE DEFINITIONS
//------------------------------------------------------------------------------

typedef void* (LibRegisterFunc)();

} // namespace anonymous

//------------------------------------------------------------------------------
//                              PRIVATE CONSTRUCTOR
//------------------------------------------------------------------------------

SubsystemManager::SubsystemManager()
{
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

SubsystemManager::~SubsystemManager()
{
}

//------------------------------------------------------------------------------
//                            PUBLIC STATIC FUNCTIONS
//------------------------------------------------------------------------------

SubsystemManager* SubsystemManager::get_instance()
{
    static SubsystemManager instance;
    return &instance;
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

void SubsystemManager::startup()
{
    // bail out if we're already initialised
    if(m_initialised)
    {
        return;
    }

    global::logger->debug << "SubsystemManager startup." << std::endl;

    // build the path to the base subsystem document
    arc::io::sys::Path config_path(omi::runtime::global::config_root_dir);
    config_path << "subsystems" << "subsystems.json";

    // build in-memory data
    static const arc::str::UTF8String config_compiled(
        OMICRON_CONFIG_INLINE_RUNTIME_SUBSYSTEMS);

    // construct the variant
    m_config_data.reset(new arc::config::Variant(
        config_path,
        &config_compiled
    ));
    // use unix variant?
    #ifdef ARC_OS_UNIX
        m_config_data->set_variant("linux");
    #endif

    // get the subsystem searchpaths
    std::vector<arc::io::sys::Path> m_search_path = *m_config_data->get(
        "search_paths", AC_PATHVECV
    );
    if(m_search_path.empty())
    {
        global::logger->error
            << "No subsystem search paths provided." << std::endl;
    }

    // get the subsystem extension
    m_extension =  "." + *m_config_data->get(
        "extension", AC_U8STRV
    );

    // fill out the potential paths of subsystems based on the existing files
    // within the search path
    for(const arc::io::sys::Path& path : m_search_path)
    {
        global::logger->info
            << "Searching path for potential subsytem libraries: \"" << path
            << "\"" << std::endl;

        // handle if the search path is a file
        if(arc::io::sys::is_file(path))
        {
            add_if_ptoential_path(path);
        }
        // handle if the search path is a directory
        else if(arc::io::sys::is_directory(path))
        {
            // evaluate the files in the directory
            for(const arc::io::sys::Path& sub_path : arc::io::sys::list(path))
            {
                if(arc::io::sys::is_file(sub_path))
                {
                    add_if_ptoential_path(sub_path);
                }
            }
        }
        else
        {
            global::logger->debug
                << "Invalid path used in search path: \"" << path << "\""
                << std::endl;
        }
    }

    // bind the subsystems
    m_window.bind(get_or_load_library("window"));

    // TODO: start subsystems separately

    // we're done
    m_initialised = true;
}

void SubsystemManager::shutdown()
{
    global::logger->debug << "SubsystemManager shutdown." << std::endl;

    // release the subsystems
    m_window.release();

    // close the open dynamic libraries
    for(auto library : m_dl_handles)
    {
        global::logger->debug
            << "Closing subsystem library: " << library.first << std::endl;
        arc::io::dl::close_library(library.second);
    }
    m_dl_handles.clear();

    // we're done
    m_initialised = false;
}

void SubsystemManager::start_main_loop(
        omi::window::ss::EngineCycleFunc* engine_cycle_func)
{
    // ensure subsystems are initialised
    if(!m_initialised)
    {
        throw arc::ex::ContextError(
            "Cannot start Omicron main-loop until subsystems have been "
            "initialised"
        );
    }

    m_window.start_main_loop(engine_cycle_func);
}

//------------------------------------------------------------------------------
//                            PRIVATE MEMBER FUNCTIONS
//------------------------------------------------------------------------------

void SubsystemManager::add_if_ptoential_path(const arc::io::sys::Path& path)
{
    // has at least one part and ends with the extension
    if(path.is_empty() || !path.get_back().ends_with(m_extension))
    {
        return;
    }

    // get the name of the potential subsystem library
    arc::str::UTF8String lib_name = path.get_back().substring(
        0,
        path.get_back().get_length()  - m_extension.get_length()
    );

    // has the library been identified already?
    auto f_lib = m_potential.find(lib_name);
    if(f_lib != m_potential.end())
    {
        global::logger->warning
            << "Potential subsystem library at: \"" << path << "\" masked by "
            << "a file with the same name defined earlier in the search path: "
            << "\"" << f_lib->second << "\"" << std::endl;
        return;
    }

    global::logger->debug
        << "Identified potential subsystem library at: " << path << std::endl;

    // add to the global mapping
    m_potential[lib_name] = path;
}

arc::io::dl::Handle SubsystemManager::get_or_load_library(
        const arc::str::UTF8String& role)
{
    // get the name of the subsystem to use
    arc::str::UTF8String ss_name =
        *m_config_data->get("roles." + role, AC_U8STRV);

    if(ss_name.is_empty())
    {
        throw arc::ex::ValidationError(
            "Invalid name supplied for " + role + " subsystem \"" + ss_name +
            "\""
        );
    }

    // get the path for the subsystem
    auto f_path = m_potential.find(ss_name);
    if(f_path == m_potential.end())
    {
        throw arc::ex::ValidationError(
            "No potential " + role + " subsystem found for the name \"" +
            ss_name + "\""
        );
    }

    arc::io::sys::Path ss_path = f_path->second;
    arc::io::dl::Handle ss_handle = nullptr;

    // check if there is an already open dl handle for the subsystem
    auto f_dl_handle = m_dl_handles.find(ss_path);
    if(f_dl_handle == m_dl_handles.end())
    {
        // attempt to open the library
        try
        {
            ss_handle = arc::io::dl::open_library(ss_path);
        }
        catch(const std::exception& exc)
        {
            arc::str::UTF8String error_message;
            error_message
                << "Failed to load " + role + " subsystem library from file \""
                << ss_path << "\", with message: \"" << exc.what() << "\"";
            throw arc::ex::ValidationError(error_message);
        }
        // write the handle to the loaded map
        m_dl_handles.insert(std::make_pair(ss_path, ss_handle));
    }
    else
    {
        // just use already opened handle
        ss_handle = f_dl_handle->second;
    }

    return ss_handle;
}

void SubsystemManager::bind_role(omi::ss::Subsystem::Role role)
{
    // get the name of the role
    arc::str::UTF8String role_name = omi::ss::Subsystem::role_to_string(role);

    // load the config data for the subsystems to use for this role
    std::vector<arc::str::UTF8String> subsystem_names = *m_config_data->get(
        "roles." + role_name, AC_U8STRVECV
    );

    // ensure some names have been supplied
    if(subsystem_names.empty())
    {
        throw arc::ex::ValidationError(
            "No subsystems have been defined for the role: " + role_name
        );
    }

    // iterate through each subsystem and load it or retrieve it from the cache
    bool primary = true;
    for(const arc::str::UTF8String& subsystem_name : subsystem_names)
    {
        // check storage first
        if(m_bind_storage.find(subsystem_name) != m_bind_storage.end())
        {
            assign_from_storage(role, subsystem_name);
            primary = false;
            continue;
        }

        // get the path to the subsystem from the potentials
        auto f_potential = m_potential.find(subsystem_name);
        if(f_potential == m_potential.end())
        {
            // the potential path doesn't exist
            if(primary)
            {
                arc::str::UTF8String error_message;
                error_message
                    << "No subsystem library named \"" <<  subsystem_name
                    << "\" was found, which was requested as the primary "
                    << "subsystem for the role: " << role_name;
                throw arc::ex::ValidationError(error_message);
            }
            else
            {
                global::logger->error
                    << "No subsystem library named \"" <<  subsystem_name
                    << "\" was found, which was requested as a secondary "
                    << "subsystem for the role: " << role_name << std::endl;
                continue;
            }
        }

        // try to open a handle to the library
        arc::io::dl::Handle handle = nullptr;
        try
        {
            handle = arc::io::dl::open_library(f_potential->second);
        }
        catch(const std::exception& exc)
        {
            if(primary)
            {
                arc::str::UTF8String error_message;
                error_message
                    << "Failed to bind subsystem library from file \""
                    << f_potential->second << "\", which was requested as the "
                    << "primary subsystem for the role: \"" << role_name
                    << "\", with message: \"" << exc.what() << "\"";
                throw arc::ex::ValidationError(error_message);
            }
            global::logger->error
                << "Failed to bind subsystem library from file \""
                << f_potential->second << "\", which was requested as a "
                << "secondary subsystem for the role: \"" << role_name
                << "\", with message: \"" << exc.what() << "\"" << std::endl;
            continue;
        }
        assert(handle != nullptr);

        // attempt to find the register function symbol in the library
        LibRegisterFunc* register_func = nullptr;
        try
        {
            register_func = arc::io::dl::bind_symbol<LibRegisterFunc>(
                handle,
                m_register_symbol
            );
        }
        catch(const std::exception& exc)
        {
            arc::io::dl::close_library(handle);
            if(primary)
            {
                arc::str::UTF8String error_message;
                error_message
                    << "Failed to bind register function in subsystem library "
                    << "from file \"" << f_potential->second << "\", which was "
                    << "requested as the primary subsystem for the role: \""
                    << role_name << "\", with message: \"" << exc.what()
                    << "\"";
                throw arc::ex::ValidationError(error_message);
            }
            global::logger->error
                << "Failed to bind register function in subsystem library from "
                << "file \"" << f_potential->second << "\", which was "
                << "requested as a secondary subsystem for the role: \""
                << role_name << "\", with message: \"" << exc.what() << "\""
                << std::endl;
            continue;
        }
        assert(register_func != nullptr);

        omi::ss::Subsystem* subsystem = nullptr;
        try
        {
            subsystem = static_cast<omi::ss::Subsystem*>((*register_func)());
        }
        catch(const std::exception& exc)
        {
            arc::io::dl::close_library(handle);
            if(primary)
            {
                arc::str::UTF8String error_message;
                error_message
                    << "Encountered error while attempting to register "
                    << "subsystem from library: \"" << f_potential->second
                    << "\", which was requested as the primary subsystem for "
                    << "the role: \"" << role_name << "\", with message: \""
                    << exc.what() << "\"";
                throw arc::ex::ValidationError(error_message);
            }
            global::logger->error
                << "Encountered error while attempting to register subsystem "
                << "from library: \"" << f_potential->second << "\", which was "
                << "requested as a secondary subsystem for the role: \""
                << role_name << "\", with message: \"" << exc.what() << "\""
                << std::endl;
            continue;
        }

        global::logger->info
            << "Registered subsystem \"" << subsystem_name << "\" which "
            << "fulfills the roles: "
            << omi::ss::Subsystem::role_to_string(subsystem->get_roles())
            << std::endl;

        // store
        m_dl_handles[f_potential->second] = handle;
        m_bind_storage[subsystem_name] = subsystem;

        // assign
        assign_from_storage(role, subsystem_name);

        primary = false;
    }
}

void SubsystemManager::assign_from_storage(
        omi::ss::Subsystem::Role role,
        const arc::str::UTF8String& subsystem_name)
{
    // get the subsystem with the name from storage
    auto f_subsystem = m_bind_storage.find(subsystem_name);
    assert(f_subsystem != m_bind_storage.end());
    omi::ss::Subsystem* subsystem = f_subsystem->second;

    // identify whether this is the primary subsystem for the role
    bool primary = m_assigned.find(role) == m_assigned.end();

    // ensure the subsystem fulfills the given role
    if((subsystem->get_roles() & role) == 0)
    {
        if(primary)
        {
            arc::str::UTF8String error_message;
            error_message
                << "Cannot assign subsystem \"" << subsystem_name << "\", "
                << "which was requested as the primary subsystem, for the "
                << "role \"" << omi::ss::Subsystem::role_to_string(role)
                << "\" since the subsystem only fulfills these roles: \""
                << omi::ss::Subsystem::role_to_string(subsystem->get_roles())
                << "\"";
            throw arc::ex::ValidationError(error_message);
        }
        global::logger->error
            << "Cannot assign subsystem \"" << subsystem_name << "\", which "
            << "was requested as a secondary subsystem, for the role \""
            << omi::ss::Subsystem::role_to_string(role) << "\" since the "
            << "subsystem only fulfills these roles: \""
            << omi::ss::Subsystem::role_to_string(subsystem->get_roles())
            << "\"" << std::endl;
        return;
    }

    // assign
    if(primary)
    {
        m_assigned[role] = SubsystemArray();
    }
    m_assigned[role].push_back(subsystem);
}

} // namespace ss
} // namespace runtime
} // namespace omi
