/*!
 * \file
 * \author David Saxon
 * \date 2017
 * \copyright Copyright (C) The Arcane Initiative - All Rights Reserved
 *            Unauthorized copying of this file, via any medium is strictly
 *            prohibited
 */
#ifndef DEATHRAY_IMPL_RENDER_VIEW_HPP_
#define DEATHRAY_IMPL_RENDER_VIEW_HPP_

#include <arcanecore/base/lang/Restrictors.hpp>
#include <arcanecore/lx/AABB.hpp>
#include <arcanecore/lx/Vector.hpp>

#include "deathray/api/API.h"


namespace death
{

//------------------------------------------------------------------------------
//                              FORWARD DECLARATIONS
//------------------------------------------------------------------------------

class Scene;


/*!
 * \brief Simple data structure that stores information about a rendered view.
 */
class View
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
public:

    //--------------------------------------------------------------------------
    //                                   ENUMS
    //--------------------------------------------------------------------------

    /*!
     * \brief Used to define the render modes a view will be using.
     */
    enum RenderMode
    {
        kRenderModeNone          = 0UL,
        kRenderModeGPUPathTracer = 1UL << 0,
        kRenderModeCPUPathTracer = 1UL << 1,
        kRenderModeBound         = 1UL << 2,
        kRenderModeGeometric     = 1UL << 3,
        kRenderModeOctree        = 1UL << 4,
        kRenderModeCell          = 1UL << 5,
        kRenderModeRay           = 1UL << 6
    };

    //--------------------------------------------------------------------------
    //                                CONSTRUCTOR
    //--------------------------------------------------------------------------

    /*!
     * \brief Constructs a new view.
     *
     * \param viewport The normalised screen co-ordinates (0.0 -> 1.0) that this
     *                 view will be rendering in screenspace.
     * \param render_mode The render modes this viewport will be using.
     * \param background_colour Defines the background colour to use for this
     *                          viewport.
     */
    View(
            const arc::lx::AABB2f& viewport,
            RenderMode render_modes,
            const arc::lx::Vector3f background_colour =
                arc::lx::Vector3f(0.0F, 0.0F, 0.0F));

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    virtual ~View();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns the normalised screen co-ordinates (0.0 -> 1.0) of this
     *        view.
     */
    const arc::lx::AABB2f& get_viewport() const;

    /*!
     * \brief Sets the normalised screen-cordinates (0.0 -> 1.0) of this view.
     */
    void set_viewport(const arc::lx::AABB2f& viewport);

    /*!
     * \brief Returns the render modes of this view.
     */
    RenderMode get_render_modes() const;

    /*!
     * \brief Sets the render modes of this view.
     */
    void set_render_modes(RenderMode render_modes);

    /*!
     * \brief Returns the background colour of this viewport.
     */
    const arc::lx::Vector3f& get_background_colour() const;

    /*!
     * \brief Sets the background colour to use for this viewport.
     */
    void set_background_colour(const arc::lx::Vector3f& colour);

    /*!
     * \brief Renders this view for the given scene.
     */
    void render(death::Scene* scene);

private:

    //--------------------------------------------------------------------------
    //                            COMPILATION FIREWALL
    //--------------------------------------------------------------------------

    class ViewImpl;
    ViewImpl* m_impl;
};

} // namespace death

#endif
