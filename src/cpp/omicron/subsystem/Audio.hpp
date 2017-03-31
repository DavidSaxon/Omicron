/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_SUBSYSTEM_AUDIO_HPP_
#define OMICRON_SUBSYSTEM_AUDIO_HPP_

#include "omicron/subsystem/Subsystem.hpp"

namespace omi
{
namespace ss
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

} // namespace ss
} // namespace omi

#endif
