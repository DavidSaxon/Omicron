/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_API_SCENE_COMPONENT_RENDERABLE_MESH_HPP_
#define OMICRON_API_SCENE_COMPONENT_RENDERABLE_MESH_HPP_

#include "omicron/api/scene/component/renderable/AbstractRenderable.hpp"

#include "omicron/api/API.hpp"
#include "omicron/api/res/ResourceId.hpp"

namespace omi
{
namespace scene
{

/*!
 * \brief A component that represents a geometric mesh to be rendered by the
 *        renderer subsystem.
 *
 * A mesh consists of an array of vertex positions and 0 or more arrays of
 * vertex attributes.
 */
class Mesh
    : public omi::scene::AbstractRenderable
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTORS
    //--------------------------------------------------------------------------

    /*!
     * \brief Resource constructor.
     *
     * \throw arc::ex::KeyError If there is no resource with the given id.
     * \throw arc::ex::ValueError If the data from the given resource is not
     *                            valid for a mesh component.
     */
    OMI_API_EXPORT Mesh(omi::res::ResourceId resource);

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    OMI_API_EXPORT virtual ~Mesh();

public:

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    OMI_API_EXPORT virtual RenderableType get_renderable_type() const override;

    // TODO: deindex

    // TODO: reindex

private:

    //--------------------------------------------------------------------------
    //                            COMPILATION FIREWALL
    //--------------------------------------------------------------------------

    class MeshImpl;
    MeshImpl* m_impl;
};

} // namespace scene
} // namespace omi

#endif
