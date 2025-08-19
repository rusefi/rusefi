/**
 *  @file thread_controller.h
 * 
 *  @date Jan 5, 2019
 *  @author Matthew Kennedy, (c) 2019
 */

#pragma once

/**
 * @brief A base class for a controller that requires its own thread.
 * 
 * Inherit from ThreadController.  Implement ThreadTask with the logic required for your thread.
 * The template parameter specifies the size of the stack used for the thread.  (because we have to
 * allocate the stack at compile time, it has to be a template parameter instead of a normal parameter)
 */
template <int TStackSize>
class ThreadController : public chibios_rt::BaseStaticThread<TStackSize>
{
private:
	const tprio_t m_prio;
	/* TODO: use ref instead of m_started */
	bool m_started = false;
	chibios_rt::ThreadReference ref;

protected:
	// Override this function to implement your controller's thread's behavior.
	virtual void ThreadTask() = 0;

	void main() override {
		this->setName(m_name);

		ThreadTask();
	}

	const char* const m_name;

public:
	ThreadController(const char* name, tprio_t priority)
		: m_prio(priority)
		, m_name(name)
	{
	}

	using chibios_rt::BaseStaticThread<TStackSize>::start;

	/**
	 * @brief Start the thread.
	 */
	void start()
	{
		if (!m_started)
		{
			m_started = true;
			ref = chibios_rt::BaseStaticThread<TStackSize>::start(m_prio);
		}
	}

	/**
	 * @brief Request thread termination and waits for termination
	 *
	 * Thread should periadicaly execute something like:
	 * if (chThdShouldTerminateX())
	 * 		chThdExit((msg_t)0x0);
	 */
	void stop()
	{
		if (m_started) {
			/* Asking for thread termination.*/
			ref.requestTerminate();

			/* Waiting for termination, releasing the reference.*/
			ref.wait();

			m_started = false;
		}
	}
};
