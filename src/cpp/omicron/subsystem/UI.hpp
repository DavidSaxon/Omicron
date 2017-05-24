/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_SUBSYSTEM_UI_HPP_
#define OMICRON_SUBSYSTEM_UI_HPP_

#include "omicron/subsystem/Subsystem.hpp"


namespace omi
{
namespace ss
{

/*!
 * \brief TODO:
 */
class UI : virtual public Subsystem
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTORS
    //--------------------------------------------------------------------------

    /*!
     * \brief TODO:
     */
    UI()
        : Subsystem()
    {
        // set UI role
        m_roles = static_cast<Role>(m_roles | kRoleUI);
    }

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    virtual ~UI()
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
