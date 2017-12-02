/*!
 * \file
 * \author David Saxon
 * \date 2017
 * \copyright Copyright (C) The Arcane Initiative - All Rights Reserved
 *            Unauthorized copying of this file, via any medium is strictly
 *            prohibited
 */
#ifndef DEATHRAY_IMPL_ACCELERATION_GPUOCTREE_HPP_
#define DEATHRAY_IMPL_ACCELERATION_GPUOCTREE_HPP_

#include <arcanecore/base/lang/Restrictors.hpp>

#include "deathray/gl/Texture2D.hpp"

#include "deathray/api/API.h"


namespace death
{

//------------------------------------------------------------------------------
//                              FORWARD DECLARATIONS
//------------------------------------------------------------------------------

class Octree;


// TODO: DOC
// TODO: DOC the layout
/*!
 * Loads an Octree into GPU Texture.
 *
 * The layout of the texture is as follows:
 *
 *                                -- header --
 *                          [position (Vector3f)]
 *                                     v
 *                    0xFFFFFFFF | 0xFFFFFFFF | 0xFFFFFFFF
 *
 *             [octree size (float)] [min leaf size (float)]
 *                        v                     v
 *                   0xFFFFFFFF     |      0XFFFFFFFF
 *
 *                     -- octant --
 *  [-x -y -z child address] [ x -y -z child address]
 *             v                        v
 *        0xFFFFFFFF        |      0xFFFFFFFF
 *  [-x  y -z child address] [ x  y -z child address]
 *             v                        v
 *        0xFFFFFFFF        |      0xFFFFFFFF
 *  [-x -y  z child address] [ x -y  z child address]
 *             v                        v
 *        0xFFFFFFFF        |      0xFFFFFFFF
 *  [-x  y  z child address] [ x  y  z child address]
 *             v                        v
 *        0xFFFFFFFF        |      0xFFFFFFFF
 *
 *                  -- cell --
 * TODO: shading information can be skipped if it's the same shader for the
 *       whole octree.
 * [shading address] [TODO: surface information]
 *          v
 *     0xFFFFFFFF
 */
class GPUOctree
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTOR
    //--------------------------------------------------------------------------

    GPUOctree(death::Octree* octree);

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    virtual ~GPUOctree();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns the width of the 2D buffer containing the GPU data.
     */
    std::size_t get_buffer_width() const;

    /*!
     * \brief Returns the 2d texture buffer containing the GPU data.
     */
    death::gl::Texture2D& get_gpu_buffer();

private:

    //--------------------------------------------------------------------------
    //                            COMPILATION FIREWALL
    //--------------------------------------------------------------------------

    class GPUOctreeImpl;
    GPUOctreeImpl* m_impl;
};

} // namespace death

#endif
