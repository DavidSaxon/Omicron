/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_API_CONTEXT_SURFACE_HPP_
#define OMICRON_API_CONTEXT_SURFACE_HPP_

#include <arcanecore/base/Types.hpp>
#include <arcanecore/base/lang/Restrictors.hpp>

#include "omicron/api/API.hpp"


namespace omi
{
namespace context
{

/*!
 * \brief Singleton which can be used to query and control the rendering surface
 *        of Omicron.
 */
class Surface
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
public:

    //--------------------------------------------------------------------------
    //                          PUBLIC STATIC FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns the singleton instance of the Omicron surface.
     */
    OMI_API_EXPORT static Surface* instance();

    //-----------------------------ENGINE INTERNALS-----------------------------
    // hide from doxygen
    #ifndef IN_DOXYGEN

    /*!
     * \brief Sets the implementation that will be used by the current runtime.
     */
    OMI_API_EXPORT static void set_implementation(
            omi::SubsytemObject<Surface>* impl);

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
     * \brief Returns the width of this surface (in pixels).
     */
    virtual arc::int32 get_width() const = 0;

    /*!
     * \brief Returns the height of this window (in pixels).
     */
    virtual arc::int32 get_height() const = 0;

    /*!
     * \brief Returns the x position of the window (in pixels).
     */
    virtual arc::int32 get_position_x() const = 0;

    /*!
     * \brief Returns the y position of the window (in pixels).
     */
    virtual arc::int32 get_position_y() const = 0;

    // TODO: get context info

    // TODO: is_resizable

    // TODO: is_moveable

    // TODO: set_size

    // TODO: set_position

    /*!
     * \brief Sets whether the cursor is hidden or not.
     */
    virtual void hide_cursor(bool state) = 0;

    /*!
     * \brief Forces the cursor to return to the centre of the window at the end
     *        of each cycle.
     */
    virtual void lock_mouse(bool state) = 0;

protected:

    //--------------------------------------------------------------------------
    //                           PROTECTED CONSTRUCTOR
    //--------------------------------------------------------------------------

    OMI_API_EXPORT Surface();

    //--------------------------------------------------------------------------
    //                            PROTECTED DESTRUCTOR
    //--------------------------------------------------------------------------

    OMI_API_EXPORT virtual ~Surface();
};

} // namespace context
} // namespace omi

#endif
