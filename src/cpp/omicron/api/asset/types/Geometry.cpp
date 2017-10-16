#include "omicron/api/asset/types/Geometry.hpp"

#include <arcanecore/base/Exceptions.hpp>

#include "omicron/api/common/Attributes.hpp"


namespace omi
{
namespace asset
{

//------------------------------------------------------------------------------
//                                 IMPLEMENTATION
//------------------------------------------------------------------------------

class Geometry::GeometryImpl
{
private:

    //-------------------P R I V A T E    A T T R I B U T E S-------------------

    // TODO: index list?
    omi::FloatAttribute m_positions;
    // TODO: where are normals and uvs stored? as attribute assets or with the
    //       geometry

public:

    //--------------------------C O N S T R U C T O R---------------------------

    GeometryImpl()
    {
    }

    //---------------------------D E S T R U C T O R----------------------------

    ~GeometryImpl()
    {
    }

    //-------------P U B L I C    M E M B E R    F U N C T I O N S--------------

    void init(const omi::MapAttribute& data)
    {
        // get the vertex data - this will throw out if the key does not exist
        m_positions = data["geometry.vertex.positions"];
        // check valid?
        if(m_positions.get_size() == 0)
        {
            throw arc::ex::ValueError("Empty position data supplied");
        }
        if(m_positions.get_size() % 3 != 0)
        {
            throw arc::ex::ValueError(
                "Position data with size that is not a multiple of 3"
            );
        }
    }

    void release()
    {
        m_positions = omi::FloatAttribute();
    }

    std::size_t get_vertex_size() const
    {
        // TODO:
        return m_positions.get_size() / 3;
    }

    const std::vector<float>& get_positions() const
    {
        return m_positions.get_values();
    }
};

//------------------------------------------------------------------------------
//                                  CONSTRUCTOR
//------------------------------------------------------------------------------

OMI_API_EXPORT Geometry::Geometry()
    : m_impl(new GeometryImpl())
{
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

OMI_API_EXPORT Geometry::~Geometry()
{
    delete m_impl;
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_EXPORT void Geometry::init(const omi::MapAttribute& data)
{
    m_impl->init(data);
}

OMI_API_EXPORT void Geometry::release()
{
    m_impl->release();
}

OMI_API_EXPORT std::size_t Geometry::get_vertex_size() const
{
    return m_impl->get_vertex_size();
}

OMI_API_EXPORT const std::vector<float>& Geometry::get_positions() const
{
    return m_impl->get_positions();
}

} // namespace asset
} // namespace omi
