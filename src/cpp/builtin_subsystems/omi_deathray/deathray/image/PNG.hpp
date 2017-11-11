/*!
 * \file
 * \author David Saxon
 * \date 2017
 * \copyright Copyright (C) The Arcane Initiative - All Rights Reserved
 *            Unauthorized copying of this file, via any medium is strictly
 *            prohibited
 */
#ifndef DEATH_IMAGE_PNG_HPP_
#define DEATH_IMAGE_PNG_HPP_

#include <arcanecore/io/sys/Path.hpp>


namespace death
{
namespace image
{

/*!
 * \brief Utilities for png image data.
 */
namespace png
{

/*!
 * \brief Attempts to load a png image from the given file path.
 *
 * \param path The file system path of the png image to load into memory.
 * \param out_width Returns the width (in pixels) of the image data.
 * \param out_height Returns the height (in pixels) of the image data.
 * \param out_has_alpha Returns whether the image has an alpha channel.
 * \param Retruns the allocated image data - this must be deleted by the caller.
 *
 * \throw arc::ex::IOError The given file path cannot be accessed.
 * \throw arc::ex::RuntimeError If there is an error with the png reading
 *                              library.
 * \throw arc::ex::ParseError The file cannot be parsed as a png image.
 */
void load_file(
        const arc::io::sys::Path& path,
        std::size_t& out_width,
        std::size_t& out_height,
        bool& out_has_alpha,
        unsigned char*& out_data);

} // namespace png

} // namespace image
} // namespace death

#endif
