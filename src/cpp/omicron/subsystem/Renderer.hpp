/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_SUBSYSTEM_RENDERER_HPP_
#define OMICRON_SUBSYSTEM_RENDERER_HPP_

#include "omicron/subsystem/Subsystem.hpp"


namespace omi
{
namespace ss
{

/*!
 * \brief TODO:
 */
class Renderer : virtual public Subsystem
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTORS
    //--------------------------------------------------------------------------

    /*!
     * \brief TODO:
     */
    Renderer()
        : Subsystem()
    {
        // set renderer role
        m_roles = static_cast<Role>(m_roles | kRoleRenderer);
    }

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    virtual ~Renderer()
    {
    }

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Is called once a valid GL context has been established to setup
     *        rendering.
     */
    virtual void setup_rendering() = 0;

    /*!
     * \brief Is called to render the current frame.
     */
    virtual void render() = 0;
};

} // namespace ss
} // namespace omi

#endif
