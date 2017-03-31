/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_SUBSYSTEM_WINDOWMANAGER_HPP_
#define OMICRON_SUBSYSTEM_WINDOWMANAGER_HPP_

#include "omicron/subsystem/Subsystem.hpp"

namespace omi
{
namespace ss
{

/*!
 * \brief TODO:
 */
class WindowManager : virtual public Subsystem
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTORS
    //--------------------------------------------------------------------------

    /*!
     * \brief TODO:
     */
    WindowManager()
        : Subsystem()
    {
        // set window manager role
        m_roles = static_cast<Role>(m_roles | kRoleWindowManager);
    }

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    virtual ~WindowManager()
    {
    }

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief TODO
     */
    virtual void open_main_window() = 0;
};

} // namespace ss
} // namespace omi

#endif
