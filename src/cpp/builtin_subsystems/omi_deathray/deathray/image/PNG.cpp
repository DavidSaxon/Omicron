#include "deathray/image/PNG.hpp"

#include <cstring>
#include <png.h>

#include <arcanecore/base/Exceptions.hpp>


namespace death
{
namespace image
{
namespace png
{

void load_file(
        const arc::io::sys::Path& path,
        std::size_t& out_width,
        std::size_t& out_height,
        bool& out_has_alpha,
        unsigned char*& out_data)
{
    FILE* file_in = fopen(path.to_native().get_raw(), "rb");

    if(file_in == nullptr)
    {
        arc::str::UTF8String error_message;
        error_message
            << "Cannot access file \"" << path << "\"";
        throw arc::ex::IOError(error_message);
    }

    // setup up the libpng read structure
    png_structp read_ptr = png_create_read_struct(
        PNG_LIBPNG_VER_STRING,
        nullptr,
        nullptr,
        nullptr
    );
    if(read_ptr == nullptr)
    {
        fclose(file_in);
        throw arc::ex::RuntimeError(
            "Failed to instantiate libpng read structure."
        );
    }

    // setup the libpng info structure
    png_infop info_ptr = png_create_info_struct(read_ptr);
    if(info_ptr == nullptr)
    {
        png_destroy_read_struct(&read_ptr, png_infopp_NULL, png_infopp_NULL);
        fclose(file_in);
        throw arc::ex::RuntimeError(
            "Failed to instantiate libpng info structure."
        );
    }

    // perform jump
    if(setjmp(png_jmpbuf(read_ptr)))
    {
        png_destroy_read_struct(&read_ptr, png_infopp_NULL, png_infopp_NULL);
        fclose(file_in);
        throw arc::ex::RuntimeError(
            "Failed to perform libpng jump."
        );
    }

    // setup
    png_init_io(read_ptr, file_in);
    unsigned int sig_read = 0;
    png_set_sig_bytes(read_ptr, sig_read);

    // read the file
    png_read_png(
        read_ptr,
        info_ptr,
        PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND,
        png_voidp_NULL
    );

    // get the size
    out_width = static_cast<std::size_t>(info_ptr->width);
    out_height = static_cast<std::size_t>(info_ptr->height);

    // handle the format
    switch(info_ptr->color_type)
    {
        case PNG_COLOR_TYPE_RGBA:
        {
            out_has_alpha = true;
            break;
        }
        case PNG_COLOR_TYPE_RGB:
        {
            out_has_alpha = false;
            break;
        }
        default:
        {
            throw arc::ex::ParseError(
                "Png file has an unsupported data type."
            );
            break;
        }
    }

    // allocate the data buffer
    std::size_t row_bytes =
        static_cast<std::size_t>(png_get_rowbytes(read_ptr, info_ptr));
    out_data = new unsigned char[row_bytes * out_height];

    // memcpy into the return data
    png_bytepp row_pointers = png_get_rows(read_ptr, info_ptr);
    for(std::size_t i = 0; i < out_height; ++i)
    {
        std::memcpy(
            out_data + (row_bytes * (out_height - 1 - i)),
            row_pointers[i],
            row_bytes
        );
    }

    // clean up
    png_destroy_read_struct(&read_ptr, &info_ptr, png_infopp_NULL);
    fclose(file_in);
}

} // namespace png
} // namespace image
} // namespace death
