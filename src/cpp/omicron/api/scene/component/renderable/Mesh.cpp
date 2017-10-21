#include "omicron/api/scene/component/renderable/Mesh.hpp"


namespace omi
{
namespace scene
{

//------------------------------------------------------------------------------
//                                 IMPLEMENTATION
//------------------------------------------------------------------------------

class Mesh::MeshImpl
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
private:

    //-------------------P R I V A T E    A T T R I B U T E S-------------------

    // TODO:

public:

    //--------------------------C O N S T R U C T O R---------------------------

    MeshImpl(omi::res::ResourceId resource)
    {
    }

    //---------------------------D E S T R U C T O R----------------------------

    ~MeshImpl()
    {
    }

    //-------------P U B L I C    M E M B E R    F U N C T I O N S--------------

    RenderableType get_renderable_type() const
    {
        return omi::scene::RenderableType::kMesh;
    }
};

//------------------------------------------------------------------------------
//                                  CONSTRUCTOR
//------------------------------------------------------------------------------

OMI_API_EXPORT Mesh::Mesh(omi::res::ResourceId resource)
    : AbstractRenderable()
    , m_impl            (new MeshImpl(resource))
{
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

OMI_API_EXPORT Mesh::~Mesh()
{
    delete m_impl;
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_EXPORT RenderableType Mesh::get_renderable_type() const
{
    return m_impl->get_renderable_type();
}

} // namespace scene
} // namespace omi
