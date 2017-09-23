#include <arcanecore/base/Exceptions.hpp>
#include <arcanecore/io/sys/FileReader.hpp>
#include <arcanecore/lx/Vector.hpp>

#include "omicron/api/common/Attributes.hpp"
#include "omicron/api/res/DefineLoader.hpp"

// TODO: REMOVE ME
#include <iostream>


namespace omi
{
namespace res
{
namespace
{

//------------------------------------------------------------------------------
//                                   CONSTANTS
//------------------------------------------------------------------------------

// The stride of a geometry position (in floats).
static const std::size_t POSITION_STRIDE = 3;

//------------------------------------------------------------------------------
//                                   FUNCTIONS
//------------------------------------------------------------------------------

// loads a Wavefront obj file into Omicron attributes
static omi::Attribute load_obj(arc::io::sys::FileReader& reader)
{
    // TODO: REMOVE ME
    std::cout << "LOADING OBJ FILE!" << std::endl;

    // TODO: time this an optimise file reading

    // per point positions
    std::vector<float> point_positions;
    // per vertex positions
    std::vector<float> vertex_positions;

    // TODO: reserve this list (can try read from file)

    // iterate over each line of the file
    while(!reader.eof())
    {
        // TODO: fastest to probably be able to get data from stream without
        // copying
        // TODO: move out?
        arc::str::UTF8String line;
        reader.read_line(line);

        // position?
        if(line.starts_with("v"))
        {
            // TODO: split with start from and end with
            // split on space
            std::vector<arc::str::UTF8String> values = line.split(" ");
            // right number of values?
            if(values.size() != (POSITION_STRIDE + 1))
            {
                // TODO: REMOVE ME
                std::cout << "throwing out" << std::endl;
                throw arc::ex::ParseError(
                    "Invalid position line: \"" + line + "\""
                );
            }
            point_positions.push_back(values[1].to_float());
            point_positions.push_back(values[2].to_float());
            point_positions.push_back(values[3].to_float());
        }
        else if(line.starts_with("f"))
        {
            // TODO: split with start from and end with
            // split on space
            std::vector<arc::str::UTF8String> values = line.split(" ");
            // right number of values?
            if(values.size() != 4)
            {
                throw arc::ex::ParseError("Non-triangle face line: " + line);
            }

            // TODO: support normals and uv etc
            // get indices
            std::size_t i0 =
                static_cast<std::size_t>(values[1].to_uint64()) *
                POSITION_STRIDE;
            std::size_t i1 =
                static_cast<std::size_t>(values[2].to_uint64()) *
                POSITION_STRIDE;
            std::size_t i2 =
                static_cast<std::size_t>(values[3].to_uint64()) *
                POSITION_STRIDE;

            // add to vertex positions
            // point 0
            vertex_positions.push_back(point_positions[i0 + 0]);
            vertex_positions.push_back(point_positions[i0 + 1]);
            vertex_positions.push_back(point_positions[i0 + 2]);
            // point 1
            vertex_positions.push_back(point_positions[i1 + 0]);
            vertex_positions.push_back(point_positions[i1 + 1]);
            vertex_positions.push_back(point_positions[i1 + 2]);
            // point 2
            vertex_positions.push_back(point_positions[i2 + 0]);
            vertex_positions.push_back(point_positions[i2 + 1]);
            vertex_positions.push_back(point_positions[i2 + 2]);
        }
    }

    // TODO: builder
    // build the data to return
    omi::MapAttribute::DataType vertex_map_data = {
        {
            "positions",
            omi::FloatAttribute(vertex_positions, POSITION_STRIDE, false)
        }
    };
    omi::MapAttribute vertex_map(vertex_map_data, false);

    omi::MapAttribute::DataType geometry_map_data = {
        {"vertex", omi::MapAttribute(vertex_map_data, false)}
    };

    omi::MapAttribute::DataType root_data = {
        {"geometry", omi::MapAttribute(geometry_map_data, false)}
    };

    // TODO: REMOVE ME
    std::cout << "OBJ LOAD SUCCESS!" << std::endl;

    return omi::MapAttribute(root_data, false);
};

} // namespace anonymous
} // namespace res
} // namespace omi

OMICRON_API_RES_DEFINE_LOADER(omi::res::load_obj, "obj");
