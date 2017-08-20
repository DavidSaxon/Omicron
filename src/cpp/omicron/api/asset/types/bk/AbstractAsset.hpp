/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_API_ASSET_TYPES_ABSTRACTASSET_HPP_
#define OMICRON_API_ASSET_TYPES_ABSTRACTASSET_HPP_

#include <arcanecore/base/lang/Restrictors.hpp>

#include "omicron/api/API.hpp"


namespace omi
{
namespace asset
{

// TODO: should the sub asset types be move to their own modules in the API
//       (probably then they can go under just an asset folder and not a types
//        one)

// TODO: DOC
class AbstractAsset
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTORS
    //--------------------------------------------------------------------------

    // TODO:
    OMI_API_GLOBAL AbstractAsset()
    {
    }

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    OMI_API_GLOBAL virtual ~AbstractAsset()
    {
    }
}

} // namespace asset
} // namespace omi

#endif
