/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_API_ASSET_RESOURCES_OBJ_HPP_
#define OMICRON_API_ASSET_RESOURCES_OBJ_HPP_

#include "omicron/api/asset/resource/AbstractResource.hpp"


namespace omi
{
namespace asset
{

// TODO: DOC
class OBJResource : public AbstractResource
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTORS
    //--------------------------------------------------------------------------

    // TODO: DOC
    OMI_API_GLOBAL OBJResource();

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    OMI_API_GLOBAL virtual ~OBJResource();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    // override
    OMI_API_GLOBAL virtual void load(arc::io::sys::FileReader* reader);

    // override
    OMI_API_GLOBAL virtual void release();

private:

    //--------------------------------------------------------------------------
    //                            COMPILATION FIREWALL
    //--------------------------------------------------------------------------

    class OBJResourceImpl;
    OBJResourceImpl* m_impl;

};

} // namespace asset
} // namespace omi

#endif
