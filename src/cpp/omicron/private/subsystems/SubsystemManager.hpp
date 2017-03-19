#ifndef OMICRON_PRIVATE_SUBSYSTEMS_SUBSYSTEMMANAGER_HPP_
#define OMICRON_PRIVATE_SUBSYSTEMS_SUBSYSTEMMANAGER_HPP_

#include <arcanecore/base/str/UTF8String.hpp>
#include <arcanecore/io/dl/DLOperations.hpp>
#include <arcanecore/io/sys/Path.hpp>

#include "omicron/private/util/MetaUtil.hpp"
#include "omicron/subsystem/SubsystemCommon.h"

namespace omi_
{
namespace subsys
{

class SubsystemManager
{
private:

    ARC_DISALLOW_COPY_AND_ASSIGN(SubsystemManager);

public:

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    ~SubsystemManager();

    //--------------------------------------------------------------------------
    //                          PUBLIC STATIC FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns the singleton instance of the SubsystemManager.
     */
    static SubsystemManager* get_instance();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Starts up the SubsystemManager and binds subsystem DSOs.
     *
     * If the manager is already initialized this function will do nothing.
     *
     * \return Whether startup completed successfully.
     */
    bool startup();

    /*!
     * \brief Shuts down the SubsystemManager.
     *
     * \return Whether shutdown completed successfully.
     */
    bool shutdown();

private:

    //--------------------------------------------------------------------------
    //                              TYPE DEFINITIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Maps subsystem names to the roles they should fulfill.
     */
    typedef std::map<arc::str::UTF8String, ossRole> NamesToRoles;

    // TOOD: move to subsystem object?
    /*!
     * \brief The format for the register function that all subsystems must
     *        implement.
     *
     * \param name Returns the name of the subsystem.
     * \param version Returns the version of the subsystem
     * \param roles Returns the roles this subsystem fulfills.
     */
    typedef void (RegisterFunc)(char** name, char** version, ossRole& roles);

    //--------------------------------------------------------------------------
    //                             PRIVATE ATTRIBUTES
    //--------------------------------------------------------------------------

    /*!
     * \brief Whether the SubsystemManager has been initialised yet or not.
     */
    bool m_initialised;

    /*!
     * \brief The MetaEngine Variant for general subsystem configuration.
     */
    util::meta::VariantPtr m_metadata;

    //--------------------------------------------------------------------------
    //                            PRIVATE CONSTRUCTORS
    //--------------------------------------------------------------------------

    /*!
     * \brief Creates a new subsystem manager instance.
     */
    SubsystemManager();

    //--------------------------------------------------------------------------
    //                          PRIVATE STATIC FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Updates or inserts a new entry for the given name with the role.
     */
    static void update_roles(
            const arc::str::UTF8String& name,
            ossRole role,
            NamesToRoles& names_to_roles);

    //--------------------------------------------------------------------------
    //                          PRIVATE MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Builds a mapping from subsystem names to the roles that subsystem
     *        that has been requested to fulfill.
     */
    void build_names_to_roles_mapping(NamesToRoles& names_to_roles);

    /*!
     * \brief Attempts to bind the given file as a subsystem dynamic library.
     *
     * \param path The path to the subsystem to the subsystem to attempt to
     *             bind.
     * \param name The name of the subsystem, as returned by the library's
     *             register function.
     * \param version The version of the subsystem, as returned by the library's
     *                register function.
     * \param roles The roles that this subsystem fulfills, as returned by the
     *              library's register function.
     *
     * \return Whether a subsystem could be bound from the given path.
     */
    bool bind_subsystem(
            const arc::io::sys::Path& path,
            arc::str::UTF8String& name,
            arc::str::UTF8String& version,
            ossRole& roles);

    // TODO: exceptions?
    /*!
     * \brief Attempts to set the given subsystem dynamic for the roles.
     */
    void set_subsystem(ossRole roles, arc::io::dl::Handle dl_handle);
};

} // namespace subsys
} // namespace omi_

#endif
