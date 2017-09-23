/*!
 * \file
 * \author David Saxon
 * \brief Defines the interface for the raw loader function.
 */
#ifndef OMICRON_API_RES_LOADERS_RAWLOADER_HPP_
#define OMICRON_API_RES_LOADERS_RAWLOADER_HPP_

#include <arcanecore/io/sys/FileReader.hpp>

#include "omicron/api/common/Attributes.hpp"


namespace omi
{
namespace res
{

//------------------------------------------------------------------------------
//                                   FUNCTIONS
//------------------------------------------------------------------------------

/*!
 * \brief Loads the file as a MapAttribute with a single ByteAttribute entry
 *        named "raw" which contains the entire data of the file.
 */
omi::Attribute load_raw(arc::io::sys::FileReader& reader);

} // namespace res
} // namespace omi

#endif
