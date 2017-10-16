/*!
 * \file
 * \author David Saxon
 * \brief Defines the ResourceId type and functionality for generating ids.
 */
#ifndef OMICRON_API_RES_RESOURCEID_HPP_
#define OMICRON_API_RES_RESOURCEID_HPP_

#include <arcanecore/base/str/UTF8String.hpp>
#include <arcanecore/io/sys/Path.hpp>

#include "omicron/api/API.hpp"


namespace omi
{
namespace res
{

//------------------------------------------------------------------------------
//                                TYPE DEFINITIONS
//------------------------------------------------------------------------------

/*!
 * \brief Integral that can be used to uniquely identify resources.
 */
typedef arc::uint64 ResourceId;

//------------------------------------------------------------------------------
//                                   FUNCTIONS
//------------------------------------------------------------------------------

/*!
 * \brief Returns a ResourceId for the given resource path (represented
 *        as a string).
 */
OMI_API_EXPORT ResourceId get_id(const arc::str::UTF8String& resource_path);

/*!
 * \brief Returns a ResourceId for the given resource path.
 */
OMI_API_EXPORT ResourceId get_id(const arc::io::sys::Path& resource_path);

} // namespace res
} // namespace omi

#endif
