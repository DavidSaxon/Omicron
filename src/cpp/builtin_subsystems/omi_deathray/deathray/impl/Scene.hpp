/*!
 * \file
 * \author David Saxon
 * \date 2017
 * \copyright Copyright (C) The Arcane Initiative - All Rights Reserved
 *            Unauthorized copying of this file, via any medium is strictly
 *            prohibited
 */
#ifndef DEATHRAY_IMPL_SCENE_HPP_
#define DEATHRAY_IMPL_SCENE_HPP_

#include <unordered_set>
#include <vector>

#include <arcanecore/base/lang/Restrictors.hpp>
#include <arcanecore/lx/Vector.hpp>

#include "deathray/api/API.h"
#include "deathray/api/Scene.h"


namespace death
{

//------------------------------------------------------------------------------
//                              FORWARD DECLARATIONS
//------------------------------------------------------------------------------

class Camera;
class Octree;
class Spatial;
class View;


// TODO: DOC
class Scene
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTOR
    //--------------------------------------------------------------------------

    // TODO: DOC
    Scene(DeathSceneHandle handle);

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    virtual ~Scene();

    //--------------------------------------------------------------------------
    //                          PUBLIC: MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns the resolution this scene is rendering to.
     */
    const arc::lx::Vector2u& get_resolution() const;

    /*!
     * \brief Implementation of the death_scene_set_resolution function.
     */
    DeathError set_resolution(const arc::lx::Vector2u& resolution);

    /*!
     * \brief Returns the camera being used by this scene.
     */
    death::Camera* get_camera();

    /*!
     * \brief Implementation of the death_scene_set_camera function.
     */
    DeathError set_camera(death::Camera* camera);

    /*!
     * \brief Return the set of spatial entities within this scene.
     */
    const std::unordered_set<death::Spatial*> get_spatials() const;

    /*!
     * \brief Implementation of the death_scene_add_spatial function.
     */
    DeathError add_spatial(death::Spatial* spatial);

    /*!
     * \brief Implementation of the death_scene_remove_spatial function.
     */
    DeathError remove_spatial(death::Spatial* spatial);

    /*!
     * \brief Gets or creates and returns the Octree object for the given
     *        spatial.
     */
    death::Octree* get_or_create_octree(death::Spatial* spatial);

    /*!
     * \brief Returns the views this scene is using.
     */
    const std::vector<death::View*>& get_views() const;

    /*!
     * \brief Implementation of the death_scene_render function.
     */
    DeathError render();

    /*!
     * \brief Destroys objects within this scene that are staged for cleanup but
     *        require a graphics state to perform cleanup.
     */
    void graphics_state_cleanup();

private:

    //--------------------------------------------------------------------------
    //                            COMPILATION FIREWALL
    //--------------------------------------------------------------------------

    class SceneImpl;
    SceneImpl* m_impl;
};

} // namespace death

//------------------------------------------------------------------------------
//                             API STRUCT DEFINITION
//------------------------------------------------------------------------------

struct DeathScene
{
    death::Scene* impl;
    DeathScene() : impl(nullptr) {};
};

#endif
