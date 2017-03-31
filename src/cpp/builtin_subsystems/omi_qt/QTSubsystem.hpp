/*!
 * \file
 * \author David Saxon
 */
#ifndef OMI_QT_SUBSYSTEM_HPP_
#define OMI_QT_SUBSYSTEM_HPP_

#include <omicron/subsystem/Input.hpp>
#include <omicron/subsystem/UI.hpp>
#include <omicron/subsystem/WindowManager.hpp>

/*!
 * \brief TODO
 */
class QTSubsystem
    : public omi::ss::WindowManager
    , public omi::ss::Input
    , public omi::ss::UI
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTORS
    //--------------------------------------------------------------------------

    /*!
     * \brief TODO
     */
    QTSubsystem();

    //--------------------------------------------------------------------------
    //                                DESTRUCTORS
    //--------------------------------------------------------------------------

    /*!
     * \brief TODO
     */
    virtual ~QTSubsystem()
    {
    }

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief TODO:
     */
    virtual void boot();

    /*!
     * \brief TODO
     */
    virtual void open_main_window();
};

#endif
