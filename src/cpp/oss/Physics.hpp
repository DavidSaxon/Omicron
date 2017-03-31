/*!
 * \file
 * \author David Saxon
 */
#ifndef OSS_PHYSICS_HPP_
#define OSS_PHYSICS_HPP_

#include "oss/Subsystem.hpp"

namespace oss
{

/*!
 * \brief TODO:
 */
class Physics : virtual public Subsystem
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTORS
    //--------------------------------------------------------------------------

    /*!
     * \brief TODO:
     */
    Physics()
        : Subsystem()
    {
        // set physics role
        m_roles = static_cast<Role>(m_roles | kRolePhysics);
    }

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    virtual ~Physics()
    {
    }

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    // TODO:
};

} // namespace oss

#endif
