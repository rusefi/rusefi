/**
 * @file	scheduler.h
 *
 * @date May 18, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */
#pragma once

typedef void (*schfunc_t)(void *);

template<class To, class From>
std::enable_if_t<
	sizeof(To) == sizeof(From) &&
	std::is_trivially_copyable_v<From> &&
	std::is_trivially_copyable_v<To>,
	To>
// constexpr support needs compiler magic
bit_cast(const From& src) noexcept
{
	static_assert(std::is_trivially_constructible_v<To>,
		"This implementation additionally requires "
		"destination type to be trivially constructible");
 
	To dst;
	std::memcpy(&dst, &src, sizeof(To));
	return dst;
}

class action_s {
public:
	// Default constructor constructs null action (ie, implicit bool conversion returns false)
	action_s() = default;

	// Allow implicit conversion from schfunc_t to action_s
	action_s(schfunc_t callback) : action_s(callback, nullptr) { }
	action_s(schfunc_t callback, void *param) : m_callback(callback), m_param(param) { }

	// Allow any function that takes a single pointer parameter, so long as param is also of the same pointer type.
	// This constructor means you shouldn't ever have to cast to schfunc_t on your own.
	template <typename TArg>
	action_s(void (*callback)(TArg*), TArg* param) : m_callback((schfunc_t)callback), m_param(param) { }
	template <typename TArg>
	action_s(void (*callback)(TArg), TArg param) : m_callback(bit_cast<schfunc_t>(callback)), m_param(reinterpret_cast<void*>(param)) { }

	void execute();
	schfunc_t getCallback() const;
	void * getArgument() const;

	// Actions with a callback set are truthy, all others are falsy
	operator bool() const {
		return m_callback != nullptr;
	}

private:
	schfunc_t m_callback = nullptr;
	void *m_param = nullptr;
};

/**
 * This structure holds information about an event scheduled in the future: when to execute what callback with what parameters
 */
#pragma pack(push, 4)
struct scheduling_s {
#if EFI_SIGNAL_EXECUTOR_SLEEP
	virtual_timer_t timer;
#endif /* EFI_SIGNAL_EXECUTOR_SLEEP */

	// timestamp represented as 64-bit value of ticks since MCU start
	efitick_t momentX;

	// Scheduler implementation uses a sorted linked list of these scheduling records.
	scheduling_s *nextScheduling_s = nullptr;

	action_s action;
};
#pragma pack(pop)

struct Scheduler {
	/**
	 * @brief Schedule an action to be executed in the future.
	 * 
	 * scheduleByAngle is useful if you want to schedule something in terms of crank angle instead of time.
	 *
	 * @param msg Name of this event to use for logging in case of an error.
	 * @param scheduling Storage to use for the scheduled event. If null, one will be used from the pool.
	 * @param targetTime When to execute the specified action. If this time is in the past or
	 *                   very near future, it may execute immediately.
	 * @param action An action to execute at the specified time.
	 */
	virtual void schedule(const char *msg, scheduling_s *scheduling, efitick_t targetTime, action_s action) = 0;

	/**
	 * @brief Cancel the specified scheduling_s so that, if currently scheduled, it does not execute.
	 * 
	 * @param scheduling The scheduling_s to cancel.
	 */
	virtual void cancel(scheduling_s* scheduling) = 0;
};

Scheduler *getScheduler();

