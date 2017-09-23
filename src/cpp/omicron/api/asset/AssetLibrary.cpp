#include "omicron/api/asset/AssetLibrary.hpp"

#include <memory>

#include <arcanecore/col/Accessor.hpp>
#include <arcanecore/col/Reader.hpp>
#include <arcanecore/config/Document.hpp>
#include <arcanecore/config/visitors/Shorthand.hpp>

#include "omicron/api/asset/AssetGlobals.hpp"
#include "omicron/api/asset/types/Geometry.hpp"
#include "omicron/api/config/ConfigInline.hpp"
#include "omicron/api/report/Logging.hpp"


namespace omi
{
namespace asset
{

//------------------------------------------------------------------------------
//                                 IMPLEMENTATION
//------------------------------------------------------------------------------

class AssetLibrary::AssetLibraryImpl
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
private:

    //-------------------P R I V A T E    A T T R I B U T E S-------------------

    /*!
     * \brief The config document for the asset library.
     */
    arc::config::DocumentPtr m_config_data;
    /*!
     * \brief Access to the resource directory through ArcaneCore Collate.
     */
    std::unique_ptr<arc::col::Accessor> m_accessor;

public:

    // TODO: how is this stuff booted? Should the asset the library actually be
    //       provided from the truntime?
    //       think is the same result as a hidden startup function...
    // Look at report

    //--------------------------C O N S T R U C T O R---------------------------

    AssetLibraryImpl()
    {
    }

    //---------------------------D E S T R U C T O R----------------------------

    ~AssetLibraryImpl()
    {
    }

    //-------------P U B L I C    M E M B E R    F U N C T I O N S--------------

    void startup_routine()
    {
        // create the logging profile
        arc::log::Profile profile("OMICRON-ASSET");
        // vend the input from the shared handler
        omi::asset::global::logger =
            omi::report::log_handler.vend_input(profile);

        global::logger->debug << "AssetLibrary startup." << std::endl;

        // build the path to the configuration data
        arc::io::sys::Path config_path(omi::asset::global::config_root_dir);
        config_path << "library.json";

        // built-in memory data
        static const arc::str::UTF8String config_compiled(
            OMICRON_CONFIG_INLINE_ASSET_LIBRARY
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
        m_accessor.reset(new arc::col::Accessor(toc_path));
    }

    void shutdown_routine()
    {
        global::logger->debug << "AssetLibrary shutdown." << std::endl;

        m_config_data.reset();
    }

    // TODO: REMOVE ME
    void load_blocking(const arc::str::UTF8String& id)
    {
    }

    omi::asset::Geometry* get_geometry(const arc::str::UTF8String& id)
    {


        // TODO:

        return nullptr;
    }
};

//------------------------------------------------------------------------------
//                            PUBLIC STATIC FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_GLOBAL AssetLibrary* AssetLibrary::instance()
{
    static AssetLibrary inst;
    return &inst;
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_GLOBAL void AssetLibrary::startup_routine()
{
    m_impl->startup_routine();
}

OMI_API_GLOBAL void AssetLibrary::shutdown_routine()
{
    m_impl->shutdown_routine();
}

OMI_API_GLOBAL void AssetLibrary::load_blocking(const arc::str::UTF8String& id)
{
    m_impl->load_blocking(id);
}

OMI_API_GLOBAL omi::asset::Geometry* AssetLibrary::get_geometry(
        const arc::str::UTF8String& id)
{
    return m_impl->get_geometry(id);
}

//------------------------------------------------------------------------------
//                              PRIVATE CONSTRUCTOR
//------------------------------------------------------------------------------

AssetLibrary::AssetLibrary()
    : m_impl(new AssetLibraryImpl())
{
}

//------------------------------------------------------------------------------
//                               PRIVATE DESTRUCTOR
//------------------------------------------------------------------------------

AssetLibrary::~AssetLibrary()
{
    delete m_impl;
}

} // namespace asset
} // namespace omi
