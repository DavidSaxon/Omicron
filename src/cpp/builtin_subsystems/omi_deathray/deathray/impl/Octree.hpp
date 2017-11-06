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


namespace death
{

//------------------------------------------------------------------------------
//                              FORWARD DECLARATIONS
//------------------------------------------------------------------------------

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

private:

    //--------------------------------------------------------------------------
    //                            COMPILATION FIREWALL
    //--------------------------------------------------------------------------

    class OctreeImpl;
    OctreeImpl* m_impl;
};

} // namespace death

#endif
