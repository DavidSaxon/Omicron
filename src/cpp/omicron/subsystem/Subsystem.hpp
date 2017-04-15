/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_SUBSYSTEM_SUBSYSTEM_HPP_
#define OMICRON_SUBSYSTEM_SUBSYSTEM_HPP_

#include <arcanecore/base/Exceptions.hpp>
#include <arcanecore/base/lang/Restrictors.hpp>
#include <arcanecore/base/str/StringOperations.hpp>
#include <arcanecore/base/str/UTF8String.hpp>
#include <arcanecore/io/dl/DLOperations.hpp>

/*!
 * \brief Macro that should be used exactly once in a subsystem DSO in order
 *        to provided the needed symbols for Omicron to register the subsystem.
 *
 * This macro should be used in the global namespace.
 *
 * \param subsystem The class name of the subsystem object the library is
 *                  registering.
 */
#define OSS_REGISTER_SUBSYSTEM(subsystem)                                      \
extern "C"                                                                     \
{                                                                              \
ARC_IO_DL_EXPORT void* oss_register_lib()                                      \
{                                                                              \
    return static_cast<omi::ss::Subsystem*>(new subsystem());                  \
}                                                                              \
}

namespace omi
{
namespace ss
{

/*!
 * \brief TODO:
 */
class Subsystem
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
public:

    //--------------------------------------------------------------------------
    //                                ENUMERATORS
    //--------------------------------------------------------------------------

    /*!
     * \brief The possible roles a subsystem can fulfill.
     *
     * A subsystem may fulfill one or more of these rolls.
     */
    enum Role
    {
        /// The subsystem will provide the window manager functionality, this
        /// includes providing the main window of Omicron, and a GL context.
        kRoleWindowManager = 1L << 0,
        /// The subsystem will provide functionality for querying user input
        /// from one or more difference devices.
        kRoleInput          = 1L << 1,
        /// The subsystem will provide UI rendering and functionality.
        kRoleUI             = 1L << 2,
        /// The subsystem will provide Omicron's 3D rendering.
        kRoleRenderer       = 1L << 3,
        /// The subsystem will provide Omicron's physics simulation systems.
        kRolePhysics        = 1L << 4,
        /// The subsystem will provide audio playback.
        kRoleAudio          = 1L << 5
    };

    //--------------------------------------------------------------------------
    //                                CONSTRUCTORS
    //--------------------------------------------------------------------------

    /*!
     * \brief TODO
     */
    Subsystem()
        : m_roles(static_cast<Role>(0))
    {
    }

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    virtual ~Subsystem()
    {
    }

    //--------------------------------------------------------------------------
    //                          PUBLIC STATIC FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns a string representation of the given subsystem role(s).
     */
    static arc::str::UTF8String role_to_string(Role role)
    {
        std::vector<arc::str::UTF8String> role_strings;
        if(role & kRoleWindowManager)
        {
            role_strings.push_back("window_manager");
        }
        if(role & kRoleInput)
        {
            role_strings.push_back("input");
        }
        if(role & kRoleUI)
        {
            role_strings.push_back("ui");
        }
        if(role & kRoleRenderer)
        {
            role_strings.push_back("renderer");
        }
        if(role & kRolePhysics)
        {
            role_strings.push_back("physics");
        }
        if(role & kRoleAudio)
        {
            role_strings.push_back("audio");
        }

        return arc::str::join(role_strings, ", ");
    }

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns the bit-field of the roles this subsystem fulfills.
     */
    Role get_roles() const
    {
        return m_roles;
    }

    /*!
     * \brief Startups up this subsystem.
     *
     * \warning This function should not be called manually.
     *
     * Other than the constructor, this will be the first call made to this
     * object, and will only be called once.
     */
    virtual void startup() {};

    /*!
     * \brief Performs shutdown of this subsystem.
     *
     * \warning This function should not be called manually.
     *
     * Other than the destructor, once this function has been called, no further
     * calls to this object will be made, and shutdown will only be called once.
     */
    virtual void shutdown() {}

protected:

    //--------------------------------------------------------------------------
    //                            PROTECTED ATTRIBUTES
    //--------------------------------------------------------------------------

    /*!
     * \brief Defines the roles this Subsystem fulfills.
     */
    Role m_roles;
};

} // namespace ss
} // namespace omi

#endif
