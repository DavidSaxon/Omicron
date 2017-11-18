/*!
 * \file
 * \author David Saxon
 * \date 2017
 * \copyright Copyright (C) The Arcane Initiative - All Rights Reserved
 *            Unauthorized copying of this file, via any medium is strictly
 *            prohibited
 */
#include "deathray/impl/Scene.hpp"

#include <list>
#include <new>
#include <unordered_map>

#include <arcanecore/lx/AABB.hpp>

#include "deathray/impl/Camera.hpp"
#include "deathray/impl/Globals.hpp"
#include "deathray/impl/Octree.hpp"
#include "deathray/impl/Spatial.hpp"
#include "deathray/impl/PipelineController.hpp"
#include "deathray/impl/render/View.hpp"


namespace death
{

//------------------------------------------------------------------------------
//                                 IMPLEMENTATION
//------------------------------------------------------------------------------

class Scene::SceneImpl
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
private:

    //-------------------P R I V A T E    A T T R I B U T E S-------------------

    // The Scene interface this implementation is for
    Scene* m_self;
    // the API handle to the scene
    DeathSceneHandle m_handle;

    // the resolution to render this scene to
    arc::lx::Vector2u m_resolution;

    // the explicitly set camera to be used by this view
    death::Camera* m_camera;
    // the default camera that will be used by this scene
    death::Camera m_default_camera;

    // the set of spatial entities within the scene
    std::unordered_set<death::Spatial*> m_spatials;

    // mapping from spatials to their associated Octree objects.
    std::unordered_map<death::Spatial*, death::Octree*> m_octrees;
    // queue of octree objects that need to be destroyed once we have a valid
    // graphics state
    std::list<death::Octree*> m_octree_cleanup;

    // the views being rendered for this scene
    std::vector<death::View*> m_views;

public:

    //--------------------------C O N S T R U C T O R---------------------------

    SceneImpl(Scene* self, DeathSceneHandle handle)
        : m_self          (self)
        , m_handle        (handle)
        , m_camera        (nullptr)
        , m_default_camera(nullptr)
    {
        // ensure logging is setup
        death::global::setup_logging();

        // set up views
        // m_views.push_back(new death::View(
        //     arc::lx::AABB2f(
        //         arc::lx::Vector2f(0.0F, 0.0F),
        //         arc::lx::Vector2f(1.0F, 1.0F)
        //     ),
        //     death::View::kRenderModePathTracer,
        //     arc::lx::Vector3f(0.0F, 0.0F, 0.0F)
        // ));

        // m_views.reserve(2);
        // m_views.push_back(new death::View(
        //     arc::lx::AABB2f(
        //         arc::lx::Vector2f(0.0F, 0.0F),
        //         arc::lx::Vector2f(1.0F, 1.0F)
        //     ),
        //     static_cast<death::View::RenderMode>(
        //         death::View::kRenderModeBound |
        //         death::View::kRenderModeGeometric
        //     ),
        //     arc::lx::Vector3f(0.2F, 0.2F, 0.2F)
        // ));
        // m_views.push_back(new death::View(
        //     arc::lx::AABB2f(
        //         arc::lx::Vector2f(0.5F, 0.0F),
        //         arc::lx::Vector2f(1.0F, 0.5F)
        //     ),
        //     death::View::kRenderModeGeometric,
        //     arc::lx::Vector3f(0.0F, 0.0F, 0.0F)
        // ));

        m_views.reserve(4);
        m_views.push_back(new death::View(
            arc::lx::AABB2f(
                arc::lx::Vector2f(0.0F, 0.5F),
                arc::lx::Vector2f(0.5F, 1.0F)
            ),
            // arc::lx::AABB2f(
            //     arc::lx::Vector2f(0.0F, 0.0F),
            //     arc::lx::Vector2f(1.0F, 1.0F)
            // ),
            death::View::kRenderModePathTracer,
            arc::lx::Vector3f(0.0F, 0.0F, 0.0F)
        ));
        m_views.push_back(new death::View(
            arc::lx::AABB2f(
                arc::lx::Vector2f(0.5F, 0.5F),
                arc::lx::Vector2f(1.0F, 1.0F)
            ),
            static_cast<death::View::RenderMode>(
                death::View::kRenderModeGeometric |
                death::View::kRenderModeBound
            ),
            arc::lx::Vector3f(0.1F, 0.1F, 0.1F)
        ));
        m_views.push_back(new death::View(
            arc::lx::AABB2f(
                arc::lx::Vector2f(0.0F, 0.0F),
                arc::lx::Vector2f(0.5F, 0.5F)
            ),
            static_cast<death::View::RenderMode>(
                death::View::kRenderModeGeometric |
                death::View::kRenderModeOctree
            ),
            arc::lx::Vector3f(0.85F, 0.85F, 0.85F)
        ));
        m_views.push_back(new death::View(
            arc::lx::AABB2f(
                arc::lx::Vector2f(0.5F, 0.0F),
                arc::lx::Vector2f(1.0F, 0.5F)
            ),
            static_cast<death::View::RenderMode>(
                death::View::kRenderModeCell |
                death::View::kRenderModeOctree
            ),
            arc::lx::Vector3f(0.6F, 0.6F, 0.6F)
        ));
    }

