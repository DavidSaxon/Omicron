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

    ALSubsystem();

    //--------------------------------------------------------------------------
    //                                DESTRUCTORS
    //--------------------------------------------------------------------------

    virtual ~ALSubsystem();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    // override
    virtual void startup();
};

#endif
