/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_DEATHRAY_RENDERABLE_CAMERA_HPP_
#define OMICRON_DEATHRAY_RENDERABLE_CAMERA_HPP_

#include <arcanecore/base/lang/Restrictors.hpp>

#include <omicron/api/scene/component/renderable/Camera.hpp>

#include <deathray/api/Scene.h>


namespace omi_death
{

/*!
 * \brief Omicron-DeathRay renderable type that handles a camera component.
 */
class DeathCamera
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTOR
    //--------------------------------------------------------------------------

    /*!
     * \brief Creates a new handler for the given camera component.
     */
    DeathCamera(omi::scene::Camera* component, DeathSceneHandle scene);

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    virtual ~DeathCamera();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Applies this camera to the scene.
     *
     * \note This is only called if this is the active camera.
     */
    void apply();

    /*!
     * \brief Applies this camera to the scene as the debug camera.
     */
    void apply_debug();

private:

    //--------------------------------------------------------------------------
    //                            COMPILATION FIREWALL
    //--------------------------------------------------------------------------

    class DeathCameraImpl;
    DeathCameraImpl* m_impl;
};

} // namespace omi_death

#endif
