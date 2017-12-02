/*!
 * \file
 * \author David Saxon
 * \date 2017
 * \copyright Copyright (C) The Arcane Initiative - All Rights Reserved
 *            Unauthorized copying of this file, via any medium is strictly
 *            prohibited
 */
#include "deathray/impl/pathtracer/cpu/CPURayHierarchy.hpp"


namespace death
{
namespace cpu
{

//------------------------------------------------------------------------------
//                                  CONSTRUCTORS
//------------------------------------------------------------------------------

RayHierarchy::RayHierarchy()
    : start(0.0F, 0.0F, 0.0F)
    , end  (0.0F, 0.0F, 0.0F)
{
}

RayHierarchy::RayHierarchy(
        const arc::lx::Vector3f& start_,
        arc::lx::Vector3f& end_)
    : start(start_)
    , end  (end_)
{
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

RayHierarchy::~RayHierarchy()
{
    // delete children
    for(RayHierarchy* child : children)
    {
        delete child;
    }
    children.clear();
}

} // namespace cpu
} // namespace death
