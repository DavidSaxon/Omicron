/*!
 * \file
 * \author David Saxon
 */
#ifndef OSS_AUDIO_HPP_
#define OSS_AUDIO_HPP_

#include "oss/Subsystem.hpp"

namespace oss
{

/*!
 * \brief TODO:
 */
class Audio : virtual public Subsystem
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTORS
    //--------------------------------------------------------------------------

    /*!
     * \brief TODO:
     */
    Audio()
        : Subsystem()
    {
        // set renderer role
        m_roles = static_cast<Role>(m_roles | kRoleAudio);
    }

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    virtual ~Audio()
    {
    }

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    // TODO:
};

} // namespace oss

#endif
