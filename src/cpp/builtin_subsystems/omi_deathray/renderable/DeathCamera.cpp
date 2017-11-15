#include "omi_deathray/renderable/DeathCamera.hpp"

#include <omicron/api/scene/component/transform/AbstractTransform.hpp>

#include <deathray/api/Camera.h>

// TODO: REMOVE ME
#include <iostream>


namespace omi_death
{

//------------------------------------------------------------------------------
//                                 IMPLEMENTATION
//------------------------------------------------------------------------------

class DeathCamera::DeathCameraImpl
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
private:

    //-------------------P R I V A T E    A T T R I B U T E S-------------------

    // The camera component being handled
    omi::scene::Camera* m_component;
    // The DeathRay scene object
    DeathSceneHandle m_scene;

    // camera entity
    DeathCameraHandle m_camera;

    // the the last read hash of the camera component
    omi::Hash m_hash;

public:

    //--------------------------C O N S T R U C T O R---------------------------

    DeathCameraImpl(omi::scene::Camera* component, DeathSceneHandle scene)
        : m_component(component)
        , m_scene    (scene)
        , m_camera   (nullptr)
    {
        // generate the camera
        death_cam_gen(1, &m_camera);
    }

    //---------------------------D E S T R U C T O R----------------------------

    ~DeathCameraImpl()
    {
        // clean up
        death_cam_delete(1, &m_camera);
    }

    //-------------P U B L I C    M E M B E R    F U N C T I O N S--------------

    void apply()
    {
        // does the camera need updating?
        omi::Hash new_hash = m_component->get_hash();
        if(new_hash != m_hash)
        {
            m_hash = new_hash;
            death_cam_set_properties(
                m_camera,
                m_component->get_focal_length(),
                m_component->get_sensor_size()(0),
                m_component->get_sensor_size()(1),
                m_component->get_sensor_offset()(0),
                m_component->get_sensor_offset()(1)
            );
        }

        arc::lx::Matrix44f matrix = arc::lx::Matrix44f::Identity();
        const omi::scene::AbstractTransform* transform =
            m_component->get_transform();
        if(transform != nullptr)
        {
            matrix = transform->eval();
        }

        death_cam_set_transform(
            m_camera,
            &matrix(0, 0)
        );

        // attach the camera to the scene
        death_scene_set_camera(m_scene, m_camera);
    }
};

//------------------------------------------------------------------------------
//                                  CONSTRUCTOR
//------------------------------------------------------------------------------

DeathCamera::DeathCamera(omi::scene::Camera* component, DeathSceneHandle scene)
    : m_impl(new DeathCameraImpl(component, scene))
{
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

DeathCamera::~DeathCamera()
{
    delete m_impl;
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

void DeathCamera::apply()
{
    m_impl->apply();
}

} // namespace omi_death
