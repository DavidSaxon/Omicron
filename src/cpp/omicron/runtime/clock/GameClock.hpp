#ifndef OMICRON_RUNTIME_CLOCK_GAMECLOCK_HPP_
#define OMICRON_RUNTIME_CLOCK_GAMECLOCK_HPP_

#include "omicron/runtime/clock/AbstractClock.hpp"

namespace omi
{
namespace runtime
{

/*!
 * \brief Clock that measures time since the game engine last started.
 */
class GameClock : public AbstractClock
{
public:

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    // override
    virtual Metric get_native_metric() const;

protected:

    //--------------------------------------------------------------------------
    //                         PROTECTED MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    // override
    time_int tick();

private:

    //--------------------------------------------------------------------------
    //                         PRIVATE STATIC ATTRIBUTES
    //--------------------------------------------------------------------------

    /*!
     * \brief Represents wall clock time when the game was started.
     */
    static time_int s_game_start;
};

} // namespace runtime
} // namespace omi

#endif
