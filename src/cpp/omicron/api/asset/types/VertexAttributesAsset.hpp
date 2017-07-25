/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_API_ASSET_TYPES_VERTEXATTRIBUTES_HPP_
#define OMICRON_API_ASSET_TYPES_VERTEXATTRIBUTES_HPP_

#include "omicron/api/API.hpp"
#include "omicron/api/types/AbstractAsset.hpp"


namespace omi
{
namespace asset
{

// TODO: doc
class VertexAttributes : public AbstractAsset
{
    // each attribute type should have a name
    // is each vertex attribute its own asset type?
    // can be 1d, 2d, 3d, (or 4d) data per vertex
    // can also be defined per vertex or just per face and expected to be
    // interpolated
    // does it have to be expressed for ever vertex in the geometry
};

} // namespace asset
} // namespace omi

#endif
