#include "omicron/api/res/ResourceRegistry.hpp"

#include <memory>
#include <unordered_map>
#include <unordered_set>

#include <arcanecore/col/Accessor.hpp>
#include <arcanecore/col/Reader.hpp>
#include <arcanecore/config/Document.hpp>
#include <arcanecore/config/visitors/Shorthand.hpp>

#include "omicron/api/common/attribute/Attribute.hpp"
#include "omicron/api/config/ConfigInline.hpp"
#include "omicron/api/report/Logging.hpp"
#include "omicron/api/report/stats/StatsDatabase.hpp"
#include "omicron/api/res/ResourceGlobals.hpp"
#include "omicron/api/res/loaders/RawLoader.hpp"


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

    // The config document for the ResourceRegistry.
    arc::config::DocumentPtr m_config_data;

    // Access to the resource directory through ArcaneCore Collate.
    std::unique_ptr<arc::col::Accessor> m_accessor;

    // Mapping from defined extensions to their respective loaders.
    std::unordered_map<arc::str::UTF8String, LoaderFunc*> m_loaders;

    // Mapping from discovered resource Ids to the associated paths.
    std::unordered_map<ResourceId, arc::io::sys::Path> m_entries;

    // Stores the data of the currently loaded resources
    std::unordered_map<ResourceId, omi::Attribute> m_resources;

    #ifndef OMI_API_MODE_PRODUCTION
        // records the resources that have been loaded at least once
        std::unordered_set<ResourceId> m_load_records;
    #endif

    // stats
    omi::Int32Attribute m_stat_loads;
    omi::Int32Attribute m_stat_raw_loads;
    omi::Int32Attribute m_stat_redunant_loads;
    #ifndef OMI_API_MODE_PRODUCTION
        omi::Int32Attribute m_stat_reoccurring_loads;
    #endif


public:

    //--------------------------C O N S T R U C T O R---------------------------

    ResourceRegistryImpl()
        : m_stat_loads         (0, false)
        , m_stat_raw_loads     (0, false)
        , m_stat_redunant_loads(0, false)
        #ifndef OMI_API_MODE_PRODUCTION
            , m_stat_reoccurring_loads(0, false)
        #endif
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
        // compute the Id and add an entry for each discovered resource
        for(const arc::io::sys::Path& resource : resource_files)
        {
            ResourceId id = omi::res::get_id(resource);

            // check for hash collisions in non-production builds
            #ifndef OMI_API_MODE_PRODUCTION
                // TODO: REMOVE ME
                auto f_collision_check = m_entries.find(id);
                if(f_collision_check != m_entries.end())
                {
                    global::logger->critical
                        << "Detected ResourceId collision on id: " << id
                        << "which has been generated by both \""
                        << f_collision_check->second.to_unix() << "\" and \""
                        << resource.to_unix() << "\"" << std::endl;
                    continue;
                }
            #endif

            m_entries.insert(std::make_pair(id, resource));
        }

        // TODO: should move stats out of the sub directory
        // set up the stats
        omi::report::StatsDatabase::instance()->define_entry(
            "ResourceRegistry.loads",
            m_stat_loads
        );
        omi::report::StatsDatabase::instance()->define_entry(
            "ResourceRegistry.raw_loads",
            m_stat_raw_loads
        );
        omi::report::StatsDatabase::instance()->define_entry(
            "ResourceRegistry.redunant_loads",
            m_stat_redunant_loads
        );
        #ifndef OMI_API_MODE_PRODUCTION
            omi::report::StatsDatabase::instance()->define_entry(
                "ResourceRegistry.reoccurring_loads",
                m_stat_reoccurring_loads
            );
        #endif


        return true;
    }

    bool shutdown_routine()
    {
        global::logger->debug << "ResourceRegistry shutdown." << std::endl;

        m_entries.clear();
        m_accessor.reset();
        m_config_data.reset();

        return true;
    }

    void define_loader(
            LoaderFunc* function,
            const arc::str::UTF8String& extension)
    {
        // ensure the loader hasn't already been defined
        #ifndef OMI_API_MODE_PRODUCTION
            auto f_loader = m_loaders.find(extension);
            if(f_loader != m_loaders.end())
            {
                global::logger->error
                    << "Multiple loaders defined for the \"" << extension
                    << "\" extension." << std::endl;
                return;
            }
        #endif

        // store
        m_loaders.insert(std::make_pair(extension, function));
    }

    void load_blocking(ResourceId id)
    {
        // early exit if the resource is already loaded
        auto f_resource = m_resources.find(id);
        if(f_resource != m_resources.end())
        {
            // record in stats
            m_stat_redunant_loads.set_at(0, m_stat_redunant_loads.at(0) + 1);
            return;
        }

        // ensure the resource exists
        auto f_entry = m_entries.find(id);
        if(f_entry == m_entries.end())
        {
            arc::str::UTF8String error_message;
            error_message
                << "No entry in ResourceRegistry with id: "
                << std::to_string(id);
            throw arc::ex::KeyError(error_message);
        }

        // get the extension of the resource
        arc::str::UTF8String ext = f_entry->second.get_extension();

        // open the file reader
        arc::col::Reader reader(f_entry->second, m_accessor.get());

        // record the number of loads
        m_stat_loads.set_at(0, m_stat_loads.at(0) + 1);

        // record the number of reoccurring loads
        #ifndef OMI_API_MODE_PRODUCTION
            auto f_record = m_load_records.find(id);
            if(f_record != m_load_records.end())
            {
                m_stat_reoccurring_loads.set_at(
                    0,
                    m_stat_reoccurring_loads.at(0) + 1
                );
            }
            else
            {
                m_load_records.insert(id);
            }
        #endif

        // find the loader for this extension
        auto f_loader = m_loaders.find(ext);
        if(f_loader != m_loaders.end())
        {
            // TODO: stat load -> stat duplicate loads
            // do load
            m_resources.insert(std::make_pair(id, f_loader->second(reader)));
        }
        // fallback to the raw loader
        else
        {
            // record in stats
            m_stat_raw_loads.set_at(0, m_stat_raw_loads.at(0) + 1);
            m_resources.insert(std::make_pair(id, omi::res::load_raw(reader)));
        }
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

OMI_API_GLOBAL void ResourceRegistry::define_loader(
        LoaderFunc* function,
        const arc::str::UTF8String& extension)
{
    m_impl->define_loader(function, extension);
}

OMI_API_GLOBAL void ResourceRegistry::load_blocking(ResourceId id)
{
    m_impl->load_blocking(id);
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
