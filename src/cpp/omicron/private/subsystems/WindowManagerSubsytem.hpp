#ifndef OMICRON_PRIVATE_SUBSYSTEMS_WINDOWMANAGERSUBSYSTEM_HPP_
#define OMICRON_PRIVATE_SUBSYSTEMS_WINDOWMANAGERSUBSYSTEM_HPP_

#include <arcanecore/base/Preproc.hpp>
#include <arcanecore/io/dl/DLOperations.hpp>

namespace omi_
{
namespace subsys
{

class WindowManagerSubsystem
{
private:

    ARC_DISALLOW_COPY_AND_ASSIGN(WindowManagerSubsystem);

public:

    //--------------------------------------------------------------------------
    //                          PUBLIC STATIC FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Retursn the singleton instance of the window manager subsystem.
     */
    static WindowManagerSubsystem* get_instance();

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    ~WindowManagerSubsystem();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Performs the startup routine for the window manager subsystem.
     *
     * \param dl_handle The handle for the dynamic library for the window
     *                  manager subsystem.
     */
    bool startup(arc::io::dl::Handle dl_handle);

    /*!
     * \brief Performs the shutdown routine for the window manager subsystem.
     */
    bool shutdown();

    // TODO:

private:

    //--------------------------------------------------------------------------
    //                             PRIVATE ATTRIBUTES
    //--------------------------------------------------------------------------

    /*!
     * \brief Whether the window manager subsystem has been initialised yet or
     *        not.
     */
    bool m_initialised;

    /*!
     * \brief The handle to the dynamic library for the window manager
     *        subsystem.
     */
    arc::io::dl::Handle dl_handle;

    //--------------------------------------------------------------------------
    //                            PRIVATE CONSTRUCTORS
    //--------------------------------------------------------------------------

    WindowManagerSubsystem();
};

} // namespace subsys
} // namespace omi_

#endif
