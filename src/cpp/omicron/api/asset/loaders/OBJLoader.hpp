/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_API_ASSET_LOADER_OBJLOADER_HPP_
#define OMICRON_API_ASSET_LOADER_OBJLOADER_HPP_

#include <arcanecore/io/sys/FileReader.hpp>

#include "omicron/api/API.hpp"
#include "omicron/api/common/attribute/Attribute.hpp"


namespace omi
{
namespace asset
{

// TODO: DOC
class OBJLoader
{
public:

    //--------------------------------------------------------------------------
    //                          PUBLIC STATIC FUNCTIONS
    //--------------------------------------------------------------------------

    // TODO: DOC
    // TODO: this data is taken and stored by a wrapper object
    OMI_API_GLOBAL static omi::Attribute load(arc::io::sys::FileReader& reader);
};

} // namespace asset
} // namespace omi

#endif
