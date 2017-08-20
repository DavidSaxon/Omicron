/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_API_ASSET_ABSTRACTRESOURCE_HPP_
#define OMICRON_API_ASSET_ABSTRACTRESOURCE_HPP_

#include <arcanecore/base/lang/Restrictors.hpp>
#include <arcanecore/io/sys/FileReader.hpp>

#include "omicron/api/API.hpp"


namespace omi
{
namespace asset
{

// TODO: DOC
class AbstractResource
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTORS
    //--------------------------------------------------------------------------

    // TODO: DOC
    // TODO: takes reader?
    OMI_API_GLOBAL AbstractResource()
    {
    }

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    OMI_API_GLOBAL virtual ~AbstractResource()
    {
    }

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    // TODO: DOC
    // TODO: takes reader?
    OMI_API_GLOBAL virtual void load(arc::io::sys::FileReader* reader) = 0;

    // TODO: loading stage with GPU? could subclass with GPU resource

    // TODO: DOC
    OMI_API_GLOBAL virtual void release() = 0;

    // TODO: some form of access could be purely implementation specific
};

} // namespace asset
} // namespace omi

#endif
