/*!
 * \file
 * \author David Saxon
 */
#ifndef OSS_SUBSYSTEM_HPP_
#define OSS_SUBSYSTEM_HPP_

#include <arcanecore/base/Exceptions.hpp>
#include <arcanecore/base/lang/Restrictors.hpp>
#include <arcanecore/base/str/StringOperations.hpp>
#include <arcanecore/base/str/UTF8String.hpp>
#include <arcanecore/io/dl/DLOperations.hpp>

// TODO: REMOVE ME
#include <iostream>

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
    return static_cast<oss::Subsystem*>(new subsystem());                      \
}                                                                              \
}

namespace oss
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
        ROLE_WINDOW_MANAGER = 1L << 0,
        /// The subsystem will provide functionality for querying user input
        /// from one or more difference devices.
        ROLE_INPUT          = 1L << 1,
        /// The subsystem will provide UI rendering and functionality.
        ROLE_UI             = 1L << 2,
        /// The subsystem will provide Omicron's 3D rendering.
        ROLE_RENDERER       = 1L << 3,
        /// The subsystem will provide Omicron's physics simulation systems.
        ROLE_PHYSICS        = 1L << 4,
        /// The subsystem will provide audio playback.
        ROLE_AUDIO          = 1L << 5
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
        std::cout << "subsystem super constructor!" << std::endl;
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
        if(role & ROLE_WINDOW_MANAGER)
        {
            role_strings.push_back("window_manager");
        }
        if(role & ROLE_INPUT)
        {
            role_strings.push_back("input");
        }
        if(role & ROLE_UI)
        {
            role_strings.push_back("ui");
        }
        if(role & ROLE_RENDERER)
        {
            role_strings.push_back("renderer");
        }
        if(role & ROLE_PHYSICS)
        {
            role_strings.push_back("physics");
        }
        if(role & ROLE_AUDIO)
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
     * \brief TODO:
     */
    virtual void boot() = 0;

protected:

    //--------------------------------------------------------------------------
    //                            PROTECTED ATTRIBUTES
    //--------------------------------------------------------------------------

    /*!
     * \brief Defines the roles this Subsystem fulfills.
     */
    Role m_roles;
};

} // namespace oss

#endif
