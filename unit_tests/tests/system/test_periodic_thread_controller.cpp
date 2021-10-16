/*
 * @file test_periodic_thread_controller.cpp
 *
 * May 14, 2021
 * @author Andrey Belomutskiy, (c) 2012-2021
 */

#include "pch.h"

#include "periodic_thread_controller.h"
#include <thread>

class TestPeriodicController : public PeriodicController<111> {
public:
	TestPeriodicController() : PeriodicController("test") { }
private:
	void PeriodicTask(efitick_t nowNt) override	{
		UNUSED(nowNt);
	}
};

static TestPeriodicController instance;

systime_t chVTGetSystemTime(void) {
	return getTimeNowUs();
}

systime_t chThdSleepUntilWindowed(systime_t prev, systime_t next) {
	return 0;
}

bool chThdShouldTerminateX(void) {
	return false;
}

class taskq {
public:
//  int trigger(taskq &tq);
//  mutex mtx;
};


void func_wrapper(taskq &tq) {

}

thread_t *chThdCreateStatic(void *wsp, size_t size,
                            tprio_t prio, tfunc_t funcp, void *arg) {

	taskq insta;

	taskq &tq = insta;
	/**
	 * I do not understand what's going on here. Do allocate thread on stack and that would crash once method is over?
	 * how do we dynamically create new std::thread?
	 */

	std::thread thr(func_wrapper, std::ref(tq));

	thread_t *thread = new thread_t();
	thread->funcp = funcp;
	thread->arg = arg;
	return thread;

}

TEST(system, periodic) {
//	instance.Start();
}
