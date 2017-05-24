/*!
 * \file
 * \author David Saxon
 * \brief The global definitions for the Omicron window interface.
 */
#ifndef OMICRON_API_WINDOW_WINDOW_HPP_
#define OMICRON_API_WINDOW_WINDOW_HPP_


namespace omi
{

/*!
 * \brief The window management interface of Omicron.
 */
namespace window
{

//------------------------------------------------------------------------------
//                                  ENUMERATORS
//------------------------------------------------------------------------------

/*!
 * \brief Defines the different modes a window can be in.
 */
enum WindowMode
{
    /*!
     * \brief The window is a standard window with borders.
     */
    kModeWindowed = 0,
    /*!
     * \brief A window without standard operating system provided borders.
     */
    kModeBorderless,
    /*!
     * \brief A window without borders and also occupies the entire screen.
     */
    kModeFullscreen
};

} // namespace window

} // namespace omi

#endif
