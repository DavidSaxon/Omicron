/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_SUBSYSTEM_PHYSICS_HPP_
#define OMICRON_SUBSYSTEM_PHYSICS_HPP_

#include "omicron/subsystem/Subsystem.hpp"


namespace omi
{
namespace ss
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

} // namespace ss
} // namespace omi

#endif
