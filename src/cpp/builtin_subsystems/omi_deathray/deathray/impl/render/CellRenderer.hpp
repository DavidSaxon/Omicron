/*!
 * \file
 * \author David Saxon
 * \date 2017
 * \copyright Copyright (C) The Arcane Initiative - All Rights Reserved
 *            Unauthorized copying of this file, via any medium is strictly
 *            prohibited
 */
#ifndef DEATHRAY_IMPL_RENDER_CELLRENDERER_HPP_
#define DEATHRAY_IMPL_RENDER_CELLRENDERER_HPP_

#include <arcanecore/base/lang/Restrictors.hpp>

#include "deathray/api/API.h"


namespace death
{

//------------------------------------------------------------------------------
//                              FORWARD DECLARATIONS
//------------------------------------------------------------------------------

class Scene;

/*!
 * \brief Singleton object which is used to render debug octree cells.
 */
class CellRenderer
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
public:

    //--------------------------------------------------------------------------
    //                          PUBLIC STATIC FUNCTIONS
    //--------------------------------------------------------------------------

    static CellRenderer& instance();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Renders debug octrees for the given scene.
     */
    void render(death::Scene* scene);

private:

    //--------------------------------------------------------------------------
    //                            PRIVATE CONSTRUCTOR
    //--------------------------------------------------------------------------

    CellRenderer();

    //--------------------------------------------------------------------------
    //                             PRIVATE DESTRUCTOR
    //--------------------------------------------------------------------------

    ~CellRenderer();

    //--------------------------------------------------------------------------
    //                            COMPILATION FIREWALL
    //--------------------------------------------------------------------------

    class CellRendererImpl;
    CellRendererImpl* m_impl;
};

} // namespace death

#endif
