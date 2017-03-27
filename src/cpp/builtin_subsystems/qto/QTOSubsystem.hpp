/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_SUBSYSTEMS_QTO_SUBSYSTEM_HPP_
#define OMICRON_SUBSYSTEMS_QTO_SUBSYSTEM_HPP_

#include <oss/Input.hpp>
#include <oss/WindowManager.hpp>

namespace qto
{

/*!
 * \brief TODO
 */
class QTOSubsystem
    : public oss::WindowManager
    , public oss::Input
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTORS
    //--------------------------------------------------------------------------

    /*!
     * \brief TODO
     */
    QTOSubsystem();

    //--------------------------------------------------------------------------
    //                                DESTRUCTORS
    //--------------------------------------------------------------------------

    /*!
     * \brief TODO
     */
    virtual ~QTOSubsystem()
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

} // namespace qto

#endif
