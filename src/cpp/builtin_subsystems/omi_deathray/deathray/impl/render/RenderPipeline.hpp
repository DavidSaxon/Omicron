/*!
 * \file
 * \author David Saxon
 * \date 2017
 * \copyright Copyright (C) The Arcane Initiative - All Rights Reserved
 *            Unauthorized copying of this file, via any medium is strictly
 *            prohibited
 */
#ifndef DEATHRAY_IMPL_RENDER_RENDERPIPELINE_HPP_
#define DEATHRAY_IMPL_RENDER_RENDERPIPELINE_HPP_

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
class RenderPipeline
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
public:

    //--------------------------------------------------------------------------
    //                          PUBLIC STATIC FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns the singleton instance of the DeathRay RenderPipeline.
     */
    static RenderPipeline& instance();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Acquires a unique lock on the RenderPipeline.
     */
    void lock();

    /*!
     * \brief Releases the unique lock.
     */
    void unlock();

    /*!
     * \brief Begins execution of the DeathRay RenderPipeline for the given
     *        scene.
     *
     * \return The error state after executing the RenderPipeline.
     */
    DeathError execute(death::Scene* scene);

private:

    //--------------------------------------------------------------------------
    //                            PRIVATE CONSTRUCTOR
    //--------------------------------------------------------------------------

    RenderPipeline();

    //--------------------------------------------------------------------------
    //                             PRIVATE DESTRUCTOR
    //--------------------------------------------------------------------------

    ~RenderPipeline();

    //--------------------------------------------------------------------------
    //                            COMPILATION FIREWALL
    //--------------------------------------------------------------------------

    class RenderPipelineImpl;
    RenderPipelineImpl* m_impl;
};

} // namespace death

#endif
