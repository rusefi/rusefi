/**
 * @file	scheduler.h
 *
 * @date May 18, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */
#pragma once

typedef void (*schfunc_t)(void *);

class action_s {
public:
	// Default constructor constructs null action (ie, implicit bool conversion returns false)
	action_s() = default;

	// Allow implicit conversion from schfunc_t to action_s
	action_s(schfunc_t callback) : action_s(callback, nullptr) { }
	action_s(schfunc_t callback, void *param) : callback(callback), param(param) { }

	// Allow any function that takes a single pointer parameter, so long as param is also of the same pointer type.
	// This constructor means you shouldn't ever have to cast to schfunc_t on your own.
	template <typename TArg>
	action_s(void (*callback)(TArg*), TArg* param) : callback((schfunc_t)callback), param(param) { }

	void execute();
	schfunc_t getCallback() const;
	void * getArgument() const;

	operator bool() const {
		return callback != nullptr;
	}

private:
	schfunc_t callback = nullptr;
	void *param = nullptr;
};

/**
 * This structure holds information about an event scheduled in the future: when to execute what callback with what parameters
 */
#pragma pack(push, 4)
struct scheduling_s {
#if EFI_SIGNAL_EXECUTOR_SLEEP
	virtual_timer_t timer;
#endif /* EFI_SIGNAL_EXECUTOR_SLEEP */

	DECLARE_ENGINE_PTR;

	/**
	 * timestamp represented as 64-bit value of ticks since MCU start
	 */
	volatile efitime_t momentX = 0;

	/**
	 * Scheduler implementation uses a sorted linked list of these scheduling records.
	 */
	scheduling_s *nextScheduling_s = nullptr;

	action_s action;
};
#pragma pack(pop)

struct ExecutorInterface {
	/**
	 * see also scheduleByAngle
	 */
	virtual void scheduleByTimestamp(const char *msg, scheduling_s *scheduling, efitimeus_t timeUs, action_s action) = 0;
	virtual void scheduleByTimestampNt(const char *msg, scheduling_s *scheduling, efitime_t timeUs, action_s action) = 0;
	virtual void scheduleForLater(scheduling_s *scheduling, int delayUs, action_s action) = 0;
};
