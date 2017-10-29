/*!
 * \file
 * \author David Saxon
 */
#ifndef DEATHRAY_RENDERER_HPP_
#define DEATHRAY_RENDERER_HPP_

#include <arcanecore/base/lang/Restrictors.hpp>

#include "deathray/DeathRayAPI.hpp"


namespace death
{

class Geometry;

// TODO: DOC
class Renderer
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
public:

    //--------------------------------------------------------------------------
    //                          PUBLIC STATIC FUNCTIONS
    //--------------------------------------------------------------------------

    DEATH_API_EXPORT static Renderer& instance();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    DEATH_API_EXPORT void setup();

    DEATH_API_EXPORT void render();

    DEATH_API_EXPORT void add_geometry(Geometry* geo);

private:

    //--------------------------------------------------------------------------
    //                            PRIVATE CONSTRUCTOR
    //--------------------------------------------------------------------------

    Renderer();

    //--------------------------------------------------------------------------
    //                             PRIVATE DESTRUCTOR
    //--------------------------------------------------------------------------

    virtual ~Renderer();

    //--------------------------------------------------------------------------
    //                            COMPILATION FIREWALL
    //--------------------------------------------------------------------------

    class RendererImpl;
    RendererImpl* m_impl;
};

} // namespace death

#endif
