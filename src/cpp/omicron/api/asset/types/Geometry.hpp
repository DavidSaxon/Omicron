/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_API_ASSET_TYPES_GEOMETRY_HPP_
#define OMICRON_API_ASSET_TYPES_GEOMETRY_HPP_

#include "omicron/api/asset/types/AbstractAsset.hpp"


namespace omi
{
namespace asset
{

class Geometry : public AbstractAsset
{
public:

    //--------------------------------------------------------------------------
    //                            RESOURCE DEFINITION
    //--------------------------------------------------------------------------

    class Resource : public AbstractAsset::AbstractResource
    {

    };

    // TODO: only problem with this method is that the resource has to be
    //       aquired to get the asset
};

} // namespace asset
} // namespace omi


#endif
