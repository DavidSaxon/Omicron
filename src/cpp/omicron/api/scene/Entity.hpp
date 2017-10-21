/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_API_SCENE_ENTITY_HPP_
#define OMICRON_API_SCENE_ENTITY_HPP_

#include <list>

#include <arcanecore/base/lang/Restrictors.hpp>
#include <arcanecore/base/str/UTF8String.hpp>

#include "omicron/api/API.hpp"
#include "omicron/api/common/Attributes.hpp"


namespace omi
{
namespace scene
{

//------------------------------------------------------------------------------
//                              FORWARD DECLARATIONS
//------------------------------------------------------------------------------

class AbstractComponent;
class AbstractRenderable;
class SceneState;

/*!
 * \brief An Entity is the core component of Omicron. It is a game object that
 *        is update once per frame and is managed by the SceneState.
 *
 * Entity implementations must be derived from this base class.
 */
class Entity
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
private:

    //--------------------------------------------------------------------------
    //                                  FRIENDS
    //--------------------------------------------------------------------------

    friend class SceneState;

public:

    // move everything protected?

    //--------------------------------------------------------------------------
    //                                CONSTRUCTOR
    //--------------------------------------------------------------------------

    /*!
     * \brief Super constructor.
     *
     * \param name The name of this Entity instance (if this entity needs to be
     *             looked by its name this should be unique across the current
     *             SceneState.)
     */
    OMI_API_EXPORT Entity(const arc::str::UTF8String& name);

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    OMI_API_EXPORT virtual ~Entity();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns the name of this Entity instance.
     */
    OMI_API_EXPORT const arc::str::UTF8String& get_name() const;

    /*!
     * \brief Returns the list of current renderable components of this entity.
     */
    OMI_API_EXPORT
    const std::list<AbstractRenderable*>& get_renderable_components() const;

protected:

    //--------------------------------------------------------------------------
    //                         PROTECTED MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    //--------------------V I R T U A L    F U N C T I O N S--------------------

    /*!
     * \brief Is called once per frame before rendering to update the state of
     *        this Entity.
     */
    OMI_API_EXPORT virtual void update() = 0;

    // TODO: physics update?

    //--------------------------------------------------------------------------

    /*!
     * \brief Adds the given component to this entity.
     *
     * This entity will manage deleting the component.
     *
     * \throw arc::ex::ValueError If this component has already been added to
     *                            this entity.
     */
    OMI_API_EXPORT void add_component(AbstractComponent* component);

    // TODO: add update dependencies

private:

    //--------------------------------------------------------------------------
    //                            COMPILATION FIREWALL
    //--------------------------------------------------------------------------

    class EntityImpl;
    EntityImpl* m_impl;
};

} // namespace scene
} // namespace

#endif
