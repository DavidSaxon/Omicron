/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_API_ASSET_ASSETLIBRARY_HPP_
#define OMICRON_API_ASSET_ASSETLIBRARY_HPP_

#include <arcanecore/base/lang/Restrictors.hpp>
#include <arcanecore/base/str/UTF8String.hpp>

#include "omicron/api/API.hpp"


namespace omi
{
namespace asset
{

//------------------------------------------------------------------------------
//                              FORWARD DECLARATIONS
//------------------------------------------------------------------------------

class Geometry;

// TODO: DOC
class AssetLibrary
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
public:



    //--------------------------------------------------------------------------
    //                          PUBLIC STATIC FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns the singleton instance of the AssetLibrary.
     */
    OMI_API_GLOBAL static AssetLibrary* instance();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    //-----------------------------ENGINE INTERNALS-----------------------------
    // hide from doxygen
    #ifndef IN_DOXYGEN

    /*!
     * \brief Initialises Omicron's AssetLibrary.
     */
    OMI_API_GLOBAL void startup_routine();

    /*!
     * \brief Shutdowns Omicron's AssetLibrary.
     */
    OMI_API_GLOBAL void shutdown_routine();

    #endif
    // IN_DOXYGEN
    //--------------------------------------------------------------------------

    // TODO: DOC
    OMI_API_GLOBAL void load_blocking(const arc::str::UTF8String& id);

    // TODO:
    // void load_ansyc()

    // TODO: release blocking

    // TODO: release async

    // TODO: DOC
    // omi::asset::AbstractAsset* get();

    // TODO:
    OMI_API_GLOBAL omi::asset::Geometry* get_geometry(
            const arc::str::UTF8String& id);

    // TODO: load request packets

    // TODO: how to run multi-threaded to get data onto GPU

    // TODO: how to know when to release data
    //       e.g. static geo can be loaded into VBOs released
    //       e.g. textures can be loaded straight onto the GPU and then released
    //       e.g. prehaps its better not too, since it requires everything to be
    //            on the GPU...
    //            perhaps we hold copies in memory with an out-of-core limit
    //
    //      -> note on texture loading - textures and resources are different
    //         image types

private:

    //--------------------------------------------------------------------------
    //                            COMPILATION FIREWALL
    //--------------------------------------------------------------------------

    class AssetLibraryImpl;
    AssetLibraryImpl* m_impl;

    //--------------------------------------------------------------------------
    //                            PRIVATE CONSTRUCTOR
    //--------------------------------------------------------------------------

    /*!
     * \brief Constructs the AssetLibrary instance.
     */
    AssetLibrary();

    //--------------------------------------------------------------------------
    //                             PRIVATE DESTRUCTOR
    //--------------------------------------------------------------------------

    ~AssetLibrary();
};

} // namespace asset
} // namespace omi

#endif
