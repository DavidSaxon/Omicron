/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_RUNTIME_SUBSYSTEM_SUBSYSTEMMANAGER_HPP_
#define OMICRON_RUNTIME_SUBSYSTEM_SUBSYSTEMMANAGER_HPP_

#include <arcanecore/base/lang/Restrictors.hpp>


namespace omi
{
namespace runtime
{
namespace ss
{

/*!
 * \brief Object that manages determines and finds which implementations of
 *        subsystems should be used, and binds them into Omicron.
 */
class SubsystemManager
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
public:

    //--------------------------------------------------------------------------
    //                          PUBLIC STATIC FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns the singleton instance of the SubsystemManager.
     */
    static SubsystemManager* instance();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Performs the startup routines of the SubsystemManager
     *       (determining, finding, and binding subsystems).
     *
     * \return Whether the SubsystemManager completed its startup routines
     *         successfully.
     */
    bool startup_routine();

    /*!
     * \brief Performs the shutdown routines of the SubsystemManager (releasing
     *        the subsystems that have been bound into Omicron).
     *
     * \return Whether the SubsystemManager completed its shutdown routines
     *         successfully.
     */
    bool shutdown_routine();

private:

    //--------------------------------------------------------------------------
    //                            PRIVATE CONSTRUCTOR
    //--------------------------------------------------------------------------

    SubsystemManager();

    //--------------------------------------------------------------------------
    //                             PRIVATE DESTRUCTOR
    //--------------------------------------------------------------------------

    ~SubsystemManager();

    //--------------------------------------------------------------------------
    //                            COMPILATION FIREWALL
    //--------------------------------------------------------------------------

    class SubsystemManagerImpl;
    SubsystemManagerImpl* m_impl;
};

} // namespace ss
} // namespace runtime
} // namespace omi

#endif
