#pragma once

#include "ControllerBase.h"

#include "ch.h"

template <int TStackSize>
class ThreadController : public ControllerBase
{
private:
    THD_WORKING_AREA(m_threadstack, TStackSize);
    const tprio_t m_prio;

    /**
     * This exists because the OS can only call a function with a single void* param.  We have
     * to convert back to an instance of ThreadController, and call the task to run.
     */
    static void StaticThreadTaskAdapter(void* thread)
    {
        ThreadController* t = static_cast<ThreadController*>(thread);

        t->ThreadTask();
    }

protected:
    virtual void ThreadTask() = 0;
    thread_t* m_thread;

public:
    ThreadController(const char* name, tprio_t priority)
        : ControllerBase(name)
        , m_prio(priority)
    {
    }

    /**
     * @brief Start the thread.
     */
    virtual void Start()
    {
        m_thread = chThdCreateStatic(m_threadstack, sizeof(m_threadstack), m_prio, StaticThreadTaskAdapter, this);
        m_thread->name = GetName();
    }
};
