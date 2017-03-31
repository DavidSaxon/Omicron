/*!
 * \file
 * \author David Saxon
 */
#ifndef OSS_RENDERER_HPP_
#define OSS_RENDERER_HPP_

#include "oss/Subsystem.hpp"

namespace oss
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

    // TODO:
};

} // namespace oss

#endif
