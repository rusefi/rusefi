#pragma once

#include "ThreadController.h"
#include "efitime.h"

template <int TStackSize>
class PeriodicController : public ThreadController<TStackSize>
{
private:
    const systime_t m_period;
    bool m_running = true;

protected:
    const float m_periodSeconds;

    /**
     * @brief Called before running the periodic task.  Optionally override this method to set up.
     */
    virtual void OnStarted() {};

    /**
     * @brief Called periodically.  Override this method to do work for your controller.
     */
    virtual void PeriodicTask(efitime_t nowNt) = 0;

    /**
     * Called when the controller is stopped.  Optionally override this method to clean up.
     */
    virtual void OnStopped() {};

private:
    void ThreadTask() override final
    {
        OnStarted();

        while(m_running)
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

        OnStopped();
    }

public:
    PeriodicController(const char* name, tprio_t priority, float frequencyHz)
        : ThreadController<TStackSize>(name, priority)
        , m_period(CH_CFG_ST_FREQUENCY / frequencyHz)
        // we compute floating point seconds to get the ACTUAL period
        // as it may be different due to rounding errors
        , m_periodSeconds(m_period / (float)CH_CFG_ST_FREQUENCY)
    {
    }

    void Start() override final
    {
        m_running = true;

        ThreadController<TStackSize>::Start();
    }

    void Stop() override final
    {
        m_running = false;
    }
};
