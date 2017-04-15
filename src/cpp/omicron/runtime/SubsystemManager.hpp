/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_RUNTIME_SUBSYSTEMMANAGER_HPP_
#define OMICRON_RUNTIME_SUBSYSTEMMANAGER_HPP_

#include <map>
#include <unordered_map>

#include <arcanecore/base/lang/Restrictors.hpp>
#include <arcanecore/base/str/UTF8String.hpp>
#include <arcanecore/io/dl/DLOperations.hpp>
#include <arcanecore/io/sys/Path.hpp>

#include <json/json.h>

#include <metaengine/Variant.hpp>

#include <omicron/subsystem/Subsystem.hpp>

namespace omi
{
namespace runtime
{

class SubsystemManager
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
private:

    //--------------------------------------------------------------------------
    //                            PRIVATE CONSTRUCTOR
    //--------------------------------------------------------------------------

    /*!
     * \brief Creates a instance of the manager.
     */
    SubsystemManager();

public:

    //--------------------------------------------------------------------------
    //                                  TYPEDEFS
    //--------------------------------------------------------------------------

    typedef std::vector<omi::ss::Subsystem*> SubsystemArray;

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
     * \brief Startups the manager and binds subsystem DSOs.
     *
     * If the manager is already initialized this function will do nothing.
     */
    void startup();

    /*!
     * \brief Shuts down the SubsystemManager.
     */
    void shutdown();

    /*!
     * \brief Returns the primary subsystem assigned to the given role.
     */
    omi::ss::Subsystem* get_subsystem(omi::ss::Subsystem::Role role);

    /*!
     * \brief Returns the list of subsystems assigned to the given role.
     */
    SubsystemArray get_subsystems(omi::ss::Subsystem::Role role);

private:

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
    metaengine::VariantPtr m_metadata;

    /*!
     * \brief The search path to locate subsystem libraries within.
     */
    std::vector<arc::io::sys::Path> m_search_path;

    /*!
     * \brief The expected file extension of subsystem libraries.
     */
    arc::str::UTF8String m_extension;

    /*!
     * \brief Mapping from names of potential subsystems to their paths within
     *        the search path.
     */
    std::unordered_map<arc::str::UTF8String, arc::io::sys::Path> m_potential;

    /*!
     * \brief The name of the exported function symbol used to register
     *        subsystem libraries.
     */
    arc::str::UTF8String m_register_symbol;

    /*!
     * \brief Stores the open DL handles so they can be deleted at shutdown.
     */
    std::unordered_map<
        arc::io::sys::Path,
        arc::io::dl::Handle> m_dl_handles;

    /*!
     * \brief Storage for the currently bound subsystems.
     */
    std::unordered_map<arc::str::UTF8String, omi::ss::Subsystem*>
        m_bind_storage;

    /*!
     * \brief The assigned subsystems (with the first being the primary) for
     *        the roles.
     */
    std::map<omi::ss::Subsystem::Role, SubsystemArray> m_assigned;

    //--------------------------------------------------------------------------
    //                             PRIVATE FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief If the given path is valid as a potential subsystem library then
     *        it will be added to the class storage of potential paths.
     */
    void add_if_ptoential_path(const arc::io::sys::Path& path);

    /*!
     * \brief Loads (or acquires from an already loaded instance) the DSOs
     *        requested for subsystem role, then binds the subsystem objects
     *        from the DSO.
     *
     * \param role The role to load subsystems for.
     */
    void bind_role(omi::ss::Subsystem::Role role);

    /*!
     * \brief Assigns the subsystem in storage with the given name for the role.
     */
    void assign_from_storage(
            omi::ss::Subsystem::Role role,
            const arc::str::UTF8String& subsystem_name);
};

} // namespace runtime
} // namespace omi

#endif
