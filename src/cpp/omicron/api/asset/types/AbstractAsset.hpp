/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_API_ASSET_TYPES_ABSTRACT_HPP_
#define OMICRON_API_ASSET_TYPES_ABSTRACT_HPP_

#include <arcanecore/base/lang/Restrictors.hpp>

#include "omicron/api/API.hpp"


namespace omi
{
namespace asset
{

// TODO: DOC:
class AbstractAsset
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
public:

    //--------------------------------------------------------------------------
    //                                INNER TYPES
    //--------------------------------------------------------------------------

    // TODO: DOC:
    class AbstractResource
        : private arc::lang::Noncopyable
        , private arc::lang::Nonmovable
        , private arc::lang::Noncomparable
    {
        // TODO:
    };

    // TODO:
};

} // namespace asset
} // namespace omi

#endif
