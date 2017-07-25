/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_API_ASSET_TYPES_MATERIAL_HPP_
#define OMICRON_API_ASSET_TYPES_MATERIAL_HPP_

#include "omicron/api/API.hpp"
#include "omicron/api/types/CompoundAsset.hpp"


namespace omi
{
namespace asset
{

// TODO: doc
class Material : public CompoundAsset
{
    // contains:
    //  - shader
    //  - multiple textures
    //  - shader parameters
    //  - vertex attributes

    // TODO: are shader parameters on this object or a sub type

    // TODO: vertex attributes are probably just a part of the shader parameters

    // TODO: vertex attributes don't work well for volumetric or other effect
    //       types

};

} // namespace asset
} // namespace omi

#endif
