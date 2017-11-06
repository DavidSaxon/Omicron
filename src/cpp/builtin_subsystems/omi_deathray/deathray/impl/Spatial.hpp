/*!
 * \file
 * \author David Saxon
 * \date 2017
 * \copyright Copyright (C) The Arcane Initiative - All Rights Reserved
 *            Unauthorized copying of this file, via any medium is strictly
 *            prohibited
 */
#ifndef DEATHRAY_IMPL_SPATIAL_HPP_
#define DEATHRAY_IMPL_SPATIAL_HPP_

#include <unordered_set>

#include <arcanecore/base/lang/Restrictors.hpp>

#include "deathray/api/API.h"
#include "deathray/api/Spatial.h"


namespace death
{

//------------------------------------------------------------------------------
//                              FORWARD DECLARATIONS
//------------------------------------------------------------------------------

class Geometric;


// TODO: DOC
class Spatial
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTOR
    //--------------------------------------------------------------------------

    // TODO: DOC
    Spatial(DeathSpatialHandle handle);

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    virtual ~Spatial();

    //--------------------------------------------------------------------------
    //                          PUBLIC: MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns the set of geometric objects within this spatial entity.
     */
    const std::unordered_set<death::Geometric*> get_geometrics() const;

    /*!
     * \brief Implementation of the death_spatial_attach_geo function.
     */
    DeathError attach_geo(death::Geometric* geometric);

    /*!
     * \brief Implementation of the death_spatial_remove_geo function.
     */
    DeathError remove_geo(death::Geometric* geometric);

private:

    //--------------------------------------------------------------------------
    //                            COMPILATION FIREWALL
    //--------------------------------------------------------------------------

    class SpatialImpl;
    SpatialImpl* m_impl;
};

} // namespace death

//------------------------------------------------------------------------------
//                             API STRUCT DEFINITION
//------------------------------------------------------------------------------

struct DeathSpatial
{
    death::Spatial* impl;
    DeathSpatial() : impl(nullptr) {};
};

#endif
