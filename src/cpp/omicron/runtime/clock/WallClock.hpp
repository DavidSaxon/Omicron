#ifndef OMICRON_RUNTIME_CLOCK_WALLCLOCK_HPP_
#define OMICRON_RUNTIME_CLOCK_WALLCLOCK_HPP_

#include "omicron/runtime/clock/AbstractClock.hpp"

namespace omi
{
namespace runtime
{

/*!
 * \brief Clock that measures real time from January 1st 1970.
 */
class WallClock : public AbstractClock
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
};

} // namespace runtime
} // namespace omi

#endif
