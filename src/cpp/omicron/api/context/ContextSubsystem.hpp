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
    //                                  TYPEDEFS
    //--------------------------------------------------------------------------

    /*!
     * \brief Function signature for the function which runs a cycle of
     *        Omicron's main loop.
     */
    typedef bool (EngineCycleFunc)();

    //--------------------------------------------------------------------------
    //                          PUBLIC STATIC FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns the singleton instance of the ContextSubsystem.
     */
    OMI_API_EXPORT static ContextSubsystem* instance();

    //-----------------------------ENGINE INTERNALS-----------------------------
    // hide from doxygen
    #ifndef IN_DOXYGEN

    /*!
     * \brief Sets the implementation that will be used by the current runtime.
     */
    OMI_API_EXPORT static void set_implementation(
            omi::SubsytemObject<ContextSubsystem>* impl);

    /*!
     * \brief Destroys the current implementation object.
     */
    OMI_API_EXPORT static void destroy();

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

    /*!
     * \brief Starts the main loop of Omicron which will be managed by context
     *        subsystem and will call the given function every cycle of the
     *        main loop.
     */
    virtual void main_loop(EngineCycleFunc* engine_cycle_func) = 0;

protected:

    //--------------------------------------------------------------------------
    //                           PROTECTED CONSTRUCTOR
    //--------------------------------------------------------------------------

    OMI_API_EXPORT ContextSubsystem();

    //--------------------------------------------------------------------------
    //                            PROTECTED DESTRUCTOR
    //--------------------------------------------------------------------------

    OMI_API_EXPORT virtual ~ContextSubsystem();
};

} // namespace context
} // namespace omi

/*!
 * \brief Macro to register a ContextSubsystem and it various components.
 *
 * \param version String literal which represents the version of the context
 *                subsystem being registered.
 * \param ContextSubsystemType The class name of the implementation of the
 *                             ContextSubsystem object.
 * \param SurfaceType The class name of the implementation of the Surface
 *                    object.
 */
#define OMI_CONTEXT_REGISTER_SUBSYSTEM(                                        \
        version,                                                               \
        ContextSubsystemType,                                                  \
        SurfaceType)                                                           \
    extern "C"                                                                 \
    {                                                                          \
    OMI_PLUGIN_EXPORT const char* OMI_CONTEXT_get_version()                    \
    {                                                                          \
        return version;                                                        \
    }                                                                          \
    OMI_PLUGIN_EXPORT void* OMI_CONTEXT_subsystem_factory()                    \
    {                                                                          \
        return new ContextSubsystemType();                                     \
    }                                                                          \
    OMI_PLUGIN_EXPORT void OMI_CONTEXT_subsystem_destroy(void* self)           \
    {                                                                          \
        delete static_cast<ContextSubsystemType*>(self);                       \
    }                                                                          \
    OMI_PLUGIN_EXPORT void* OMI_CONTEXT_surface_factory()                      \
    {                                                                          \
        return new SurfaceType();                                              \
    }                                                                          \
    OMI_PLUGIN_EXPORT void OMI_CONTEXT_surface_destroy(void* self)             \
    {                                                                          \
        delete static_cast<SurfaceType*>(self);                                \
    }                                                                          \
    }

#endif
