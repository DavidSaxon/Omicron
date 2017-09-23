#include "omicron/api/res/loaders/RawLoader.hpp"


namespace omi
{
namespace res
{

//------------------------------------------------------------------------------
//                                   FUNCTIONS
//------------------------------------------------------------------------------

omi::Attribute load_raw(arc::io::sys::FileReader& reader)
{
    // get the number of byes to read
    arc::int64 length = reader.get_size();

    // skip the BOM if there is one
    if(reader.has_bom())
    {
        arc::int64 bom_size = static_cast<arc::int64>(reader.get_bom_size());
        // make sure we're not reading past the BOM
        if(length > bom_size)
        {
            reader.seek(bom_size);
            length -= bom_size;
        }
    }

    // read the entire file
    char* data = new char[static_cast<std::size_t>(length)];
    reader.read(data, length);

    // TODO: this induces an extra copy - can we avoid by giving attribute some
    //       sort of claim functionality?
    // build the data into attributes
    omi::MapAttribute::DataType map_data = {
        {"raw", omi::ByteAttribute(data, data + length)}
    };

    // clean up
    delete[] data;

    return omi::MapAttribute(map_data);
}

} // namespace res
} // namespace omi
