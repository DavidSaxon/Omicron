/*!
 * \file
 * \author David Saxon
 * \brief The interface for registering a window subsystem.
 */
#ifndef OMICRON_API_WINDOW_SUBSYSTEM_INTERFACE_HPP_
#define OMICRON_API_WINDOW_SUBSYSTEM_INTERFACE_HPP_

#include <arcanecore/io/dl/DLOperations.hpp>


/*!
 * brief The function symbol used to register an Omicron window subsystem.
 */
#define OMICRON_API_WINDOW_SUBSYSTEM_REGISTER_SYMBOL omi_ss_register_window

/*!
 * \brief Defines the implemented types for a Omicron window subsystem.
 *
 * Every window subsystem should use this macro once and only once within its
 * implementation.
 *
 * \param BootstrapperType The subsystem's implementation of
 *                         omi::window::ss:Bootstrap.
 */
#define OMICRON_API_WINDOW_SUBSYSTEM_REGISTER(                                 \
        BootstrapperType,                                                      \
        MainWindowType)                                                        \
    extern "C"                                                                 \
    {                                                                          \
    omi::window::MainWindow* main_window_factory()                             \
    {                                                                          \
        return (omi::window::MainWindow*) MainWindowType::get_instance();      \
    }                                                                          \
    ARC_IO_DL_EXPORT void OMICRON_API_WINDOW_SUBSYSTEM_REGISTER_SYMBOL(        \
            void** bootstrap,                                                  \
            void** main_window_factory_func)                                   \
    {                                                                          \
        *bootstrap = BootstrapperType::get_instance();                         \
        *main_window_factory_func = (void*) &main_window_factory;              \
    }                                                                          \
    }


namespace omi
{
namespace window
{

//------------------------------------------------------------------------------
//                              FORWARD DECLARATIONS
//------------------------------------------------------------------------------

class MainWindow;

/*!
 * \brief Module for implementing Omicron window subsystems.
 */
namespace ss
{

/*!
 * \brief Function signature for the function which runs a cycle of
 *        Omicron's main loop.
 */
typedef bool (EngineCycleFunc)();

typedef omi::window::MainWindow* (MainWindowFactory)();

} // namespace ss
} // namespace window
} // namespace omi

#endif