/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_DEATHRAY_SUBSYSTEM_HPP_
#define OMICRON_DEATHRAY_SUBSYSTEM_HPP_

#include <unordered_map>

#include <omicron/api/render/RenderSubsystem.hpp>

#include <deathray/api/Scene.h>

#include <omi_deathray/renderable/DeathCamera.hpp>

// TODO: MOVE TO SOURCE?
#include <list>
#include <GL/glew.h>
#include "omi_deathray/renderable/DeathMesh.hpp"

// TODO: REMOVE ME
#include <arcanecore/lx/Alignment.hpp>
#include <arcanecore/lx/AxisAngle.hpp>
#include <arcanecore/lx/Matrix.hpp>
#include <arcanecore/lx/MatrixMath44f.hpp>
#include <arcanecore/lx/Quaternion.hpp>
#include <arcanecore/lx/Vector.hpp>


namespace omi_death
{

/*!
 * \brief The DeathRay implementation of Omicrons render subsystem.
 */
class DeathSubsystem
    : public omi::render::RenderSubsystem
{
public:

    // TODO: REMOVE ME
    ARC_LX_ALIGNED_NEW

    //--------------------------------------------------------------------------
    //                                CONSTRUCTOR
    //--------------------------------------------------------------------------

    DeathSubsystem();

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    virtual ~DeathSubsystem();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    virtual bool startup_routine() override;

    virtual bool firstframe_routine() override;

    virtual bool shutdown_routine() override;

    virtual void add_renderable(
            omi::scene::AbstractRenderable* renderable) override;

    virtual void remove_renderable(
            omi::scene::AbstractRenderable* renderable) override;

    virtual void set_active_camera(const omi::scene::Camera* camera) override;

    virtual void set_debug_camera(const omi::scene::Camera* camera) override;

    virtual void render() override;

private:

    //--------------------------------------------------------------------------
    //                             PRIVATE ATTRIBUTES
    //--------------------------------------------------------------------------

    // the DeathRay scene
    DeathSceneHandle m_scene;

    // the cameras within the scene (keyed by component id)
    std::unordered_map<omi::scene::ComponentId, DeathCamera*> m_cameras;
    // the camera that is currently active
    DeathCamera* m_active_camera;
    // the debug camera
    DeathCamera* m_debug_camera;

    // TODO: REMOVE ME
    std::list<DeathMesh*> m_meshes;

    // TODO: REMOVE ME
    arc::lx::Matrix44f m_projection_matrix;
    arc::lx::Matrix44f m_view_matrix;

    // TODO: REMOVE ME
    arc::lx::Vector3f m_rotation;
};

} // namespace omi_death

#endif
