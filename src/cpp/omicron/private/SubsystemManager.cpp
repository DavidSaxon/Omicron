#include "omicron/private/SubsystemManager.hpp"

#include <set>

#include <arcanecore/base/os/OSOperations.hpp>
#include <arcanecore/io/dl/DLOperations.hpp>
#include <arcanecore/io/sys/FileSystemOperations.hpp>

#include <metaengine/visitors/Path.hpp>
#include <metaengine/visitors/String.hpp>

#include "omicron/private/Logging.hpp"
#include "omicron/private/MetaCompiled.hpp"

namespace omi_
{

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

bool SubsystemManager::startup()
{
    // bail out if we're already initialised
    if(m_initialised)
    {
        return true;
    }

    omi_::logger->debug << "Initialising SubsystemManager." << std::endl;

    // build the path to the base subsystem document
    arc::io::sys::Path meta_path(util::meta::META_CORE_DIR);
    meta_path << "subsystems" << "subsystems.json";

    // build in-memory data
    static const arc::str::UTF8String meta_compiled(
        OMI_METACOMPILED_SUBSYSTEMS);

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

    // build the mapping from names to the expected roles
    NamesToRoles names_to_roles;
    build_names_to_roles_mapping(names_to_roles);

    // access the subsystem searchpaths
    std::vector<arc::io::sys::Path> search_paths = *m_metadata->get(
        "search_paths",
        metaengine::PathVectorV::instance()
    );
    if(search_paths.empty())
    {
        omi_::logger->warning
            << "No subsystem search paths provided." << std::endl;
    }

    // get the extension of subsystem files
    arc::str::UTF8String extension(*m_metadata->get(
        "extension",
        metaengine::UTF8StringV::instance()
    ));
    if(!extension.is_empty())
    {
        extension = arc::str::UTF8String(".") + extension;
    }

    for(auto sp = search_paths.begin(); sp != search_paths.end(); ++sp)
    {
        omi_::logger->info
            << "Searching path for subsystems: \"" << *sp << "\"" << std::endl;

        // list the files in this directory
        std::vector<arc::io::sys::Path> search_files(arc::io::sys::list(*sp));
        for(auto sf = search_files.begin(); sf != search_files.end(); ++sf)
        {
            // only check files with the extension
            if(!extension.is_empty() && !sf->get_back().ends_with(extension))
            {
                continue;
            }

            arc::str::UTF8String ss_name;
            arc::str::UTF8String ss_version;
            ossRole ss_roles = OSS_ROLE_NONE;
            if(bind_subsystem(*sf, ss_name, ss_version, ss_roles))
            {
                omi_::logger->debug
                    << "Found subsystem: \"" << ss_name << "-" << ss_version
                    << "\"" << std::endl;
                // has this subsystem been requested for any roles
                NamesToRoles::const_iterator f_name =
                    names_to_roles.find(ss_name);
                if(f_name != names_to_roles.end())
                {
                    // TODO: load for roles
                    // remove from the mapping since we don't want to search for
                    // any other subsystems with this name
                    names_to_roles.erase(f_name);
                }
            }
        }
    }

    // have all the required subsystems been loaded?
    if(!names_to_roles.empty())
    {
        // report
        omi_::logger->error << "Failed to find required subsystem";
        if(names_to_roles.size() > 1)
        {
            omi_::logger->error << "s";
        }
        omi_::logger->error << " with name";
        if(names_to_roles.size() > 1)
        {
            omi_::logger->error << "s";
        }
        omi_::logger->error << ": ";
        for(auto it = names_to_roles.begin(); it != names_to_roles.end();)
        {
            omi_::logger->error << "\"" << it->first << "\"";
            ++it;
            if(it == names_to_roles.end())
            {
                break;
            }
            omi_::logger->error << ", ";
        }
        omi_::logger->error << std::endl;

        return false;
    }

    // initialisation succesfully completed
    m_initialised = true;
    return true;
}

void SubsystemManager::shutdown()
{
    // TODO:
}

//------------------------------------------------------------------------------
//                              PRIVATE CONSTRUCTORS
//------------------------------------------------------------------------------

SubsystemManager::SubsystemManager()
    :
    m_initialised(false)
{
}

//------------------------------------------------------------------------------
//                            PRIVATE STATIC FUNCTIONS
//------------------------------------------------------------------------------

void SubsystemManager::update_roles(
        const arc::str::UTF8String& name,
        ossRole role,
        NamesToRoles& names_to_roles)
{
    NamesToRoles::iterator f_name = names_to_roles.find(name);
    if(f_name != names_to_roles.end())
    {
        names_to_roles[name] =
            static_cast<ossRole>(names_to_roles[name] | role);
    }
    else
    {
        names_to_roles[name] = role;
    }
}

//------------------------------------------------------------------------------
//                            PRIVATE MEMBER FUNCTIONS
//------------------------------------------------------------------------------

void SubsystemManager::build_names_to_roles_mapping(
        NamesToRoles& names_to_roles)
{
    update_roles(
        *m_metadata->get(
            "roles.window_manager",
            metaengine::UTF8StringV::instance()
        ),
        OSS_ROLE_WINDOW_MANAGER,
        names_to_roles
    );
    update_roles(
        *m_metadata->get(
            "roles.input",
            metaengine::UTF8StringV::instance()
        ),
        OSS_ROLE_INPUT,
        names_to_roles
    );
    update_roles(
        *m_metadata->get(
            "roles.ui",
            metaengine::UTF8StringV::instance()
        ),
        OSS_ROLE_UI,
        names_to_roles
    );
}

bool SubsystemManager::bind_subsystem(
        const arc::io::sys::Path& path,
        arc::str::UTF8String& name,
        arc::str::UTF8String& version,
        ossRole& roles)
{
    omi_::logger->debug
        << "Attempting to bind subsystem from: \"" << path << "\"."
        << std::endl;

    arc::io::dl::Handle handle = nullptr;

    // open a handle to the library
    try
    {
        handle = arc::io::dl::open_library(path);
    }
    catch(const std::exception& exc)
    {
        omi_::logger->warning
            << "Failed to bind subsystem from file \"" << path << "\" with "
            << "message: \"" << exc.what() << "\"" << std::endl;
        return false;
    }
    assert(handle != nullptr);

    // get the name of the register function
    arc::str::UTF8String register_func_name(*m_metadata->get(
        "register_func_name",
        metaengine::UTF8StringV::instance()
    ));

    // find the register function
    RegisterFunc* register_func = nullptr;
    try
    {
        register_func =
            arc::io::dl::bind_symbol<RegisterFunc>(handle, register_func_name);
    }
    catch(const std::exception& exc)
    {
        omi_::logger->warning
            << "Failed to bind register function from file \"" << path
            << "\" with message: \"" << exc.what() << "\"" << std::endl;
        // bind failed, attempt to close the library
        try
        {
            arc::io::dl::close_library(handle);
        }
        catch(...) {}
        return false;
    }

    // call the register function, if this throws out, we will just let it.
    // failing during this function is fairly critical
    char* c_name = nullptr;
    char* c_version = nullptr;
    (*register_func)(&c_name, &c_version, roles);

    // build the return values, (and take ownership of the memory)
    name.claim(c_name);
    version.claim(c_version);

    return true;
}

} // namespace omi_
