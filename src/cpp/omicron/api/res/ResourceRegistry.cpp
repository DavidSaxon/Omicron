#include "omicron/api/res/ResourceRegistry.hpp"

#include <memory>
#include <unordered_map>

#include <arcanecore/col/Accessor.hpp>
#include <arcanecore/col/Reader.hpp>
#include <arcanecore/config/Document.hpp>
#include <arcanecore/config/visitors/Shorthand.hpp>

#include "omicron/api/res/ResourceGlobals.hpp"
#include "omicron/api/config/ConfigInline.hpp"
#include "omicron/api/report/Logging.hpp"


namespace omi
{
namespace res
{

//------------------------------------------------------------------------------
//                                 IMPLEMENTATION
//------------------------------------------------------------------------------

class ResourceRegistry::ResourceRegistryImpl
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
private:

    //-------------------P R I V A T E    A T T R I B U T E S-------------------

    /*!
     * \brief The config document for the ResourceRegistry.
     */
    arc::config::DocumentPtr m_config_data;
    /*!
     * \brief Access to the resource directory through ArcaneCore Collate.
     */
    std::unique_ptr<arc::col::Accessor> m_accessor;

    /*!
     * \brief Mapping from discovered resource Ids to the associated paths.
     */
    std::unordered_map<ResourceId, arc::io::sys::Path> m_entries;

public:

    //--------------------------C O N S T R U C T O R---------------------------

    ResourceRegistryImpl()
    {
    }

    //---------------------------D E S T R U C T O R----------------------------

    ~ResourceRegistryImpl()
    {
    }

    //-------------P U B L I C    M E M B E R    F U N C T I O N S--------------

    bool startup_routine()
    {
        // create the logging profile
        arc::log::Profile profile("OMICRON-RES");
        // vend the input from the shared handler
        omi::res::global::logger =
            omi::report::log_handler.vend_input(profile);

        global::logger->debug << "ResourceRegistry startup." << std::endl;

        // build the path to the configuration data
        arc::io::sys::Path config_path(omi::res::global::config_root_dir);
        config_path << "registry.json";

        // built-in memory data
        static const arc::str::UTF8String config_compiled(
            OMICRON_CONFIG_INLINE_RES_REGISTRY
        );

        // construct the document
        m_config_data.reset(new arc::config::Document(
            config_path,
            &config_compiled
        ));

        // using real files?
        if(*m_config_data->get("use_real_files", AC_BOOLV))
        {
            arc::col::Accessor::force_real_resources = true;
            global::logger->warning
                << "Using file-system resources rather than collated "
                << "resources." << std::endl;
        }

        // get the path to the data directory
        arc::io::sys::Path toc_path(
            *m_config_data->get("data_directory", AC_PATHV)
        );
        // get the name of the table of contents
        toc_path << (*m_config_data->get("table_of_contents", AC_U8STRV));

        // setup the accessor
        try
        {
            m_accessor.reset(new arc::col::Accessor(toc_path));
        }
        catch(const std::exception& exc)
        {
            global::logger->critical
                << "Encountered error while attempt to create resource "
                << "accessor: " << exc.what() << std::endl;
            return false;
        }

        // get the path to the res directory
        arc::io::sys::Path res_path(
            *m_config_data->get("resource_directory", AC_PATHV)
        );

        // discover resources
        std::vector<arc::io::sys::Path> resource_files =
            m_accessor->list_rec(res_path);
        // TODO: accessor list should skip directories
        // TODO: REMOVE ME
        global::logger->critical << "RESOURCES: " << std::endl;
        for(const arc::io::sys::Path& p : resource_files)
        {
            global::logger->critical << "\t-> " << p << std::endl;
        }

        return true;
    }

    bool shutdown_routine()
    {
        global::logger->debug << "ResourceRegistry shutdown." << std::endl;

        m_accessor.reset();
        m_config_data.reset();

        return true;
    }
};

//------------------------------------------------------------------------------
//                            PUBLIC STATIC FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_GLOBAL ResourceRegistry* ResourceRegistry::instance()
{
    static ResourceRegistry inst;
    return &inst;
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_GLOBAL bool ResourceRegistry::startup_routine()
{
    return m_impl->startup_routine();
}

OMI_API_GLOBAL bool ResourceRegistry::shutdown_routine()
{
    return m_impl->shutdown_routine();
}

//------------------------------------------------------------------------------
//                              PRIVATE CONSTRUCTOR
//------------------------------------------------------------------------------

ResourceRegistry::ResourceRegistry()
    : m_impl(new ResourceRegistryImpl())
{
}

//------------------------------------------------------------------------------
//                               PRIVATE DESTRUCTOR
//------------------------------------------------------------------------------

ResourceRegistry::~ResourceRegistry()
{
    delete m_impl;
}

} // namespace res
} // namespace omi
