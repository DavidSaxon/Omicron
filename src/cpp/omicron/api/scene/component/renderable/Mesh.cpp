#include "omicron/api/scene/component/renderable/Mesh.hpp"

#include "omicron/api/common/Attributes.hpp"
#include "omicron/api/res/ResourceRegistry.hpp"
#include "omicron/api/scene/SceneGlobals.hpp"


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

    // The parent map attribute of this mesh's data.
    omi::MapAttribute m_data;

    // vertex positions
    omi::FloatAttribute m_vertex_positions;

public:

    //--------------------------C O N S T R U C T O R---------------------------

    MeshImpl(omi::res::ResourceId resource)
    {
        omi::Attribute data =
            omi::res::ResourceRegistry::instance()->get(resource);
        // check
        if(!validate(data))
        {
            enter_error_state();
        }
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

    const std::vector<float>& get_vertex_positions() const
    {
        return m_vertex_positions.get_values();
    }

private:

    //------------P R I V A T E    M E M B E R    F U N C T I O N S-------------

    // checks and returns whether the given attribute is valid mesh data
    bool validate(const omi::Attribute& data)
    {
        omi::MapAttribute root = data;
        if(!root.is_valid())
        {
            global::logger->warning
                << "Invalid mesh data: root attribute is not a valid "
                << "MapAttribute." << std::endl;
            return false;
        }

        m_data = root.get("geometry");
        if(!m_data.is_valid())
        {
            global::logger->warning
                << "Invalid mesh data: no valid geometry MapAttribute."
                << std::endl;
            return false;
        }

        m_vertex_positions = m_data.get("vertex.positions");
        if(!m_vertex_positions.is_valid())
        {
            global::logger->warning
                << "Invalid mesh data: no valid geometry.vertex.positions "
                << "FloatAttribute." << std::endl;
            return false;
        }

        return true;
    }

    // sets the mesh to render an error state
    void enter_error_state()
    {
        // clear current state
        m_data = omi::MapAttribute();
        m_vertex_positions = omi::FloatAttribute();

        // TODO: render explanation mark;
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

OMI_API_EXPORT const std::vector<float>& Mesh::get_vertex_positions() const
{
    return m_impl->get_vertex_positions();
}

} // namespace scene
} // namespace omi
