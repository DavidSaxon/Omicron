/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_API_SCENE_COMPONENT_RENDERABLE_ABSTRACTRENDERABLE_HPP_
#define OMICRON_API_SCENE_COMPONENT_RENDERABLE_ABSTRACTRENDERABLE_HPP_

#include "omicron/api/API.hpp"
#include "omicron/api/scene/component/AbstractComponent.hpp"


namespace omi
{
namespace scene
{

//------------------------------------------------------------------------------
//                                     ENUMS
//------------------------------------------------------------------------------

/*!
 * \brief Defines the different derived renderable types.
 */
enum class RenderableType
{
    kCamera,
    kMesh
    // TODO: more types
};

/*!
 * \brief Defines a component that will be handled by the render subsystem.
 */
class AbstractRenderable
    : public omi::scene::AbstractComponent
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTOR
    //--------------------------------------------------------------------------

    AbstractRenderable()
        : omi::scene::AbstractComponent()
    {
    }

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    virtual ~AbstractRenderable()
    {
    }

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    virtual ComponentType get_component_type() const override
    {
        return omi::scene::ComponentType::kRenderable;
    }

    /*!
     * \brief Returns the type of this renderable component.
     */
    virtual RenderableType get_renderable_type() const = 0;
};

} // namespace scene
} // namespace omi

#endif
