/*!
 * \file
 * \author David Saxon
 */
#ifndef OSS_INPUT_HPP_
#define OSS_INPUT_HPP_

#include "oss/Subsystem.hpp"

namespace oss
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

} // namespace oss

#endif
