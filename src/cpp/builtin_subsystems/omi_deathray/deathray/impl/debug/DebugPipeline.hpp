/*!
 * \file
 * \author David Saxon
 * \date 2017
 * \copyright Copyright (C) The Arcane Initiative - All Rights Reserved
 *            Unauthorized copying of this file, via any medium is strictly
 *            prohibited
 */
#ifndef DEATHRAY_IMPL_DEBUG_DEBUGPIPELINE_HPP_
#define DEATHRAY_IMPL_DEBUG_DEBUGPIPELINE_HPP_

#include <arcanecore/base/lang/Restrictors.hpp>

#include "deathray/api/API.h"


namespace death
{

//------------------------------------------------------------------------------
//                              FORWARD DECLARATIONS
//------------------------------------------------------------------------------

class Scene;


// TODO: DOC
//       note this section of the pipeline requires a valid graphics state
class DebugPipeline
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
public:

    //--------------------------------------------------------------------------
    //                          PUBLIC STATIC FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns the singleton instance of the DeathRay DebugPipeline.
     */
    static DebugPipeline& instance();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Acquires a unique lock on the DebugPipeline.
     */
    void lock();

    /*!
     * \brief Releases the unique lock.
     */
    void unlock();

    /*!
     * \brief Begins execution of the DeathRay DebugPipeline for the given
     *        scene.
     *
     * \return The error state after executing the DebugPipeline.
     */
    DeathError execute(death::Scene* scene);

private:

    //--------------------------------------------------------------------------
    //                            PRIVATE CONSTRUCTOR
    //--------------------------------------------------------------------------

    DebugPipeline();

    //--------------------------------------------------------------------------
    //                             PRIVATE DESTRUCTOR
    //--------------------------------------------------------------------------

    ~DebugPipeline();

    //--------------------------------------------------------------------------
    //                            COMPILATION FIREWALL
    //--------------------------------------------------------------------------

    class DebugPipelineImpl;
    DebugPipelineImpl* m_impl;
};

} // namespace death

#endif
