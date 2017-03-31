/*!
 * \file
 * \author David Saxon
 * \brief Globals for QTO.
 */
#ifndef OMI_QT_GLOBALS_HPP_
#define OMI_QT_GLOBALS_HPP_

#include <arcanelog/Input.hpp>

namespace global
{

/*!
 * \brief The logging input to be used by QTO.
 */
extern arclog::Input* input;

} // namespace global

/*!
 * \brief Convenience reference to the logger.
 *
 * Points to qto::global::input
 */
extern arclog::Input*& logger;


#endif
