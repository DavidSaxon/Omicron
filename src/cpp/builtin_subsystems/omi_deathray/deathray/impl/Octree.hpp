/*!
 * \file
 * \author David Saxon
 * \date 2017
 * \copyright Copyright (C) The Arcane Initiative - All Rights Reserved
 *            Unauthorized copying of this file, via any medium is strictly
 *            prohibited
 */
#ifndef DEATHRAY_IMPL_OCTREE_HPP_
#define DEATHRAY_IMPL_OCTREE_HPP_

#include <arcanecore/base/lang/Restrictors.hpp>
#include <arcanecore/lx/AABB.hpp>
#include <arcanecore/lx/Matrix.hpp>

#include "deathray/api/API.h"


namespace death
{

//------------------------------------------------------------------------------
//                              FORWARD DECLARATIONS
//------------------------------------------------------------------------------

class GLCells;
class GLOctree;
class GPUOctree;
class Octant;
class Spatial;


// TODO: DOC
class Octree
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTOR
    //--------------------------------------------------------------------------

    Octree(death::Spatial* spatial);

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    virtual ~Octree();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns whether this Octree has no renderable data.
     */
    bool is_empty() const;

    /*!
     * \brief Returns the bounds which exactly contain the data of the spatial
     *        this Octree is associated with.
     */
    const arc::lx::AABB3f& get_true_bounds() const;

    /*!
     * \brief Returns the size of the top level of the octree.
     */
    float get_root_size() const;

    /*!
     * \brief Returns the offset matrix to use to transform the octree from
     *        world origin to model origin.
     */
    const arc::lx::Matrix44f& get_offset() const;

    /*!
     * \brief Returns the root Octant of this Octree.
     */
    death::Octant* get_root();

    /*!
     * \brief Returns the GPU data for this Octree.
     *
     * \note Of the GPUOctree doesn't exist (or is out-of-date) it is created
     *       by this function. However if this Octree has no valid data, null is
     *       returned.
     */
    death::GPUOctree* get_gpu_data();

    /*!
     * \brief Returns the debug GLOctree for this OCtree.
     *
     * \note If the GLOctree doesn't exist (or is out-of-date) it is created
     *       by this function. However if this Octree has no valid data, null is
     *       returned.
     */
    death::GLOctree* get_debug_octree();

    /*!
     * \brief Returns the debug GLCells for this OCtree.
     *
     * \note If the GLCells doesn't exist (or is out-of-date) it is created
     *       by this function. However if this Octree has no valid data, null is
     *       returned.
     */
    death::GLCells* get_debug_cells();

private:

    //--------------------------------------------------------------------------
    //                            COMPILATION FIREWALL
    //--------------------------------------------------------------------------

    class OctreeImpl;
    OctreeImpl* m_impl;
};

} // namespace death

#endif
