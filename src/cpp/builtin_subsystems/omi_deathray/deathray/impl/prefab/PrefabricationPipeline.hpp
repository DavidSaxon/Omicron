/*!
 * \file
 * \author David Saxon
 * \date 2017
 * \copyright Copyright (C) The Arcane Initiative - All Rights Reserved
 *            Unauthorized copying of this file, via any medium is strictly
 *            prohibited
 */
#ifndef DEATHRAY_IMPL_PREFAB_PREFABRICATIONPIPELINE_HPP_
#define DEATHRAY_IMPL_PREFAB_PREFABRICATIONPIPELINE_HPP_

#include <arcanecore/base/lang/Restrictors.hpp>

#include "deathray/api/API.h"


namespace death
{

//------------------------------------------------------------------------------
//                              FORWARD DECLARATIONS
//------------------------------------------------------------------------------

class Scene;


// TODO: DOC
//       note this section of the pipeline does no use the graphics state
class PrefabricationPipeline
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
public:

    //--------------------------------------------------------------------------
    //                          PUBLIC STATIC FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns the singleton instance of the DeathRay
     *        PrefabricationPipeline.
     */
    static PrefabricationPipeline& instance();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Acquires a unique lock on the PrefabricationPipeline.
     */
    void lock();

    /*!
     * \brief Releases the unique lock.
     */
    void unlock();

    /*!
     * \brief Begins execution of the DeathRay PrefabricationPipeline for the
     *        given scene.
     *
     * \return The error state after executing the PrefabricationPipeline.
     */
    DeathError execute(death::Scene* scene);

private:

    //--------------------------------------------------------------------------
    //                            PRIVATE CONSTRUCTOR
    //--------------------------------------------------------------------------

    PrefabricationPipeline();

    //--------------------------------------------------------------------------
    //                             PRIVATE DESTRUCTOR
    //--------------------------------------------------------------------------

    ~PrefabricationPipeline();

    //--------------------------------------------------------------------------
    //                            COMPILATION FIREWALL
    //--------------------------------------------------------------------------

    class PrefabricationPipelineImpl;
    PrefabricationPipelineImpl* m_impl;
};

} // namespace death

#endif
