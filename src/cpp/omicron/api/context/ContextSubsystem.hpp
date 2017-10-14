/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_API_CONTEXT_CONTEXTSUBSYSTEM_HPP_
#define OMICRON_API_CONTEXT_CONTEXTSUBSYSTEM_HPP_

#include <arcanecore/base/lang/Restrictors.hpp>

#include "omicron/api/API.hpp"


namespace omi
{
namespace context
{

//------------------------------------------------------------------------------
//                              FORWARD DECLARATIONS
//------------------------------------------------------------------------------

class Surface;

/*!
 * \brief Abstract base class that should be implemented by an Omicron context
 *        subsystem.
 */
class ContextSubsystem
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
public:

    //--------------------------------------------------------------------------
    //                          PUBLIC STATIC FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns the singleton instance of the ContextSubsystem.
     */
    OMI_API_GLOBAL static ContextSubsystem* instance();

    //-----------------------------ENGINE INTERNALS-----------------------------
    // hide from doxygen
    #ifndef IN_DOXYGEN

    /*!
     * \brief Sets the implementation that will be used by the current runtime.
     */
    OMI_API_GLOBAL static void set_implementation(
            omi::SubsytemObject<ContextSubsystem>* impl);

    /*!
     * \brief Destroys the current implementation object.
     */
    OMI_API_GLOBAL static void destroy();

    #endif
    // IN_DOXYGEN
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Performs the startup routine of this subsystem.
     *
     * \return Whether the routine completed successfully or not - note that in
     *         the case of failure the subsystem should report its own errors.
     */
    virtual bool startup_routine() = 0;

    /*!
     * \brief Performs the shutdown routine of this subsystem.
     *
     * \return Whether the routine completed successfully or not - note that in
     *         the case of failure the subsystem should report its own errors.
     */
    virtual bool shutdown_routine() = 0;

protected:

    //--------------------------------------------------------------------------
    //                           PROTECTED CONSTRUCTOR
    //--------------------------------------------------------------------------

    OMI_API_GLOBAL ContextSubsystem();

    //--------------------------------------------------------------------------
    //                            PROTECTED DESTRUCTOR
    //--------------------------------------------------------------------------

    OMI_API_GLOBAL virtual ~ContextSubsystem();
};

} // namespace context
} // namespace omi

#endif
