#include "omicron/runtime/clock/GameClock.hpp"

#include "arcanecore/base/clock/ClockOperations.hpp"

namespace omi
{
namespace runtime
{

//------------------------------------------------------------------------------
//                           PRIVATE STATIC ATTRIBUTES
//------------------------------------------------------------------------------

GameClock::time_int GameClock::s_game_start =
    arc::clock::get_current_time(arc::clock::METRIC_NANOSECONDS);

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

AbstractClock::Metric GameClock::get_native_metric() const
{
    return METRIC_NANOSECONDS;
}

//------------------------------------------------------------------------------
//                           PROTECTED MEMBER FUNCTIONS
//------------------------------------------------------------------------------

AbstractClock::time_int GameClock::tick()
{
    time_int current_time =
        arc::clock::get_current_time(arc::clock::METRIC_NANOSECONDS);
    return current_time - s_game_start;
}

} // namespace runtime
} // namespace omi
