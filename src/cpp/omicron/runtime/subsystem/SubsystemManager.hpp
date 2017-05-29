/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_RUNTIME_SUBSYSTEM_MANAGER_HPP_
#define OMICRON_RUNTIME_SUBSYSTEM_MANAGER_HPP_

#include <map>
#include <unordered_map>

#include <arcanecore/base/lang/Restrictors.hpp>
#include <arcanecore/base/str/UTF8String.hpp>
#include <arcanecore/io/dl/DLOperations.hpp>
#include <arcanecore/io/sys/Path.hpp>

#include <json/json.h>

#include <arcanecore/config/Variant.hpp>

// TODO: REMOVE ME
#include <omicron/subsystem/Subsystem.hpp>

#include "omicron/runtime/subsystem/WindowSubsystem.hpp"


namespace omi
{
namespace runtime
{
namespace ss
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
     * \brief Starts the main loop of Omicron via the window subsystem.
     */
    void start_main_loop(omi::window::ss::EngineCycleFunc* engine_cycle_func);

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
    arc::config::VariantPtr m_config_data;

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
     * \brief Stores the open DL handles so they can be deleted at shutdown.
     */
    std::unordered_map<
        arc::io::sys::Path,
        arc::io::dl::Handle
    > m_dl_handles;

    /*!
     * \brief The window manager subsystem.
     */
    WindowSubsystem m_window;





    /*!
     * \brief The name of the exported function symbol used to register
     *        subsystem libraries.
     */
    arc::str::UTF8String m_register_symbol;



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
     * \brief Utility to function to get or load a dl handle for the given
     *        subsystem role.
     */
    arc::io::dl::Handle get_or_load_library(const arc::str::UTF8String& role);

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

} // namespace ss
} // namespace runtime
} // namespace omi

#endif
