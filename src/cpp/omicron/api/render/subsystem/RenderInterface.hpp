/*!
 * \file
 * \author David Saxon
 * \brief The interface for registering a rendering subsystem.
 */
#ifndef OMICRON_API_RENDER_SUBSYSTEM_INTERFACE_HPP_
#define OMICRON_API_RENDER_SUBSYSTEM_INTERFACE_HPP_

#include <arcanecore/io/dl/DLOperations.hpp>

/*!
 * brief The function symbol used to register an Omicron rendering subsystem.
 */
#define OMICRON_API_RENDER_SUBSYSTEM_REGISTER_SYMBOL omi_ss_register_render

// TODO: REGISTER MACRO
#define OMICRON_API_RENDER_SUBSYSTEM_REGISTER(                                 \
        BootstrapperType)                                                      \
    extern "C"                                                                 \
    {                                                                          \
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
namespace render
{

/*!
 * \brief Module for implementing Omicron rendering subsystems.
 */
namespace ss
{

} // namespace ss
} // namespace render
} // namespace omi

#endif
