/*!
 * \file
 * \author David Saxon
 * \date 2017
 * \copyright Copyright (C) The Arcane Initiative - All Rights Reserved
 *            Unauthorized copying of this file, via any medium is strictly
 *            prohibited
 */
#ifndef DEATHRAY_IMPL_DATA_PACKEDOCTREE_HPP_
#define DEATHRAY_IMPL_DATA_PACKEDOCTREE_HPP_

#include <arcanecore/base/lang/Restrictors.hpp>

#include <arcanecore/lx/Vector.hpp>

#include "deathray/api/API.h"


namespace death
{

//------------------------------------------------------------------------------
//                              FORWARD DECLARATIONS
//------------------------------------------------------------------------------

class Octree;

// TODO: DOC WITH layout
class PackedOctree
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTOR
    //--------------------------------------------------------------------------

    PackedOctree(death::Octree* octree);

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    virtual ~PackedOctree();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns the stride of a buffer "pixel".
     */
    std::size_t get_data_stride() const;

    /*!
     * \brief Returns the width/height of the square 2d buffer containing the
     *        Octree data.
     */
    std::size_t get_data_width() const;

    /*!
     * \brief Returns the raw data of the packed octree.
     */
    const DeathFloat* get_data() const;

    /*!
     * \brief Gets the float data associated with the given index in the data.
     */
    DeathFloat get_value(std::size_t index) const;

    /*!
     * \brief Gets the float data associated with the given x and y indices in
     *        the data.
     */
    DeathFloat get_value(std::size_t x, std::size_t y) const;

    /*!
     * \brief Gets the vector3 data associated with the given index in the data.
     */
    const arc::lx::Vector3f* get_value_3(std::size_t index) const;

    /*!
     * \brief Gets the float vector3 data associated with the given x and y
     *        indices in the data.
     */
    const arc::lx::Vector3f* get_value_3(std::size_t x, std::size_t y) const;

    /*!
     * \brief Returns the root size of this octree.
     */
    DeathFloat get_root_size() const;

    // TOOD: get min size?

    /*!
     * \brief Returns the position (including offset) of the octree.
     */
    arc::lx::Vector3f get_position() const;

    /*!
     * \brief Returns the rotation of the octree.
     */
    arc::lx::Vector3f get_rotation() const;

    /*!
     * \brief Returns the scale of the octree.
     */
    arc::lx::Vector3f get_scale() const;

private:

    //--------------------------------------------------------------------------
    //                            COMPILATION FIREWALL
    //--------------------------------------------------------------------------

    class PackedOctreeImpl;
    PackedOctreeImpl* m_impl;
};

} // namespace death

#endif
