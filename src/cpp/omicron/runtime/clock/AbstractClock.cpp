#include "omicron/runtime/clock/AbstractClock.hpp"

namespace omi
{
namespace runtime
{

//------------------------------------------------------------------------------
//                                  CONSTRUCTORS
//------------------------------------------------------------------------------

AbstractClock::AbstractClock()
    :
    m_multiplier  (1.0F),
    m_paused      (false),
    m_ticked_once (false),
    m_last_tick   (0),
    m_current_time(0)
{
}

AbstractClock::AbstractClock(const AbstractClock& other)
    :
    m_multiplier  (other.m_multiplier),
    m_paused      (other.m_paused),
    m_ticked_once (other.m_ticked_once),
    m_last_tick   (other.m_last_tick),
    m_current_time(other.m_current_time)
{
}

AbstractClock::AbstractClock(AbstractClock&& other)
    :
    m_multiplier  (other.m_multiplier),
    m_paused      (other.m_paused),
    m_ticked_once (other.m_ticked_once),
    m_last_tick   (other.m_last_tick),
    m_current_time(other.m_current_time)
{
    // reset
    other.m_multiplier   = 1.0;
    other.m_paused       = false;
    other.m_ticked_once  = false;
    other.m_last_tick    = 0;
    other.m_current_time = 0;
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

AbstractClock::~AbstractClock()
{
}

//------------------------------------------------------------------------------
//                                   OPERATORS
//------------------------------------------------------------------------------

AbstractClock& AbstractClock::operator=(const AbstractClock& other)
{
    m_multiplier   = other.m_multiplier;
    m_paused       = other.m_paused;
    m_ticked_once  = other.m_ticked_once;
    m_current_time = other.m_current_time;
    m_last_tick    = other.m_last_tick;

    return *this;
}

AbstractClock& AbstractClock::operator=(AbstractClock&& other)
{
    m_multiplier   = other.m_multiplier;
    m_paused       = other.m_paused;
    m_ticked_once  = other.m_ticked_once;
    m_current_time = other.m_current_time;
    m_last_tick    = other.m_last_tick;
    // reset
    other.m_multiplier   = 1.0;
    other.m_paused       = false;
    other.m_ticked_once  = false;
    other.m_current_time = 0;
    other.m_last_tick    = 0;

    return *this;
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

AbstractClock::time_int AbstractClock::get_time(Metric metric)
{
    // get the conversion multiplier
    double conversion = metric / get_native_metric();
    // has the clock ticked once yet?
    if(!m_ticked_once)
    {
        m_last_tick    = tick();
        m_current_time = m_current_time;
        m_ticked_once = true;
    }
    // update time if the clock is not paused
    else if(!m_paused)
    {
        // get the difference since the clock
        time_int this_tick = tick();
        time_int diff = this_tick - m_last_tick;
        // update the last tick
        m_last_tick = this_tick;
        // update the time and take the multiplier into account
        m_current_time += diff * m_multiplier;
    }
    // return converted time
    return static_cast<time_int>(m_current_time / conversion);
}

double AbstractClock::get_multiplier() const
{
    return m_multiplier;
}

void AbstractClock::set_multiplier(double multiplier)
{
    m_multiplier = multiplier;
}

void AbstractClock::resume()
{
    m_paused = false;
}

void AbstractClock::pause()
{
    m_paused = true;
}

bool AbstractClock::is_paused() const
{
    return m_paused;
}

void AbstractClock::step(time_int amount, Metric metric)
{
    if(m_paused)
    {
        // get the conversion multiplier for the amount
        double conversion = get_native_metric() / metric;
        // step by the amount
        m_current_time += amount / conversion;
    }
}

} // namespace runtime
} // namespace omi
