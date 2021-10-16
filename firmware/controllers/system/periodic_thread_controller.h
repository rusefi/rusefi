/**
 *  @file periodic_thread_controller.h
 * 
 *  @date Jan 5, 2019
 *  @author Matthew Kennedy, (c) 2019
 */

#pragma once

#include "thread_controller.h"
#include "efitime.h"
#include "perf_trace.h"

/**
 * @brief Base class for a controller that needs to run periodically to perform work.
 * 
 * For example, if we have some PID loop that needs to run at a specified frequency,
 * inherit this class, and perform your period update in PeriodicTask.  Any one-time
 * setup work can be performed in OnStarted().
 * 
 * Each instance has one underlying thread meaning that task could be blocking/synchronous.
 * This class effectively implements this functionality:
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
	// time in ChibiOS time units, see CH_CFG_ST_FREQUENCY
    systime_t m_period;
    
protected:
    /**
     * The target period between calls to PeriodicTask.
     */
//    const float m_periodSeconds;

    /**
     * @brief Called before running the periodic task.  Optionally override this method to set up.
     */
    virtual void OnStarted() {};

    /**
     * @brief Called periodically.  Override this method to do work for your controller.
     */
    virtual void PeriodicTask(efitick_t nowNt) = 0;

private:
    void ThreadTask() override final
    {
        OnStarted();

        while(!chThdShouldTerminateX()) {
            systime_t before = chVTGetSystemTime();
            efitick_t nowNt = getTimeNowNt();

			{
				ScopePerf perf(PE::PeriodicControllerPeriodicTask);

				// Run the controller's periodic work
				PeriodicTask(nowNt);
			}

            // This ensures the loop _actually_ runs at the desired frequency.
            // Suppose we want a loop speed of 500hz:
            // If the work takes 1ms, and we wait 2ms (1 / 500hz), we actually
            // get a loop at 333 hz.  We need to wait until 2ms after we START
            // doing work, so the loop runs at a predictable 500hz.
            chThdSleepUntilWindowed(before, before + m_period);
        }

		firmwareError(OBD_PCM_Processor_Fault, "Thread died: %s", this->m_name);
	}

public:
    PeriodicController(const char* name, tprio_t priority, float frequencyHz)
        : ThreadController<TStackSize>(name, priority)
        // First compute the period in systime_t
        , m_period(CH_CFG_ST_FREQUENCY / frequencyHz)
        // Then compute the float period off of the integer one to
        //  get the ACTUAL period, which may be slightly different than requested.
//        , m_periodSeconds(m_period / (float)CH_CFG_ST_FREQUENCY)
    {
    }

    PeriodicController(const char* name) : PeriodicController (name, NORMALPRIO, 1) {
    }

    /**
     * sets milliseconds period
     */
    void setPeriod(int periodMs) {
    	float frequencyHz = 1000.0 / periodMs;
    	this->m_period = CH_CFG_ST_FREQUENCY / frequencyHz;
    }
};

// let's make sure period is not below specified threshold
#define NOT_TOO_OFTEN(threshold, value) maxI(threshold, value)
