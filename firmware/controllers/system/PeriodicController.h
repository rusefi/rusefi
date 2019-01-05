/**
 *  @file PeriodicController.h
 * 
 *  @date Jan 5, 2019
 *  @author Matthew Kennedy, (c) 2019
 */

#pragma once

#include "ThreadController.h"
#include "efitime.h"


/**
 * @brief Base class for a controller that needs to run periodically to perform work.
 * 
 * For example, if we have some PID loop that needs to run at a specified frequency,
 * inherit this class, and perform your period update in PeriodicTask.  Any one-time
 * setup work can be performed in OnStarted().
 * 
 * This class effectively implements this funtionality:
 * 
 * void thread()
 * {
 *     OnStarted();
 * 
 *     while(true)
 *     {
 *         PeriodicTask(getTimeNowNt());
 *         sleep();
 *     }
 * }
 */
template <int TStackSize>
class PeriodicController : public ThreadController<TStackSize>
{
private:
    const systime_t m_period;
    
protected:
    /**
     * The target period between calls to PeriodicTask.
     */
    const float m_periodSeconds;

    /**
     * @brief Called before running the periodic task.  Optionally override this method to set up.
     */
    virtual void OnStarted() {};

    /**
     * @brief Called periodically.  Override this method to do work for your controller.
     */
    virtual void PeriodicTask(efitime_t nowNt) = 0;

private:
    void ThreadTask() override final
    {
        OnStarted();

        while(true)
        {
            systime_t before = chVTGetSystemTime();
            efitime_t nowNt = getTimeNowNt();

            // Run the controller's periodic work
            PeriodicTask(nowNt);

            // This ensures the loop _actually_ runs at the desired frequency.
            // Suppose we want a loop speed of 500hz:
            // If the work takes 1ms, and we wait 2ms (1 / 500hz), we actually
            // get a loop at 333 hz.  We need to wait until 2ms after we START
            // doing work, so the loop runs at a predictable 500hz.
            chThdSleepUntilWindowed(before, before + m_period);
        }
    }

public:
    PeriodicController(const char* name, tprio_t priority, float frequencyHz)
        : ThreadController<TStackSize>(name, priority)
        // First compute the period in systime_t
        , m_period(CH_CFG_ST_FREQUENCY / frequencyHz)
        // Then compute the float period off of the integer one to
        //  get the ACTUAL period, which may be slightly different than requested.
        , m_periodSeconds(m_period / (float)CH_CFG_ST_FREQUENCY)
    {
    }
};
