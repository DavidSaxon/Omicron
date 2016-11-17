#include "omicron/runtime/clock/WallClock.hpp"

#include "arcanecore/base/clock/ClockOperations.hpp"

namespace omi
{
namespace runtime
{

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

AbstractClock::Metric WallClock::get_native_metric() const
{
    return METRIC_NANOSECONDS;
}

//------------------------------------------------------------------------------
//                           PROTECTED MEMBER FUNCTIONS
//------------------------------------------------------------------------------

AbstractClock::time_int WallClock::tick()
{
    return arc::clock::get_current_time(arc::clock::METRIC_NANOSECONDS);
}

} // namespace runtime
} // namespace omi
