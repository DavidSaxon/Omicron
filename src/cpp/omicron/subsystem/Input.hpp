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
    //                                  TYPEDEFS
    //--------------------------------------------------------------------------

    typedef bool (*EngineCycleFunc)();

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

    /*!
     * \brief Is called to start the Input subsystem's main loop.
     *
     * \param engine_cycle A function which will execute a cycle of the Omicron
     *                     engine. This function should be called once every
     *                     cycle of the Input subsystem's main loop. This
     *                     function will return false when engine execution has
     *                     completed which signals this function should return
     *                     control.
     */
    virtual void start_main_loop(EngineCycleFunc engine_cycle) = 0;
};

} // namespace ss
} // namespace omi

#endif
