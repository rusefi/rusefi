/**
 * @file resource_protector.h
 *
 * @brief Reader-style access guard for a shared resource whose lifetime is managed
 * by a single coordinator thread.
 *
 * ProtectedResource<T> holds a pointer to an object of type T together with a mutex,
 * a condition variable and a user counter. Any number of "user" threads may access
 * the resource concurrently via enter()/leave() while it is open. A single
 * "coordinator" thread can close the resource with free(): new users are rejected
 * immediately and the call blocks until every current user has left (or a timeout
 * expires), after which the coordinator receives the detached pointer (or nullptr
 * on timeout), may safely tear down / replace the underlying object and later
 * supply a (new) instance with open().
 *
 * Typical use case: a mounted filesystem (e.g. SD card FATFS) that worker threads
 * write to, while the SD state machine needs to unmount it safely.
 *
 * Notes:
 * - The resource starts in the CLOSED state with no object attached; call open()
 *   with a valid pointer once it is ready.
 * - enter()/leave() calls must be strictly balanced by each user thread.
 * - free() returns nullptr on timeout, leaving the resource in the CLOSING state.
 * - This is not a recursive lock and provides no protection between users:
 *   T itself must be safe for concurrent use by multiple holders, or callers
 *   must serialize access by other means.
 */

#pragma once

#include "ch.hpp"

template <typename T>
class ProtectedResource {
private:
	chibios_rt::Mutex             mutex;
	// Use the core C structure instead of the protected chibios_rt::Monitor wrapper
	condition_variable_t          cond_var;
	int                           user_count;
	bool                          is_closing;
	bool                          is_forced_closed;
	T*                            resource;

public:
	/**
	 * @brief Constructs the wrapper in the CLOSED state with no object attached.
	 *
	 * The coordinator must call open() with a valid pointer before users can
	 * successfully enter().
	 */
	ProtectedResource()
		: user_count(0), is_closing(true), is_forced_closed(false), resource(nullptr) {
			chCondObjectInit(&cond_var);
		}

	/**
	 * @brief Constructs the wrapper in the OPEN state with @p initial_resource attached.
	 *
	 * Users may enter() right away without a prior open() call. Passing nullptr
	 * still opens the resource, but enter() will refuse access until a valid
	 * pointer is attached with open().
	 *
	 * @param initial_resource pointer to the object to protect from the start
	 */
	explicit ProtectedResource(T* initial_resource)
		: user_count(0), is_closing(false), is_forced_closed(false), resource(initial_resource) {
			chCondObjectInit(&cond_var);
		}

	/**
	 * @brief Acquire shared access to the resource (user threads).
	 *
	 * Never blocks: access is refused if the resource is closing, force-closed
	 * or no object is attached. On success the caller is counted as an active
	 * user and MUST call leave() when done.
	 *
	 * @return pointer to the protected object on success, nullptr otherwise
	 */
	T* enter() {
		chibios_rt::MutexLocker lock(mutex);
		if (is_closing || is_forced_closed || resource == nullptr) {
			return nullptr;
		}
		user_count++;
		return resource;
	}

	/**
	 * @brief Release shared access previously obtained with enter() (user threads).
	 *
	 * When the last user leaves, a coordinator blocked in free() is woken up
	 * (unless the resource was already force-closed by a timeout).
	 * Must be called exactly once per successful enter().
	 */
	void leave() {
		chibios_rt::MutexLocker lock(mutex);

		if (user_count > 0) {
			user_count--;
		}

		// Standard exit path: alert coordinator if it is waiting
		if (user_count == 0 && !is_forced_closed) {
			chCondSignal(&cond_var);
		}
	}

	/**
	 * @brief Close the resource, wait for users to leave and detach the object (coordinator).
	 *
	 * Immediately marks the resource as closing so any further enter() calls fail,
	 * then blocks until the user count drops to zero or the timeout expires.
	 *
	 * @param timeout maximum time to wait for users to leave
	 * @return the detached object pointer on success, or nullptr on timeout
	 */
	T* free(sysinterval_t timeout) {
		chibios_rt::MutexLocker lock(mutex);

		is_closing = true;

		sysinterval_t deadline = chVTGetSystemTimeX() + timeout;

		while (user_count > 0) {
			sysinterval_t remaining = chTimeDiffX(chVTGetSystemTimeX(), deadline);

			// TIMEOUT ENCOUNTERED HERE
			if (remaining <= 0) {
				return nullptr;
			}

			if (chCondWaitTimeoutS(&cond_var, remaining) != MSG_OK) {
				return nullptr;
			}
		}

		// --- SAFE ZONE / FORCE ZONE ---
		T* temp = resource;
		resource = nullptr; // Immediately clear reference so incoming users get nullptr
		return temp;        // Return the pointer for deletion/deinit
	}

	/**
	 * @brief Close the resource and detach the object immediately without waiting (coordinator).
	 *
	 * Unlike free(), this does not block. The internal reference is cleared
	 * immediately, so subsequent enter() calls return nullptr. Existing users
	 * still holding the resource must still call leave().
	 *
	 * @return the detached object pointer (nullptr if none was attached)
	 */
	T* close() {
		chibios_rt::MutexLocker lock(mutex);

		// --- SAFE ZONE / FORCE ZONE ---
		T* temp = resource;
		resource = nullptr; // Immediately clear reference so incoming users get nullptr
		return temp;        // Return the pointer for deletion/deinit
	}

	/**
	 * @brief Thread-safe inspection helper to read the current user count.
	 *
	 * @return current number of active users
	 */
	int get_user_count() {
		chibios_rt::MutexLocker lock(mutex);

		return user_count;
	}

	/**
	 * @brief Attach a (new) object and make the resource available to users (coordinator).
	 *
	 * Resets the user counter and the closing/force-closed flags, so subsequent
	 * enter() calls succeed and return @p new_resource.
	 *
	 * @param new_resource pointer to the object to protect from now on
	 */
	void open(T* new_resource) {
		chibios_rt::MutexLocker lock(mutex);
		user_count = 0;
		is_forced_closed = false;
		is_closing = false;
		resource = new_resource; // Assign the new object pointer
	}
};
