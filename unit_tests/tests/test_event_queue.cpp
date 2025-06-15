/**
 * @file	test_signal_executor.cpp
 *
 * @date Nov 28, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "event_queue.h"

static int callbackCounter = 0;

static void callback() {
	callbackCounter++;
}

namespace {
	auto const callbackAction{ action_s::make<callback>() };
}

static int complexTestNow;

struct TestPwm {
	TestPwm(EventQueue *eventQueue) {
		this->eventQueue = eventQueue;
	}
	scheduling_s s;
	int period;
	EventQueue *eventQueue;
};

static void complexCallback(TestPwm *testPwm) {
	callbackCounter++;

	testPwm->eventQueue->insertTask(&testPwm->s, complexTestNow + testPwm->period,
			action_s::make<complexCallback>(testPwm));
}

TEST(EventQueue, testSignalExecutor2) {
	EventQueue eq;
	TestPwm p1(&eq);
	TestPwm p2(&eq);
	p1.period = 2;
	p2.period = 3;

	complexTestNow = 0;
	callbackCounter = 0;
	eq.insertTask(&p1.s, 0, action_s::make<complexCallback>(&p1));
	eq.insertTask(&p2.s, 0, action_s::make<complexCallback>(&p2));
	eq.executeAll(complexTestNow);
	ASSERT_EQ( 2,  callbackCounter) << "callbackCounter #1";
	ASSERT_EQ(2, eq.size());

	eq.executeAll(complexTestNow = 2);
	ASSERT_EQ( 3,  callbackCounter) << "callbackCounter #2";
	ASSERT_EQ(2, eq.size());

	eq.executeAll(complexTestNow = 3);
	ASSERT_EQ( 4,  callbackCounter) << "callbackCounter #3";
	ASSERT_EQ(2, eq.size());

}

static uintptr_t prevValue = 0;

static void orderCallback(uintptr_t a) {
	uintptr_t value = a;

	printf("value=%d prevValue=%d\r\n", value, prevValue);
	ASSERT_TRUE(value > prevValue) << "orderCallback";

	prevValue = value;
}

TEST(EventQueue, simple) {
	EventQueue eq;

	scheduling_s s1;
	scheduling_s s2;
	scheduling_s s3;

	eq.insertTask(&s1, 10, action_s::make<orderCallback>(uintptr_t{1}));
	eq.insertTask(&s2, 11, action_s::make<orderCallback>(uintptr_t{2}));
	eq.insertTask(&s3, 12, action_s::make<orderCallback>(uintptr_t{3}));
	eq.executeAll(100);
}

TEST(EventQueue, complex) {
	EventQueue eq;
	ASSERT_EQ(eq.getNextEventTime(0), unexpected);
	scheduling_s s1;
	scheduling_s s2;
	scheduling_s s3;
	scheduling_s s4;

	eq.insertTask(&s1, 10, callbackAction);
	eq.insertTask(&s4, 10, callbackAction);
	eq.insertTask(&s3, 12, callbackAction);
	eq.insertTask(&s2, 11, callbackAction);

	ASSERT_EQ(4, eq.size());
	ASSERT_EQ(10, eq.getHead()->getMomentNt());
	ASSERT_EQ(10, eq.getHead()->nextScheduling_s->getMomentNt());
	ASSERT_EQ(11, eq.getHead()->nextScheduling_s->nextScheduling_s->getMomentNt());
	ASSERT_EQ(12, eq.getHead()->nextScheduling_s->nextScheduling_s->nextScheduling_s->getMomentNt());

	callbackCounter = 0;
	eq.executeAll(10);
	ASSERT_EQ( 2,  callbackCounter) << "callbackCounter/2";
	callbackCounter = 0;
	eq.executeAll(11);
	ASSERT_EQ( 1,  callbackCounter) << "callbackCounter/1#1";
	eq.executeAll(100);
	ASSERT_EQ(0, eq.size());

	eq.insertTask(&s1, 12, callbackAction);
	eq.insertTask(&s2, 11, callbackAction);
	eq.insertTask(&s3, 10, callbackAction);
	callbackCounter = 0;
	eq.executeAll(10);
	ASSERT_EQ( 1,  callbackCounter) << "callbackCounter/1#2";
	callbackCounter = 0;
	eq.executeAll(11);
	ASSERT_EQ(1, callbackCounter);
	eq.executeAll(100);
	ASSERT_EQ(0, eq.size());

	callbackCounter = 0;
	eq.insertTask(&s1, 10, callbackAction);
	ASSERT_EQ(10, eq.getNextEventTime(0).value_or(-1));

	eq.executeAll(1);
	ASSERT_EQ( 0,  callbackCounter) << "callbacks not expected";

	eq.executeAll(11);
	ASSERT_EQ(1, callbackCounter);

	ASSERT_EQ(eq.getNextEventTime(0), unexpected);

	eq.insertTask(&s1, 10, callbackAction);
	eq.insertTask(&s2, 13, callbackAction);
	ASSERT_EQ(10, eq.getNextEventTime(0).value_or(-1));

	eq.executeAll(1);
	ASSERT_EQ(10, eq.getNextEventTime(0).value_or(-1));

	eq.executeAll(100);
	ASSERT_EQ(0, eq.size());
	callbackCounter = 0;
	// both events are scheduled for the same time
	eq.insertTask(&s1, 10, callbackAction);
	eq.insertTask(&s2, 10, callbackAction);

	eq.executeAll(11);

	ASSERT_EQ(2, callbackCounter);
}

class EventQueueRemoveTest : public ::testing::Test {
protected:
	EventQueue dut;
	scheduling_s s1, s2, s3;

	void SetUp() override {
		dut.insertTask(&s1, 100, callbackAction);
		dut.insertTask(&s2, 200, callbackAction);
		dut.insertTask(&s3, 300, callbackAction);

		// Check that things are assembled as we think
		ASSERT_EQ(&s1, dut.getElementAtIndexForUnitText(0));
		ASSERT_EQ(&s2, dut.getElementAtIndexForUnitText(1));
		ASSERT_EQ(&s3, dut.getElementAtIndexForUnitText(2));
		ASSERT_EQ(nullptr, dut.getElementAtIndexForUnitText(3));
	}
};

TEST_F(EventQueueRemoveTest, removeHead) {
	// Remove the element at the head
	dut.remove(&s1);

	// Check that it's gone
	ASSERT_EQ(&s2, dut.getElementAtIndexForUnitText(0));
	ASSERT_EQ(&s3, dut.getElementAtIndexForUnitText(1));
	ASSERT_EQ(nullptr, dut.getElementAtIndexForUnitText(2));
}

TEST_F(EventQueueRemoveTest, removeMiddle) {
	// Remove the element in the middle
	dut.remove(&s2);

	// Check that it's gone
	ASSERT_EQ(&s1, dut.getElementAtIndexForUnitText(0));
	ASSERT_EQ(&s3, dut.getElementAtIndexForUnitText(1));
	ASSERT_EQ(nullptr, dut.getElementAtIndexForUnitText(2));
}

TEST_F(EventQueueRemoveTest, removeEnd) {
	// Remove the element at the end
	dut.remove(&s3);

	// Check that it's gone
	ASSERT_EQ(&s1, dut.getElementAtIndexForUnitText(0));
	ASSERT_EQ(&s2, dut.getElementAtIndexForUnitText(1));
	ASSERT_EQ(nullptr, dut.getElementAtIndexForUnitText(2));
}

TEST_F(EventQueueRemoveTest, removeNotPresent) {
	scheduling_s s4;

	// Remove an element not already in the list - shouldn't fail
	EXPECT_NO_THROW(dut.remove(&s4));

	// Check that the list didn't change
	ASSERT_EQ(&s1, dut.getElementAtIndexForUnitText(0));
	ASSERT_EQ(&s2, dut.getElementAtIndexForUnitText(1));
	ASSERT_EQ(&s3, dut.getElementAtIndexForUnitText(2));
	ASSERT_EQ(nullptr, dut.getElementAtIndexForUnitText(3));
}
