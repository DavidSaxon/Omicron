/*!
 * \file
 * \author David Saxon
 * \date 2017
 * \copyright Copyright (C) The Arcane Initiative - All Rights Reserved
 *            Unauthorized copying of this file, via any medium is strictly
 *            prohibited
 */
#ifndef DEATHRAY_IMPL_DEBUG_GLOCTREE_HPP_
#define DEATHRAY_IMPL_DEBUG_GLOCTREE_HPP_

#include <arcanecore/base/lang/Restrictors.hpp>

#include "deathray/api/API.h"


namespace death
{

//------------------------------------------------------------------------------
//                              FORWARD DECLARATIONS
//------------------------------------------------------------------------------

class Octant;

/*!
 * \brief OpenGL data for rendering debug octrees.
 */
class GLOctree
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTOR
    //--------------------------------------------------------------------------

    GLOctree(death::Octant* root);

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    virtual ~GLOctree();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Draws this octree.
     */
    void draw();

private:

    //--------------------------------------------------------------------------
    //                            COMPILATION FIREWALL
    //--------------------------------------------------------------------------

    class GLOctreeImpl;
    GLOctreeImpl* m_impl;
};

} // namespace death

#endif
