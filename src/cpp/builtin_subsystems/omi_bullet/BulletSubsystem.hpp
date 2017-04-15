/*!
 * \file
 * \author David Saxon
 */
#ifndef OMI_BULLET_SUBSYSTEM_HPP_
#define OMI_BULLET_SUBSYSTEM_HPP_

#include <omicron/subsystem/Physics.hpp>

/*!
 * \brief TODO
 */
class BulletSubsystem : public omi::ss::Physics
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTORS
    //--------------------------------------------------------------------------

    BulletSubsystem();

    //--------------------------------------------------------------------------
    //                                DESTRUCTORS
    //--------------------------------------------------------------------------

    virtual ~BulletSubsystem();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    // override
    virtual void startup();
};

#endif
