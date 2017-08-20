#include "omicron/api/asset/AssetLibrary.hpp"

#include <memory>

#include <arcanecore/col/Accessor.hpp>
#include <arcanecore/config/Document.hpp>
#include <arcanecore/config/visitors/Shorthand.hpp>

#include "omicron/api/asset/AssetGlobals.hpp"
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
{
private:

    //-------------------P R I V A T E    A T T R I B U T E S-------------------

    /*!
     * \brief Whether the asset library is current initialised or not.
     */
    bool m_initialised;
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
        : m_initialised(false)
    {
    }

    //---------------------------D E S T R U C T O R----------------------------

    ~AssetLibraryImpl()
    {
    }

    //-------------P U B L I C    M E M B E R    F U N C T I O N S--------------

    void startup_routine()
    {
        // skip if the asset library is already initialised
        if(m_initialised)
        {
            return;
        }

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
        // skip if the asset library is not yet initialised
        if(!m_initialised)
        {
            return;
        }

        global::logger->debug << "AssetLibrary shutdown." << std::endl;
    }
};


//------------------------------------------------------------------------------
//                                     HANDLE
//------------------------------------------------------------------------------

//-----------------------------D E S T R U C T O R------------------------------

OMI_API_GLOBAL AssetLibrary::~AssetLibrary()
{
    delete m_impl;
}

//---------------P U B L I C    S T A T I C    F U N C T I O N S----------------

OMI_API_GLOBAL AssetLibrary* AssetLibrary::instance()
{
    static AssetLibrary inst;
    return &inst;
}

//---------------P U B L I C    M E M B E R    F U N C T I O N S----------------

OMI_API_GLOBAL void AssetLibrary::startup_routine()
{
    m_impl->startup_routine();
}

OMI_API_GLOBAL void AssetLibrary::shutdown_routine()
{
    m_impl->shutdown_routine();
}

//--------------------P R I V A T E    C O N S T R U C T O R--------------------

AssetLibrary::AssetLibrary()
    : m_impl(new AssetLibraryImpl())
{
}

} // namespace asset
} // namespace omi
