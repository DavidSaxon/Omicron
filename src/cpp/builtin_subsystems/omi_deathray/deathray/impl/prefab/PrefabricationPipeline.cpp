/*!
 * \file
 * \author David Saxon
 * \date 2017
 * \copyright Copyright (C) The Arcane Initiative - All Rights Reserved
 *            Unauthorized copying of this file, via any medium is strictly
 *            prohibited
 */
#include "deathray/impl/prefab/PrefabricationPipeline.hpp"

#include <arcanecore/thread/lock/UniqueMutex.hpp>

#include "deathray/impl/Globals.hpp"
#include "deathray/impl/Scene.hpp"


namespace death
{

//------------------------------------------------------------------------------
//                                 IMPLEMENTATION
//------------------------------------------------------------------------------

class PrefabricationPipeline::PrefabricationPipelineImpl
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

    PrefabricationPipelineImpl()
    {
    }

    //---------------------------D E S T R U C T O R----------------------------

    ~PrefabricationPipelineImpl()
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
        // TODO: this could be threaded if threading is enabled
        // iterate over all spatial entities in the scene and prefabricate them
        for(death::Spatial* spatial : scene->get_spatials())
        {
            build_octtree(spatial, scene);
        }

        return kDeathSuccess;
    }

private:

    //------------P R I V A T E    M E M B E R    F U N C T I O N S-------------

    // Checks whether the scene has an out-of-date octree for the given spatial
    // entity and if so builds a new octree into the scene for the entity
    void build_octtree(death::Spatial* spatial, death::Scene* scene)
    {
        // get or create an Octree for this spatial
        death::Octree* octree = scene->get_or_create_octree(spatial);

        // TODO: need perform updates if things have gone out of date
    }
};

//------------------------------------------------------------------------------
//                            PUBLIC STATIC FUNCTIONS
//------------------------------------------------------------------------------

PrefabricationPipeline& PrefabricationPipeline::instance()
{
    static PrefabricationPipeline inst;
    return inst;
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

void PrefabricationPipeline::lock()
{
    m_impl->lock();
}

void PrefabricationPipeline::unlock()
{
    m_impl->unlock();
}

DeathError PrefabricationPipeline::execute(death::Scene* scene)
{
    return m_impl->execute(scene);
}

//------------------------------------------------------------------------------
//                              PRIVATE CONSTRUCTOR
//------------------------------------------------------------------------------

PrefabricationPipeline::PrefabricationPipeline()
    : m_impl(new PrefabricationPipelineImpl())
{
}

//------------------------------------------------------------------------------
//                               PRIVATE DESTRUCTOR
//------------------------------------------------------------------------------

PrefabricationPipeline::~PrefabricationPipeline()
{
    delete m_impl;
}

} // namespace death
