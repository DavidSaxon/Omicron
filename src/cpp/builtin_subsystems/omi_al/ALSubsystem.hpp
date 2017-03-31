/*!
 * \file
 * \author David Saxon
 */
#ifndef OMI_AL_SUBSYSTEM_HPP_
#define OMI_AL_SUBSYSTEM_HPP_

#include <omicron/subsystem/Audio.hpp>

/*!
 * \brief TODO
 */
class ALSubsystem : public omi::ss::Audio
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTORS
    //--------------------------------------------------------------------------

    /*!
     * \brief TODO
     */
    ALSubsystem();

    //--------------------------------------------------------------------------
    //                                DESTRUCTORS
    //--------------------------------------------------------------------------

    /*!
     * \brief TODO
     */
    virtual ~ALSubsystem();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief TODO:
     */
    virtual void boot();
};

#endif
