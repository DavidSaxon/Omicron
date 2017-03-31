/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_SUBSYSTEM_INPUT_HPP_
#define OMICRON_SUBSYSTEM_INPUT_HPP_

#include "omicron/subsystem/Subsystem.hpp"

namespace omi
{
namespace ss
{

/*!
 * \brief TODO:
 */
class Input : virtual public Subsystem
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTORS
    //--------------------------------------------------------------------------

    /*!
     * \brief TODO:
     */
    Input()
        : Subsystem()
    {
        // set input role
        m_roles = static_cast<Role>(m_roles | kRoleInput);
    }

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    virtual ~Input()
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
