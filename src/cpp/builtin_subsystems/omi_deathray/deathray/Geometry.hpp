/*!
 * \file
 * \author David Saxon
 */
#ifndef DEATHRAY_GEOMETRY_HPP_
#define DEATHRAY_GEOMETRY_HPP_

#include <list>
#include <vector>

#include <arcanecore/base/lang/Restrictors.hpp>
#include <arcanecore/lx/Matrix.hpp>

#include "deathray/DeathRayAPI.hpp"


namespace death
{

class TraceOctant;

// TODO: DOC
class Geometry
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTOR
    //--------------------------------------------------------------------------

    DEATH_API_EXPORT Geometry(const std::vector<float>& faces);

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    DEATH_API_EXPORT virtual ~Geometry();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    DEATH_API_EXPORT const std::list<TraceOctant*>& get_octants() const;

    DEATH_API_EXPORT void draw_gl_bounds(const arc::lx::Matrix44f& vp_matrix);

private:

    //--------------------------------------------------------------------------
    //                            COMPILATION FIREWALL
    //--------------------------------------------------------------------------

    class GeometryImpl;
    GeometryImpl* m_impl;
};

} // namespace death

#endif
