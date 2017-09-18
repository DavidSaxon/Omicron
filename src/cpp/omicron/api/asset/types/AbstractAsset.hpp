/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_API_ASSET_TYPES_ABSTRACT_HPP_
#define OMICRON_API_ASSET_TYPES_ABSTRACT_HPP_

#include <arcanecore/base/lang/Restrictors.hpp>

#include "omicron/api/API.hpp"
#include "omicron/api/common/attribute/MapAttribute.hpp"


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
    //                                CONSTRUCTOR
    //--------------------------------------------------------------------------

    // TODO: DOC
    AbstractAsset()
    {
    }

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    virtual ~AbstractAsset()
    {
    }

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    // TODO: DOC
    // can throw out
    // attribute is immutable
    virtual void init(const omi::MapAttribute& data) = 0;

    // TODO: DOC
    virtual void release() = 0;
};

} // namespace asset
} // namespace omi

#endif
