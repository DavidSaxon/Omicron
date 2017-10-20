/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_API_SCENE_SCENESTATE_HPP_
#define OMICRON_API_SCENE_SCENESTATE_HPP_

#include <arcanecore/base/lang/Restrictors.hpp>

#include "omicron/api/API.hpp"


namespace omi
{
namespace scene
{

//------------------------------------------------------------------------------
//                              FORWARD DECLARATIONS
//------------------------------------------------------------------------------

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
     * \brief Reforms a per-frame update of all entities in the scene.
     */
    OMI_API_EXPORT void update();

    #endif
    // IN_DOXYGEN
    //--------------------------------------------------------------------------

    /*!
     * \brief Adds a Entity to the SceneState, this will mean the entity will
     *        begin to receive per frame updates.
     *
     * \note The SceneState will take ownership of this Entity and handle
     *       deleting it.
     */
    OMI_API_EXPORT void add_entity(Entity* entity);

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
