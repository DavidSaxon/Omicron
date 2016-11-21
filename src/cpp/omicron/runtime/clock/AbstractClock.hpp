#ifndef OMICRON_RUNTIME_CLOCK_ABSTRACTCLOCK_HPP_
#define OMICRON_RUNTIME_CLOCK_ABSTRACTCLOCK_HPP_

#include <arcanecore/base/Types.hpp>

namespace omi
{
namespace runtime
{

/*!
 * \brief Abstract time keeping device.
 *
 * This objects provides functionality for modifying the speed of the clock or
 * steeping through clock ticks. For atomic clocks (such as a wall clock or a
 * CPU cycles clock) this functionality should still be provided for debugging
 * purposes.
 */
class AbstractClock
{
public:

    //--------------------------------------------------------------------------
    //                              TYPE DEFINITIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Unsigned Integral type used for measuring time.
     */
    typedef arc::uint64 time_int;

    //--------------------------------------------------------------------------
    //                                ENUMERATORS
    //--------------------------------------------------------------------------

    /*!
     * \brief Represents the different metrics that clock time can be measured
     *        in.
     */
    enum Metric
    {
        METRIC_NANOSECONDS  = 1UL,
        METRIC_MICROSECONDS = 1000UL,
        METRIC_MILLISECONDS = 1000000UL,
        METRIC_SECONDS      = 1000000000UL
    };

    //--------------------------------------------------------------------------
    //                                CONSTRUCTORS
    //--------------------------------------------------------------------------

    /*!
     * \brief Super constructor.
     */
    AbstractClock();

    /*!
     * \brief Super copy constructor.
     */
    AbstractClock(const AbstractClock& other);

    /*!
     * \brief Super move constructor.
     */
    AbstractClock(AbstractClock&& other);

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    virtual ~AbstractClock();

    //--------------------------------------------------------------------------
    //                                 OPERATORS
    //--------------------------------------------------------------------------

    /*!
     * \brief Assignment operator.
     */
    AbstractClock& operator=(const AbstractClock& other);

    /*!
     * \brief Move operator.
     */
    AbstractClock& operator=(AbstractClock&& other);

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns the highest resolution that this clock natively ticks time
     *        at.
     */
    virtual Metric get_native_metric() const = 0;

    /*!
     * \brief Returns the current time as an unsigned integral type using the
     *        given metric.
     */
    time_int get_time(Metric metric = METRIC_MILLISECONDS);

    /*!
     * \brief Returns the multiplier being used on the rate of time by this
     *        clock.
     *
     * \note Defaults to 1.0
     */
    double get_multiplier() const;

    /*!
     * \brief Sets the multiplier this clock will use on the rate of time.
     */
    void set_multiplier(double multiplier);

    /*!
     * \brief Resumes normal ticking of the clock if it has been paused.
     *
     * If the clock is not currently paused this function does nothing.
     */
    void resume();

    /*!
     * \brief Pauses ticking of the clock. This means the time returned by
     *        get_time() will remain unchanged until resume() is called or the
     *        clock is manually stepped.
     */
    void pause();

    /*!
     * \brief Returns whether this clock is currently paused or not.
     */
    bool is_paused() const;

    /*!
     * \brief Steps the clock by the given amount if the clock is currently
     *        paused.
     *
     * If the clock in not paused this function will have no affect.
     *
     * \param amount The amount to step the clock by.
     * \param metric The time metric that the amount parameter represents.
     */
    void step(time_int amount, Metric metric = METRIC_MILLISECONDS);

protected:

    //--------------------------------------------------------------------------
    //                         PROTECTED MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Should be implemented by derived classes and returns the current
     *        time for the device this clock is using.
     *
     * This function should not handle time multipliers or explicit stepping as
     * that will be handled by the AbstractClock class.
     */
    virtual time_int tick() = 0;

private:

    //--------------------------------------------------------------------------
    //                             PRIVATE ATTRIBUTES
    //--------------------------------------------------------------------------

    /*!
     * \brief The current multiplier being used for the rate of time.
     */
    double m_multiplier;
    /*!
     * \brief Whether the clock is currently paused or not.
     */
    bool m_paused;

    /*!
     * \brief Whether the clock has ticked once yet or not.
     */
    bool m_ticked_once;

    /*!
     * \brief The value of the last tick.
     */
    time_int m_last_tick;
    /*!
     * \brief The current time of this clock.
     */
    time_int m_current_time;
};

} // namespace runtime
} // namespace omi

#endif
