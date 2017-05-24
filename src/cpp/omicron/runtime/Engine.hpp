/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_RUNTIME_ENGINE_HPP_
#define OMICRON_RUNTIME_ENGINE_HPP_

#include <arcanecore/base/lang/Restrictors.hpp>


//------------------------------------------------------------------------------
//                   ____  __  _________________  ____  _   __
//                  / __ \/  |/  /  _/ ____/ __ \/ __ \/ | / /
//                 / / / / /|_/ // // /   / /_/ / / / /  |/ /
//                / /_/ / /  / // // /___/ _, _/ /_/ / /|  /
//                \____/_/  /_/___/\____/_/ |_|\____/_/ |_/
//------------------------------------------------------------------------------


namespace omi
{

//------------------------------------------------------------------------------
//                              FORWARD DECLARATIONS
//------------------------------------------------------------------------------

namespace ss
{
class Input;
class Renderer;
class WindowManager;
} // namespace ss

namespace runtime
{

/*!
 * \brief Singleton object that owns and starts the Omicron runtime.
 */
class Engine
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
public:

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    ~Engine();

    //--------------------------------------------------------------------------
    //                          PUBLIC STATIC FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns the singleton instance of the Omicron engine.
     */
    static Engine* get_instance();


    /*!
     * \brief Performs a cycle of execution of the Omicron engine.
     *
     * Is called by the input subsystem.
     */
    static bool cycle_static();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Begins execution of Omicron.
     *
     *  Control is only returned from this function once Omicron exits.
     */
    int execute();

    /*!
     * \brief Performs a cycle of execution of the Omicron engine.
     *
     * Is called by the cycle_static function.
     */
    bool cycle();

private:

    //--------------------------------------------------------------------------
    //                             PRIVATE ATTRIBUTES
    //--------------------------------------------------------------------------

    /*!
     * \brief Whether setup has performed or not.
     *
     * Setup occurs on the very first frame of the engine to setup all systems
     * that require the engine to be in a valid state or a GL context.
     */
    bool m_setup;

    /*!
     * \brief The current window manager subsystem.
     */
    omi::ss::WindowManager* m_window_manager;
    /*!
     * \brief The current input subsystem.
     */
    omi::ss::Input* m_input;
    /*!
     * \brief The current renderer subsystem.
     */
    omi::ss::Renderer* m_renderer;

    //--------------------------------------------------------------------------
    //                            PRIVATE CONSTRUCTOR
    //--------------------------------------------------------------------------

    /*!
     * \brief Creates a new instance of the engine.
     */
    Engine();
};

} // namespace runtime
} // namespace omi

#endif
