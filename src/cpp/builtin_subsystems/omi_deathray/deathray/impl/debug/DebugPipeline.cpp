/*!
 * \file
 * \author David Saxon
 * \date 2017
 * \copyright Copyright (C) The Arcane Initiative - All Rights Reserved
 *            Unauthorized copying of this file, via any medium is strictly
 *            prohibited
 */
#include "deathray/impl/debug/DebugPipeline.hpp"

#include <arcanecore/thread/lock/UniqueMutex.hpp>

#include "deathray/impl/Globals.hpp"
#include "deathray/impl/Geometric.hpp"
#include "deathray/impl/Scene.hpp"
#include "deathray/impl/Spatial.hpp"
#include "deathray/impl/render/View.hpp"


namespace death
{

//------------------------------------------------------------------------------
//                                 IMPLEMENTATION
//------------------------------------------------------------------------------

class DebugPipeline::DebugPipelineImpl
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

    DebugPipelineImpl()
    {
    }

    //---------------------------D E S T R U C T O R----------------------------

    ~DebugPipelineImpl()
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
        DEATH_LOG_DEBUG("Executing debug pipeline");

        return kDeathSuccess;
    }
};

//------------------------------------------------------------------------------
//                            PUBLIC STATIC FUNCTIONS
//------------------------------------------------------------------------------

DebugPipeline& DebugPipeline::instance()
{
    static DebugPipeline inst;
    return inst;
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

void DebugPipeline::lock()
{
    m_impl->lock();
}

void DebugPipeline::unlock()
{
    m_impl->unlock();
}

DeathError DebugPipeline::execute(death::Scene* scene)
{
    return m_impl->execute(scene);
}

//------------------------------------------------------------------------------
//                              PRIVATE CONSTRUCTOR
//------------------------------------------------------------------------------

DebugPipeline::DebugPipeline()
    : m_impl(new DebugPipelineImpl())
{
}

//------------------------------------------------------------------------------
//                               PRIVATE DESTRUCTOR
//------------------------------------------------------------------------------

DebugPipeline::~DebugPipeline()
{
    delete m_impl;
}

} // namespace death
