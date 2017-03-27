/*!
 * \file
 * \author David Saxon
 */
#ifndef OSS_INPUT_HPP_
#define OSS_INPUT_HPP_

#include "oss/Subsystem.hpp"

// TODO: REMOVE ME
#include <iostream>

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
        // TODO: REMOVE ME
        std::cout << "input constructor!" << std::endl;

        // set window manager role
        m_roles = static_cast<Role>(m_roles | ROLE_INPUT);

        std::cout << "input roles: " << m_roles << std::endl;
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