    //---------------------------D E S T R U C T O R----------------------------

    ~SceneImpl()
    {
        graphics_state_cleanup();

        // clean up octrees
        for(auto& octree : m_octrees)
        {
            delete octree.second;
        }
        m_octrees.clear();

        // clean up views
        for(death::View* view : m_views)
        {
            delete view;
        }
        m_views.clear();
    }

    //-------------P U B L I C    M E M B E R    F U N C T I O N S--------------

    const arc::lx::Vector2u& get_resolution() const
    {
        return m_resolution;
    }

    DeathError set_resolution(const arc::lx::Vector2u& resolution)
    {
        m_resolution = resolution;
        return kDeathSuccess;
    }

    death::Camera* get_camera()
    {
        if(m_camera != nullptr)
        {
            return m_camera;
        }
        return &m_default_camera;
    }

    DeathError set_camera(death::Camera* camera)
    {
        m_camera = camera;
        return kDeathSuccess;
    }

    const std::unordered_set<death::Spatial*> get_spatials() const
    {
        return m_spatials;
    }

    DeathError add_spatial(death::Spatial* spatial)
    {
        if(m_spatials.find(spatial) != m_spatials.end())
        {
            return kDeathErrorNoOperation;
        }

        m_spatials.insert(spatial);
        return kDeathSuccess;
    }

    DeathError remove_spatial(death::Spatial* spatial)
    {
        // is there an octree related to the spatial?
        auto f_octree = m_octrees.find(spatial);
        if(f_octree != m_octrees.end())
        {
            // queue this for clean up once we have a graphics state
            m_octree_cleanup.push_back(f_octree->second);
            m_octrees.erase(f_octree);
        }

        auto f_spatial = m_spatials.find(spatial);
        if(f_spatial == m_spatials.end())
        {
            return kDeathErrorNoOperation;
        }

        m_spatials.erase(f_spatial);
        return kDeathSuccess;
    }

    death::Octree* get_or_create_octree(death::Spatial* spatial)
    {
        auto f_octree = m_octrees.find(spatial);
        if(f_octree != m_octrees.end())
        {
            return f_octree->second;
        }

        // construct a new octree for this spatial
        death::Octree* new_octree = new death::Octree(spatial);
        m_octrees.insert(std::make_pair(spatial, new_octree));
        return new_octree;
    }

    const std::vector<death::View*>& get_views() const
    {
        return m_views;
    }

    DeathError render()
    {
        // execute the pipeline
        return death::PipelineController::instance().execute(m_self);
    }

    void graphics_state_cleanup()
    {
        for(death::Octree* octree : m_octree_cleanup)
        {
            delete octree;
        }
        m_octree_cleanup.clear();
    }
};

