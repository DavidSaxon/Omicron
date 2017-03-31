/*!
 * \file
 * \author David Saxon
 */
#ifndef OMI_QT_SUBSYSTEM_HPP_
#define OMI_QT_SUBSYSTEM_HPP_

#include <oss/Input.hpp>
#include <oss/UI.hpp>
#include <oss/WindowManager.hpp>

/*!
 * \brief TODO
 */
class QTSubsystem
    : public oss::WindowManager
    , public oss::Input
    , public oss::UI
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
