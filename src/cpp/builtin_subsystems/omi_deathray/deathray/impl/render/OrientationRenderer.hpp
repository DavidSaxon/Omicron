/*!
 * \file
 * \author David Saxon
 * \date 2017
 * \copyright Copyright (C) The Arcane Initiative - All Rights Reserved
 *            Unauthorized copying of this file, via any medium is strictly
 *            prohibited
 */
#ifndef DEATHRAY_IMPL_RENDER_ORIENTATIONRENDERER_HPP_
#define DEATHRAY_IMPL_RENDER_ORIENTATIONRENDERER_HPP_

#include <arcanecore/base/lang/Restrictors.hpp>

#include "deathray/api/API.h"


namespace death
{

//------------------------------------------------------------------------------
//                              FORWARD DECLARATIONS
//------------------------------------------------------------------------------

class Scene;

/*!
 * \brief Singleton object used to render a debug orientation widget to the
 *        viewport.
 */
class OrientationRenderer
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
public:

    //--------------------------------------------------------------------------
    //                          PUBLIC STATIC FUNCTIONS
    //--------------------------------------------------------------------------

    static OrientationRenderer& instance();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Renders bounds for the given scene.
     */
    void render(death::Scene* scene);

private:

    //--------------------------------------------------------------------------
    //                            PRIVATE CONSTRUCTOR
    //--------------------------------------------------------------------------

    OrientationRenderer();

    //--------------------------------------------------------------------------
    //                             PRIVATE DESTRUCTOR
    //--------------------------------------------------------------------------

    ~OrientationRenderer();

    //--------------------------------------------------------------------------
    //                            COMPILATION FIREWALL
    //--------------------------------------------------------------------------

    class OrientationRendererImpl;
    OrientationRendererImpl* m_impl;
};

} // namespace death

#endif
