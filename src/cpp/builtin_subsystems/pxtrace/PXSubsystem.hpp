/*!
 * \file
 * \author David Saxon
 */
#ifndef PXTRACE_SUBSYSTEM_HPP_
#define PXTRACE_SUBSYSTEM_HPP_

#include <oss/Renderer.hpp>

/*!
 * \brief TODO
 */
class PXSubsystem : public oss::Renderer
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTORS
    //--------------------------------------------------------------------------

    /*!
     * \brief TODO
     */
    PXSubsystem();

    //--------------------------------------------------------------------------
    //                                DESTRUCTORS
    //--------------------------------------------------------------------------

    /*!
     * \brief TODO
     */
    virtual ~PXSubsystem();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief TODO:
     */
    virtual void boot();
};

#endif
