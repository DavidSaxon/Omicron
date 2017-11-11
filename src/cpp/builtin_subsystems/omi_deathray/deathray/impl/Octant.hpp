/*!
 * \file
 * \author David Saxon
 * \date 2017
 * \copyright Copyright (C) The Arcane Initiative - All Rights Reserved
 *            Unauthorized copying of this file, via any medium is strictly
 *            prohibited
 */
#ifndef DEATHRAY_IMPL_OCTANT_HPP_
#define DEATHRAY_IMPL_OCTANT_HPP_

#include <arcanecore/base/Types.hpp>
#include <arcanecore/base/lang/Restrictors.hpp>

#include "deathray/api/API.h"


namespace death
{

// TODO: DOC
class Octant
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
public:

    //--------------------------------------------------------------------------
    //                                  STRUCTS
    //--------------------------------------------------------------------------

    // TODO: DOC
    union Id
    {
        arc::uint8 bytes[3];
        arc::uint32 integer;
    };

    //--------------------------------------------------------------------------
    //                                CONSTRUCTOR
    //--------------------------------------------------------------------------

    // TODO: DOC
    Octant(DeathFloat size);

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    virtual ~Octant();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns the size of this Octant.
     */
    float get_size() const;

    /*!
     * \brief Returns the id of this Octant.
     */
    Id get_id() const;

    // TODO: make more descriptive (solid vs volumetric - shading - triangle
    //       information)
    bool has_data() const;

    // TODO:
    void set_data();

    // TODO: DOC
    bool is_leaf() const;

    // TODO: DOC
    void branch(DeathSize depth);

    // TODO: DOC
    Octant* get_child(bool x, bool y, bool z);

private:

    //--------------------------------------------------------------------------
    //                            COMPILATION FIREWALL
    //--------------------------------------------------------------------------

    class OctantImpl;
    OctantImpl* m_impl;
};

} // namespace death

#endif
