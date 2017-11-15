/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_API_RENDER_RENDER_SUBSYSTEM_HPP_
#define OMICRON_API_RENDER_RENDER_SUBSYSTEM_HPP_

#include <arcanecore/base/lang/Restrictors.hpp>

#include "omicron/api/API.hpp"


namespace omi
{

//------------------------------------------------------------------------------
//                              FORWARD DECLARATIONS
//------------------------------------------------------------------------------

namespace scene
{
class AbstractRenderable;
class Camera;
} // namespace scene

namespace render
{

/*!
 * \brief Abstract base class that should be implemented by an Omicron render
 *        subsystem.
 */
class RenderSubsystem
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
public:

    //--------------------------------------------------------------------------
    //                          PUBLIC STATIC FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns the singleton instance of the RenderSubsystem.
     */
    OMI_API_EXPORT static RenderSubsystem& instance();

    //-----------------------------ENGINE INTERNALS-----------------------------
    // hide from doxygen
    #ifndef IN_DOXYGEN

    /*!
     * \brief Sets the implementation that will be used by the current runtime.
     */
    OMI_API_EXPORT static void set_implementation(
            omi::SubsytemObject<RenderSubsystem>* impl);

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
     * \warning At the point this function is called there is not a valid GL
     *          context.
     *
     * \return Whether the routine completed successfully or not - note that in
     *         the case of failure the subsystem should report its own errors.
     */
    virtual bool startup_routine() = 0;

    /*!
     * \brief Performs the firstframe routine of this subsystem.
     *
     * \note When this function is called there is a valid GL context, so
     *       graphics state related setup should be done within this routine.
     *
     * \return Whether the routine completed successfully or not - note that in
     *         the case of failure the subsystem should report its own errors.
     */
    virtual bool firstframe_routine() = 0;

    /*!
     * \brief Performs the shutdown routine of this subsystem.
     *
     * \return Whether the routine completed successfully or not - note that in
     *         the case of failure the subsystem should report its own errors.
     */
    virtual bool shutdown_routine() = 0;

    /*!
     * \brief Adds a new renderable component that will be rendered by this
     *        subsystem.
     */
    virtual void add_renderable(omi::scene::AbstractRenderable* renderable) = 0;

    /*!
     * \brief Removes a renderable component from this subsystem.
     */
    virtual void remove_renderable(
            omi::scene::AbstractRenderable* renderable) = 0;

    /*!
     * \brief Tells this rendering subsystem which component should be used as
     *        the camera to render the scene. This can be null.
     *
     * \note If not null, this camera component will have already been passed to
     *       this subsystem via add_renderable. And will always be set to null
     *       if the component is removed.
     */
    virtual void set_active_camera(const omi::scene::Camera* camera) = 0;

    /*!
     * \brief Requests that a frame be rendered.
     */
    virtual void render() = 0;

protected:

    //--------------------------------------------------------------------------
    //                           PROTECTED CONSTRUCTOR
    //--------------------------------------------------------------------------

    OMI_API_EXPORT RenderSubsystem();

    //--------------------------------------------------------------------------
    //                            PROTECTED DESTRUCTOR
    //--------------------------------------------------------------------------

    OMI_API_EXPORT virtual ~RenderSubsystem();
};

/*!
 * \brief Macro to register a RenderSusbsystem and it various components.
 *
 * \param version String literal which represents the version of the render
 *                subsystem being registered.
 * \param RenderSubsystemType The class name of the implementation of the
 *                            RenderSubsystem object.
 */
#define OMI_RENDER_REGISTER_SUBSYSTEM(version, RenderSubsystemType)            \
    extern "C"                                                                 \
    {                                                                          \
    OMI_PLUGIN_EXPORT const char* OMI_RENDER_get_version()                     \
    {                                                                          \
        return version;                                                        \
    }                                                                          \
    OMI_PLUGIN_EXPORT void* OMI_RENDER_subsystem_create()                      \
    {                                                                          \
        return new RenderSubsystemType();                                      \
    }                                                                          \
    OMI_PLUGIN_EXPORT void OMI_RENDER_subsystem_destroy(void* self)            \
    {                                                                          \
        delete static_cast<RenderSubsystemType*>(self);                        \
    }                                                                          \
    }

} // namespace render
} // namespace omi

#endif
