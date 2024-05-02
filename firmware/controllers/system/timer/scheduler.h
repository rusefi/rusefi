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
  efitick_t getMomentNt() {
    return momentX;
  }

  efitick_t getMomentUs();

// todo: get rid of this 'I am not sure what's the proper type' method once we are done cleaning things up in unit tests
  efitick_t getMomentRaw() {
    return momentX;
  }

  void setMomentX(efitick_t p_moment) {
    momentX = p_moment;
  }

#if EFI_SIMULATOR
  // used by signal_executor_sleep executor implementation
	virtual_timer_t timer;
#endif /* EFI_SIMULATOR */

	/**
	 * Scheduler implementation uses a sorted linked list of these scheduling records.
	 */
	scheduling_s *nextScheduling_s = nullptr;

	action_s action;
	/**
	 * timestamp represented as 64-bit value of ticks since MCU start
	 */
	// actually looks like this is ALWAYS or sometimes us these days?
	// todo: make private with explicit getter/setter?
private:
	volatile efitick_t momentX = 0;
};
#pragma pack(pop)

struct ExecutorInterface {
	/**
	 * see also scheduleByAngle
	 */
	virtual void scheduleByTimestamp(const char *msg, scheduling_s *scheduling, efitimeus_t timeUs, action_s action) = 0;
	virtual void scheduleByTimestampNt(const char *msg, scheduling_s *scheduling, efitick_t timeNt, action_s action) = 0;
	virtual void scheduleForLater(const char *msg, scheduling_s *scheduling, int delayUs, action_s action) = 0;
	virtual void cancel(scheduling_s* scheduling) = 0;
};

ExecutorInterface *getExecutorInterface();

