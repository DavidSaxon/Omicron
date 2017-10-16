/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_RUNTIME_GAME_GAMEBINDING_HPP_
#define OMICRON_RUNTIME_GAME_GAMEBINDING_HPP_

#include <arcanecore/base/lang/Restrictors.hpp>
#include <arcanecore/base/str/UTF8String.hpp>


namespace omi
{
namespace runtime
{
namespace game
{

/*!
 * \brief Object that manages binding of the game implementation.
 */
class GameBinding
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
public:

    //--------------------------------------------------------------------------
    //                          PUBLIC STATIC FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns the singleton instance of GameBinding.
     */
    static GameBinding* instance();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief The startup routine of the GameBinding (finding and binding the
     *        game library).
     *
     * \return Whether the GameBinding completed its startup routines
     *         successfully.
     */
    bool startup_routine();

    /*!
     * \brief Performs the shutdown routines of the GameBinding (releasing the
     *        binding).
     *
     * \return Whether the GameBinding completed its shutdown routines
     *         successfully.
     */
    bool shutdown_routine();

    /*!
     * \brief Returns the name of the bound game.
     */
    const arc::str::UTF8String& get_game_name() const;

    /*!
     * \brief Returns the version of the bound game.
     */
    const arc::str::UTF8String& get_game_version() const;

    /*!
     * \brief Calls the game's startup routine (if it exists).
     *
     * \return Whether the game's startup routine completed successfully or
     *         ```true``` if the routine does not exist.
     */
    bool game_startup_routine();

    /*!
     * \brief Calls the game's first-frame routine (if it exists).
     *
     * \return Whether the game's first-frame routine completed successfully or
     *         ```true``` if the routine does not exist.
     */
    bool game_firstframe_routine();

    /*!
     * \brief Calls the game's shutdown routine (if it exists).
     *
     * \return Whether the game's shutdown routine completed successfully or
     *         ```true``` if the routine does not exist.
     */
    bool game_shutdown_routine();

private:

    //--------------------------------------------------------------------------
    //                            PRIVATE CONSTRUCTOR
    //--------------------------------------------------------------------------

    GameBinding();

    //--------------------------------------------------------------------------
    //                             PRIVATE DESTRUCTOR
    //--------------------------------------------------------------------------

    ~GameBinding();

    //--------------------------------------------------------------------------
    //                            COMPILATION FIREWALL
    //--------------------------------------------------------------------------

    class GameBindingImpl;
    GameBindingImpl* m_impl;
};

} // namespace game
} // namespace runtime
} // namespace omi

#endif
