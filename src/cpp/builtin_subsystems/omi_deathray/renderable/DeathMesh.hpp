/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_DEATHRAY_RENDERABLE_DEATHMESH_HPP_
#define OMICRON_DEATHRAY_RENDERABLE_DEATHMESH_HPP_

#include <arcanecore/base/lang/Restrictors.hpp>
#include <arcanecore/lx/Matrix.hpp>

#include <omicron/api/scene/component/renderable/Mesh.hpp>

#include <deathray/api/Scene.h>


namespace omi_death
{

/*!
 * \brief Omicron-DeathRay renderable type that handles a mesh component.
 */
class DeathMesh
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTOR
    //--------------------------------------------------------------------------

    /*!
     * \brief Creates a new handler for the given mesh component.
     */
    DeathMesh(omi::scene::Mesh* component, DeathSceneHandle scene);

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    virtual ~DeathMesh();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    // TODO:
    void render(const arc::lx::Matrix44f& vp_matrix);

private:

    //--------------------------------------------------------------------------
    //                            COMPILATION FIREWALL
    //--------------------------------------------------------------------------

    class DeathMeshImpl;
    DeathMeshImpl* m_impl;
};

} // namespace omi_death

#endif
