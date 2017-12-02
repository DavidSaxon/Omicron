/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_API_SCENE_SCENESTATE_HPP_
#define OMICRON_API_SCENE_SCENESTATE_HPP_

#include <arcanecore/base/lang/Restrictors.hpp>
#include <arcanecore/base/str/UTF8String.hpp>

#include "omicron/api/API.hpp"
#include "omicron/api/GameInterface.hpp"


namespace omi
{
namespace scene
{

//------------------------------------------------------------------------------
//                              FORWARD DECLARATIONS
//------------------------------------------------------------------------------

class Camera;
class Entity;

/*!
 * \brief The SceneState is a global singleton that manages the state of the
 *        Omicron scene and the entities within it.
 */
class SceneState
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
public:

    //--------------------------------------------------------------------------
    //                          PUBLIC STATIC FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns the singleton SceneState instance.
     */
    OMI_API_EXPORT static SceneState& instance();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    //-----------------------------ENGINE INTERNALS-----------------------------
    // hide from doxygen
    #ifndef IN_DOXYGEN

    /*!
     * \brief Initialises Omicron's SceneState.
     */
    OMI_API_EXPORT bool startup_routine();

    /*!
     * \brief Shutdowns Omicron's SceneState.
     */
    OMI_API_EXPORT bool shutdown_routine();

    /*!
     * \brief Defines a entity by identifier name and the functions for creating
     *        and destroying instances of the entity.
     *
     * \throw arc::ex::KeyError If there is already an entity defined with the
     *                          given id.
     */
    OMI_API_EXPORT void define_entity(
            const arc::str::UTF8String& id,
            omi::GameEntityCreate* create_func,
            omi::GameEntityDestroy* destroy_func);

    /*!
     * \brief Reforms a per-frame update of all entities in the scene.
     */
    OMI_API_EXPORT void update();

    #endif
    // IN_DOXYGEN
    //--------------------------------------------------------------------------

    /*!
     * \brief Constructs and adds a new entity to the SceneState.
     *
     * If this function is called within the SceneState's update cycle it will
     * be construct at the call time and updated at the end of the current
     * update cycle.
     *
     * \param id The identifier string of the entity type to construct.
     * \param name The name to given to the new entity.
     * \param data The data to pass to the new entity.
     *
     * \throw arc::ex::KeyError If there is no entity type defined with the
     *                          given id.
     */
    OMI_API_EXPORT void new_entity(
            const arc::str::UTF8String& id,
            const arc::str::UTF8String& name = "",
            const omi::Attribute& data = omi::Attribute());

    /*!
     * \brief Returns the camera component that is currently being used to
     *        render the scene.
     *
     * \note This may be null if no active camera has been set.
     */
    OMI_API_EXPORT const omi::scene::Camera* get_active_camera() const;

    /*!
     * \brief Sets the camera component that will be used to render the scene.
     */
    OMI_API_EXPORT void set_active_camera(const omi::scene::Camera* camera);

    /*!
     * \brief Returns camera component that is being used for the debug
     *        perspective of the scene.
     *
     * \note This may be null if no debug camera has been set.
     */
    OMI_API_EXPORT const omi::scene::Camera* get_debug_camera() const;

    /*!
     * \brief Sets the camera component that will be used for the debug
     *        perspective of the scene.
     */
    OMI_API_EXPORT void set_debug_camera(const omi::scene::Camera* camera);

private:

    //--------------------------------------------------------------------------
    //                            PRIVATE CONSTRUCTOR
    //--------------------------------------------------------------------------

    SceneState();

    //--------------------------------------------------------------------------
    //                             PRIVATE DESTRUCTOR
    //--------------------------------------------------------------------------

    ~SceneState();

    //--------------------------------------------------------------------------
    //                            COMPILATION FIREWALL
    //--------------------------------------------------------------------------

    class SceneStateImpl;
    SceneStateImpl* m_impl;
};

} // namespace scene
} // namespace

#endif
