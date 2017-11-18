/*!
 * \file
 * \author David Saxon
 * \date 2017
 * \copyright Copyright (C) The Arcane Initiative - All Rights Reserved
 *            Unauthorized copying of this file, via any medium is strictly
 *            prohibited
 */
#include "deathray/impl/PipelineController.hpp"

#include <GL/glew.h>

#include "deathray/impl/Globals.hpp"
#include "deathray/impl/prefab/PrefabricationPipeline.hpp"
#include "deathray/impl/render/RenderPipeline.hpp"


namespace death
{

//------------------------------------------------------------------------------
//                                 IMPLEMENTATION
//------------------------------------------------------------------------------

class PipelineController::PipelineControllerImpl
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
private:

    //-------------------P R I V A T E    A T T R I B U T E S-------------------

    // TODO:

public:

    //--------------------------C O N S T R U C T O R---------------------------

    PipelineControllerImpl()
    {
    }

    //---------------------------D E S T R U C T O R----------------------------

    ~PipelineControllerImpl()
    {
    }

    //-------------P U B L I C    M E M B E R    F U N C T I O N S--------------

    DeathError execute(death::Scene* scene)
    {
        // begin with the prefab pipeline
        PrefabricationPipeline::instance().lock();
        PrefabricationPipeline::instance().execute(scene);
        PrefabricationPipeline::instance().unlock();

        //---------------------------GL CONTEXT BEGIN---------------------------

        // setup glew once
        static bool setup_once = true;
        if(setup_once)
        {
            GLenum glew_error = glewInit();
            if(glew_error != GLEW_OK)
            {
                // TODO: should set an error here
                DEATH_LOG_CRITICAL(
                    "Error initialising GLEW: " <<
                    glewGetErrorString(glew_error)
                );
                return kDeathErrorInvalidGraphicsState;
            }
            setup_once = false;
        }

        // execute the render pipeline
        RenderPipeline::instance().lock();
        RenderPipeline::instance().execute(scene);
        RenderPipeline::instance().unlock();

        //----------------------------GL CONTEXT END----------------------------

        // TODO
        return kDeathSuccess;
    }
};

//------------------------------------------------------------------------------
//                            PUBLIC STATIC FUNCTIONS
//------------------------------------------------------------------------------

PipelineController& PipelineController::instance()
{
    static PipelineController inst;
    return inst;
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

DeathError PipelineController::execute(death::Scene* scene)
{
    return m_impl->execute(scene);
}

//------------------------------------------------------------------------------
//                              PRIVATE CONSTRUCTOR
//------------------------------------------------------------------------------

PipelineController::PipelineController()
    : m_impl(new PipelineControllerImpl())
{
}

//------------------------------------------------------------------------------
//                               PRIVATE DESTRUCTOR
//------------------------------------------------------------------------------

PipelineController::~PipelineController()
{
    delete m_impl;
}

} // namespace death
