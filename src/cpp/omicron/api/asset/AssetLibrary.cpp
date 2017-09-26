#include "omicron/api/asset/AssetLibrary.hpp"

#include <memory>

#include <arcanecore/col/Accessor.hpp>
#include <arcanecore/col/Reader.hpp>

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

    // TODO:

public:

    //--------------------------C O N S T R U C T O R---------------------------

    AssetLibraryImpl()
    {
    }

    //---------------------------D E S T R U C T O R----------------------------

    ~AssetLibraryImpl()
    {
    }

    //-------------P U B L I C    M E M B E R    F U N C T I O N S--------------

    bool startup_routine()
    {
        // create the logging profile
        arc::log::Profile profile("OMICRON-ASSET");
        // vend the input from the shared handler
        omi::asset::global::logger =
            omi::report::log_handler.vend_input(profile);

        global::logger->debug << "AssetLibrary startup." << std::endl;

        return true;
    }

    bool shutdown_routine()
    {
        global::logger->debug << "AssetLibrary shutdown." << std::endl;

        return true;
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

OMI_API_GLOBAL bool AssetLibrary::startup_routine()
{
    return m_impl->startup_routine();
}

OMI_API_GLOBAL bool AssetLibrary::shutdown_routine()
{
    return m_impl->shutdown_routine();
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
