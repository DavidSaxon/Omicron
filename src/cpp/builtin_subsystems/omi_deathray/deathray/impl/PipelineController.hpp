/*!
 * \file
 * \author David Saxon
 * \date 2017
 * \copyright Copyright (C) The Arcane Initiative - All Rights Reserved
 *            Unauthorized copying of this file, via any medium is strictly
 *            prohibited
 */
#ifndef DEATHRAY_IMPL_PIPELINECONTROLLER_HPP_
#define DEATHRAY_IMPL_PIPELINECONTROLLER_HPP_

#include <arcanecore/base/lang/Restrictors.hpp>

#include "deathray/impl/Scene.hpp"


namespace death
{

// TODO: DOC
class PipelineController
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
public:

    //--------------------------------------------------------------------------
    //                          PUBLIC STATIC FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns the singleton instance of the DeathRay PipelineController.
     */
    static PipelineController& instance();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Begins execution of the DeathRay render pipeline for the given
     *        scene.
     *
     * \return The error state after executing the render pipeline.
     */
    DeathError execute(death::Scene* scene);

private:

    //--------------------------------------------------------------------------
    //                            PRIVATE CONSTRUCTOR
    //--------------------------------------------------------------------------

    PipelineController();

    //--------------------------------------------------------------------------
    //                             PRIVATE DESTRUCTOR
    //--------------------------------------------------------------------------

    ~PipelineController();

    //--------------------------------------------------------------------------
    //                            COMPILATION FIREWALL
    //--------------------------------------------------------------------------

    class PipelineControllerImpl;
    PipelineControllerImpl* m_impl;
};

} // namespace death

#endif