//------------------------------------------------------------------------------
//                                  CONSTRUCTOR
//------------------------------------------------------------------------------

Scene::Scene(DeathSceneHandle handle)
    : m_impl(new SceneImpl(this, handle))
{
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

Scene::~Scene()
{
    delete m_impl;
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

const arc::lx::Vector2u& Scene::get_resolution() const
{
    return m_impl->get_resolution();
}

DeathError Scene::set_resolution(const arc::lx::Vector2u& resolution)
{
    return m_impl->set_resolution(resolution);
}

death::Camera* Scene::get_camera()
{
    return m_impl->get_camera();
}

DeathError Scene::set_camera(death::Camera* camera)
{
    return m_impl->set_camera(camera);
}

const std::unordered_set<death::Spatial*> Scene::get_spatials() const
{
    return m_impl->get_spatials();
}

DeathError Scene::add_spatial(death::Spatial* spatial)
{
    return m_impl->add_spatial(spatial);
}

DeathError Scene::remove_spatial(death::Spatial* spatial)
{
    return m_impl->remove_spatial(spatial);
}

death::Octree* Scene::get_or_create_octree(death::Spatial* spatial)
{
    return m_impl->get_or_create_octree(spatial);
}

const std::vector<death::View*>& Scene::get_views() const
{
    return m_impl->get_views();
}

DeathError Scene::render()
{
    return m_impl->render();
}

void Scene::graphics_state_cleanup()
{
    m_impl->graphics_state_cleanup();
}

} // namespace death

//------------------------------------------------------------------------------
//                               API IMPLEMENTATION
//------------------------------------------------------------------------------

DEATH_API_EXPORT DeathError death_scene_gen(DeathSceneHandle* scene)
{
    DeathScene* handle = nullptr;
    try
    {
        handle = new DeathScene();
        handle->impl = new death::Scene(handle);
    }
    catch(std::bad_alloc&)
    {
        if(handle != nullptr)
        {
            delete handle;
        }
        return kDeathErrorOutOfMemory;
    }

    *scene = handle;
    return kDeathSuccess;
}

DEATH_API_EXPORT DeathError death_scene_delete(DeathSceneHandle* scene)
{
    if(scene == nullptr)
    {
        return kDeathErrorNullHandle;
    }
    if(*scene == nullptr)
    {
        return kDeathErrorNullHandle;
    }

     delete (*scene)->impl;
    delete *scene;
    *scene = nullptr;
    return kDeathSuccess;
}

DEATH_API_EXPORT DeathError death_scene_set_resolution(
        DeathSceneHandle scene,
        DeathUInt32 width,
        DeathUInt32 height)
{
    if(scene == nullptr)
    {
        return kDeathErrorNullHandle;
    }
    return scene->impl->set_resolution(arc::lx::Vector2u(width, height));
}

DEATH_API_EXPORT DeathError death_scene_set_camera(
        DeathSceneHandle scene,
        DeathCameraHandle camera)
{
    if(scene == nullptr)
    {
        return kDeathErrorNullHandle;
    }
    return scene->impl->set_camera(camera->impl);
}

DEATH_API_EXPORT DeathError death_scene_add_spatial(
        DeathSceneHandle scene,
        DeathSpatialHandle spatial)
{
    if(scene == nullptr || spatial == nullptr)
    {
        return kDeathErrorNullHandle;
    }

    return scene->impl->add_spatial(spatial->impl);
}

DEATH_API_EXPORT DeathError death_scene_remove_spatial(
        DeathSceneHandle scene,
        DeathSpatialHandle spatial)
{
    if(scene == nullptr || spatial == nullptr)
    {
        return kDeathErrorNullHandle;
    }

    return scene->impl->remove_spatial(spatial->impl);
}

DEATH_API_EXPORT DeathError death_scene_render(DeathSceneHandle scene)
{
    if(scene == nullptr)
    {
        return kDeathErrorNullHandle;
    }

    return scene->impl->render();
}
