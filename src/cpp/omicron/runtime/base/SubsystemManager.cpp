#include "omicron/runtime/base/SubsystemManager.hpp"

#include <cassert>
#include <unordered_set>

#include <arcanecore/base/Preproc.hpp>
#include <arcanecore/io/sys/FileSystemOperations.hpp>

#include <metaengine/visitors/Shorthand.hpp>

#include "omicron/runtime/base/Logging.hpp"
#include "omicron/runtime/meta/MetaCompiled.hpp"
#include "omicron/runtime/meta/MetaGlobals.hpp"

// TODO: REMOVE ME
// #include <omicron/Proto.hpp>

namespace omi
{
namespace runtime
{

//------------------------------------------------------------------------------
//                                TYPE DEFINITIONS
//------------------------------------------------------------------------------

namespace
{

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

    omi::runtime::logger->debug << "SubsystemManager startup." << std::endl;

    // // TODO: REMOVE ME
    // omi::runtime::logger->warning
    //     << "global var: " << omi::proto::global_var << std::endl;
    // omi::runtime::logger->warning
    //     << "func: " << omi::proto::test_me() << std::endl;
    // omi::proto::global_var = "MO FUCK!";


    // build the path to the base subsystem document
    arc::io::sys::Path meta_path(omi::runtime::meta::global::runtime_dir);
    meta_path << "subsystems" << "subsystems.json";

    // build in-memory data
    static const arc::str::UTF8String meta_compiled(
        OMICRON_RUNTIME_METACOMPILED_SUBSYSTEMS);

    // construct the variant
    m_metadata.reset(new metaengine::Variant(
        meta_path,
        &meta_compiled,
        "windows"
    ));
    // use unix variant?
    #ifdef ARC_OS_UNIX
        m_metadata->set_variant("linux");
    #endif

    // get the subsystem searchpaths
    std::vector<arc::io::sys::Path> m_search_path = *m_metadata->get(
        "search_paths", ME_PATHVECV
    );
    if(m_search_path.empty())
    {
        omi::runtime::logger->error
            << "No subsystem search paths provided." << std::endl;
    }

    // get the subsystem extension
    m_extension =  "." + *m_metadata->get(
        "extension", ME_U8STRV
    );

    // fill out the potential paths of subsystems based on the existing files
    // within the search path
    for(const arc::io::sys::Path& path : m_search_path)
    {
        omi::runtime::logger->info
            << "Searching path for potential subsytem libraries: \"" << path
            << "\"" << std::endl;

        // handle a file in the search path
        if(arc::io::sys::is_file(path))
        {
            add_if_ptoential_path(path);
        }
        // handle directories in the search path
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
            omi::runtime::logger->debug
                << "Invalid path used in search path: \"" << path << "\""
                << std::endl;
        }
    }

    // get the name of the register function
    m_register_symbol = *m_metadata->get(
        "register_symbol", ME_U8STRV
    );

    // register the subsystems requested for each role
    bind_role(omi::ss::Subsystem::kRoleWindowManager);
    bind_role(omi::ss::Subsystem::kRoleInput);
    bind_role(omi::ss::Subsystem::kRoleUI);
    bind_role(omi::ss::Subsystem::kRoleRenderer);
    bind_role(omi::ss::Subsystem::kRolePhysics);
    bind_role(omi::ss::Subsystem::kRoleAudio);

    omi::runtime::logger->debug << "Booting subsystems." << std::endl;
    // iterate through the subsystems and boot them (but only once)
    std::unordered_set<omi::ss::Subsystem*> booted;
    for(auto role : m_assigned)
    {
        for(omi::ss::Subsystem* subsystem : role.second)
        {
            // check if it's been booted already
            if(booted.find(subsystem) == booted.end())
            {
                subsystem->boot();
                booted.insert(subsystem);
            }
        }
    }

    // // TODO: REMOVE ME
    // omi::runtime::logger->warning
    //     << "global var: " << omi::proto::global_var << std::endl;
    // omi::runtime::logger->warning
    //     << "func: " << omi::proto::test_me() << std::endl;
    // omi::proto::global_var = "MO FUCK!";
}

void SubsystemManager::shutdown()
{
    omi::runtime::logger->debug << "SubsystemManager shutdown." << std::endl;

    // TODO: shutdown the specific roles

    // clear the list of assigned subsystems
    m_assigned.clear();

    // destroy the bound subsystems
    for(auto bound : m_bind_storage)
    {
        omi::runtime::logger->debug
            << "Destroying subsystem: " << bound.first << std::endl;
        delete bound.second;
    }
    m_bind_storage.clear();

    // close the open dynamic libraries
    for(auto library : m_dl_handles)
    {
        omi::runtime::logger->debug
            << "Closing subsystem library: " << library.first << std::endl;
        arc::io::dl::close_library(library.second);
    }
    m_dl_handles.clear();
}

omi::ss::Subsystem* SubsystemManager::get_subsystem(
        omi::ss::Subsystem::Role role)
{
    assert(m_assigned.find(role) != m_assigned.end());
    return m_assigned[role][0];
}

SubsystemManager::SubsystemArray SubsystemManager::get_subsystems(
        omi::ss::Subsystem::Role role)
{
    assert(m_assigned.find(role) != m_assigned.end());
    return m_assigned[role];
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
        omi::runtime::logger->warning
            << "Potential subsystem library at: \"" << path << "\" masked by "
            << "a file with the same name defined earlier in the search path: "
            << "\"" << f_lib->second << "\"" << std::endl;
        return;
    }

    omi::runtime::logger->debug
        << "Identified potential subsystem library at: " << path << std::endl;

    // add to the global mapping
    m_potential[lib_name] = path;
}

void SubsystemManager::bind_role(omi::ss::Subsystem::Role role)
{
    // get the name of the role
    arc::str::UTF8String role_name = omi::ss::Subsystem::role_to_string(role);

    // load the metadata for the subsystems to use for this role
    std::vector<arc::str::UTF8String> subsystem_names = *m_metadata->get(
        "roles." + role_name, ME_U8STRVECV
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
                omi::runtime::logger->error
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
            omi::runtime::logger->error
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
            omi::runtime::logger->error
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
            omi::runtime::logger->error
                << "Encountered error while attempting to register subsystem "
                << "from library: \"" << f_potential->second << "\", which was "
                << "requested as a secondary subsystem for the role: \""
                << role_name << "\", with message: \"" << exc.what() << "\""
                << std::endl;
            continue;
        }

        omi::runtime::logger->info
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
        omi::runtime::logger->error
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

} // namespace runtime
} // namespace omi
