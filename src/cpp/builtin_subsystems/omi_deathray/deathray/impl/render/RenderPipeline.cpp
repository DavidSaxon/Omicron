/*!
 * \file
 * \author David Saxon
 * \date 2017
 * \copyright Copyright (C) The Arcane Initiative - All Rights Reserved
 *            Unauthorized copying of this file, via any medium is strictly
 *            prohibited
 */
#include "deathray/impl/render/RenderPipeline.hpp"

#include <arcanecore/thread/lock/UniqueMutex.hpp>

#include <GL/glew.h>

#include "deathray/gl/ErrorState.hpp"

#include "deathray/impl/Globals.hpp"
#include "deathray/impl/Scene.hpp"
#include "deathray/impl/render/View.hpp"


namespace death
{

//------------------------------------------------------------------------------
//                                 IMPLEMENTATION
//------------------------------------------------------------------------------

class RenderPipeline::RenderPipelineImpl
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
private:

    //-------------------P R I V A T E    A T T R I B U T E S-------------------

    // the unique mutex to control access to this section of the pipeline
    arc::thread::lock::UniqueMutex m_mutex;

public:

    //--------------------------C O N S T R U C T O R---------------------------

    RenderPipelineImpl()
    {
    }

    //---------------------------D E S T R U C T O R----------------------------

    ~RenderPipelineImpl()
    {
    }

    //-------------P U B L I C    M E M B E R    F U N C T I O N S--------------

    void lock()
    {
        m_mutex.lock();
    }

    void unlock()
    {
        m_mutex.unlock();
    }

    DeathError execute(death::Scene* scene)
    {
        death::gl::error::check_state("before RenderPipeline execute");

        // let the scene clean up its graphics state objects
        scene->graphics_state_cleanup();

        // get the resolution from the scene
        const arc::lx::Vector2u resolution = scene->get_resolution();

        // set up the graphics state
        glClearColor(0.0F, 0.0F, 0.0F, 1.0F);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glViewport(0, 0, resolution(0), resolution(1));

        // clear the window to begin rendering the frame
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // render each view of the scene
        for(death::View* view : scene->get_views())
        {
            view->render(scene);
        }

        death::gl::error::check_state("after RenderPipeline execute");

        return kDeathSuccess;
    }
};

//------------------------------------------------------------------------------
//                            PUBLIC STATIC FUNCTIONS
//------------------------------------------------------------------------------

RenderPipeline& RenderPipeline::instance()
{
    static RenderPipeline inst;
    return inst;
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

void RenderPipeline::lock()
{
    m_impl->lock();
}

void RenderPipeline::unlock()
{
    m_impl->unlock();
}

DeathError RenderPipeline::execute(death::Scene* scene)
{
    return m_impl->execute(scene);
}

//------------------------------------------------------------------------------
//                              PRIVATE CONSTRUCTOR
//------------------------------------------------------------------------------

RenderPipeline::RenderPipeline()
    : m_impl(new RenderPipelineImpl())
{
}

//------------------------------------------------------------------------------
//                               PRIVATE DESTRUCTOR
//------------------------------------------------------------------------------

RenderPipeline::~RenderPipeline()
{
    delete m_impl;
}

} // namespace death
