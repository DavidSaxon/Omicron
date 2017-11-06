/*!
 * \file
 * \author David Saxon
 * \date 2017
 * \copyright Copyright (C) The Arcane Initiative - All Rights Reserved
 *            Unauthorized copying of this file, via any medium is strictly
 *            prohibited
 */
#ifndef DEATHRAY_IMPL_GEOMETRIC_HPP_
#define DEATHRAY_IMPL_GEOMETRIC_HPP_

#include <arcanecore/base/lang/Restrictors.hpp>

#include "deathray/api/API.h"
#include "deathray/api/Geometric.h"


namespace death
{

//------------------------------------------------------------------------------
//                              FORWARD DECLARATIONS
//------------------------------------------------------------------------------

class GLGeometry;
class VBO;


// TODO: DOC
class Geometric
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTOR
    //--------------------------------------------------------------------------

    // TODO: DOC
    Geometric(DeathGeometricHandle handle);

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    virtual ~Geometric();

    //--------------------------------------------------------------------------
    //                          PUBLIC: MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns the VBO for the given index (or returns null of this
     *        geometric object doesn't have a VBO at the index).
     */
    death::VBO* get_vbo(DeathUInt32 index);

    /*!
     * \brief Implementation of the death_geo_attach_vbo function.
     */
    DeathError attach_vbo(DeathUInt32 index, death::VBO* vbo);

    /*!
     * \brief Implementation of the death_geo_remove_vbo function.
     */
    DeathError remove_vbo(death::VBO* vbo);

    /*!
     * \brief Returns the debug GLGeometry for this geometric.
     *
     * \note If the GLGeometry doesn't exist (or is out-of-date) it is created
     *       by this function. However if this geometric has no valid data, null
     *       is returned.
     */
    death::GLGeometry* get_debug_geo();

private:

    //--------------------------------------------------------------------------
    //                            COMPILATION FIREWALL
    //--------------------------------------------------------------------------

    class GeometricImpl;
    GeometricImpl* m_impl;

    //--------------------------------------------------------------------------
    //                             PRIVATE ATTRIBUTES
    //--------------------------------------------------------------------------

    DeathGeometricHandle m_handle;
};

} // namespace death

//------------------------------------------------------------------------------
//                             API STRUCT DEFINITION
//------------------------------------------------------------------------------

struct DeathGeometric
{
    death::Geometric* impl;
    DeathGeometric() : impl(nullptr) {};
};

#endif
