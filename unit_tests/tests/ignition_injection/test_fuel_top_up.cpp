/**
 * @file test_fuel_top_up.cpp
 *
 * [tag:fuel_top_up] Top-up pulse: window bound to the main pulse, late decision,
 * extension vs separate pulse, injecting what fits, gates and the safety invariants
 * documented in fuel_top_up.cpp.
 */

#include "pch.h"
#include "injector_model.h"
#include "defaults.h"
#include <random>

static void testCloseInjector(InjectorOutputPin* pin) {
	pin->close(getTimeNowNt());
}

class FuelTopUp : public ::testing::Test {
protected:
	EngineTestHelper eth{engine_type_e::TEST_ENGINE};
	InjectionEvent* event = nullptr;
	// Fuel mass per ms of injector flow, from the configured injector.
	float perMs = 0;
	// Cylinder 1: its main pulse opens at engine phase 320 (-400 deg after its
	// compression TDC) and the default limit, -140 deg, is engine phase 580.
	static constexpr float MainStartPhase = 320;
	static constexpr float LimitPhase = 580;
	static constexpr float PhaseInWindow = 480;

	void SetUp() override {
		engineConfiguration->injectionMode = IM_SEQUENTIAL;
		engineConfiguration->isInjectionEnabled = true;
		setDefaultFuelTopUp();
		config->fuelTopUpMode = FUEL_TOP_UP_INJECT;
		engine->rpmCalculator.setRpmValue(1500);
		ASSERT_FALSE(engine->rpmCalculator.isCranking());
		engine->module<InjectorModelPrimary>()->prepare();
		perMs = engine->module<InjectorModelPrimary>()->getFuelMassForDuration(1);
		ASSERT_GT(perMs, 0);

		event = &engine->injectionEvents.elements[0];
		ASSERT_EQ(0, event->getIndex());
		// The next main pulse of this cylinder opens one cycle after this one.
		event->injectionStartAngle = MainStartPhase;
		getEngineState()->shouldUpdateInjectionTiming = true;
		state().fuelTopUpCount = 0;
		state().fuelTopUpExtensionCount = 0;
		state().fuelTopUpMoveCount = 0;
		state().fuelTopUpFault = false;
	}

	TpsAccelEnrichment& state() {
		return engine->module<TpsAccelEnrichment>().unmock();
	}

	InjectorOutputPin& pin() {
		return enginePins.injectors[0];
	}

	void setRequested(float grams) {
		getEngineState()->injectionMass[0] = grams;
	}

	float oneDegreeUs() {
		return getEngineRotationState()->getOneDegreeUs();
	}

	// A main pulse delivering `mass`, open from now+startUs to now+closeUs, which
	// opened at engine phase startPhase.
	void registerMain(float mass, int startUs, int closeUs, float startPhase = MainStartPhase) {
		efitick_t nowNt = getTimeNowNt();
		event->registerMainPulse(mass, startPhase, nowNt + US2NT(startUs), nowNt + US2NT(closeUs));
		setRequested(mass);
	}

	// Simulates an open main pulse on cylinder 1 closing after closeUs.
	void openMainPulse(int closeUs, InjectorOutputPin& injector) {
		injector.open(getTimeNowNt());
		getScheduler()->schedule("test main close", nullptr, getTimeNowNt() + US2NT(closeUs),
			action_s::make<testCloseInjector>(&injector));
	}

	FuelTopUpSkip lastSkip() {
		return static_cast<FuelTopUpSkip>(state().fuelTopUpSkipReason);
	}

	// A trigger tooth at `phase`; the next tooth comes toothDeg later.
	void evaluate(float phase = PhaseInWindow, float toothDeg = 6) {
		event->evaluateTopUp(getTimeNowNt(), phase, phase + toothDeg);
	}

	// A tooth with no further tooth before the limit: the decision cannot be deferred.
	void evaluateLastTooth(float phase = PhaseInWindow) {
		evaluate(phase, LimitPhase - phase);
	}

	// Teeth every toothDeg from `phase` with time running at the current RPM, until a
	// top-up is scheduled or the limit passes. setRequestForTooth(i) sets the request
	// before tooth i. Returns the phase of the tooth that scheduled the top-up, or NAN.
	template<typename SetRequest>
	float spinTeeth(float phase, float toothDeg, SetRequest setRequestForTooth) {
		int before = state().fuelTopUpCount;
		for (int i = 0; phase < LimitPhase + toothDeg; i++, phase += toothDeg) {
			setRequestForTooth(i);
			evaluate(phase, toothDeg);
			if (state().fuelTopUpCount != before) {
				return phase;
			}
			eth.moveTimeForwardAndInvokeEventsUs(static_cast<int>(toothDeg * oneDegreeUs()));
		}
		return NAN;
	}
};

TEST_F(FuelTopUp, ReopensAfterClosedMainPulse) {
	float missing = 2 * perMs; // 2 ms of flow
	registerMain(4 * missing, -5000, -3000);
	setRequested(5 * missing);
	evaluateLastTooth();

	EXPECT_EQ(1, state().fuelTopUpCount);
	EXPECT_EQ(0, state().fuelTopUpExtensionCount);
	EXPECT_EQ(FuelTopUpSkip::None, lastSkip());
	EXPECT_FALSE(event->m_topUpPending);
	EXPECT_NEAR(5 * missing, event->m_registeredMass, 1e-9f);
	EXPECT_NEAR(missing * 1000, state().fuelTopUpLastMass, 1e-4f);
	float pulseMs = engine->module<InjectorModelPrimary>()->getInjectionDuration(missing);
	EXPECT_NEAR(pulseMs, state().fuelTopUpLastPulse, 1e-4f);

	eth.moveTimeForwardAndInvokeEventsUs(10);
	EXPECT_TRUE(pin().currentLogicValue);
	EXPECT_EQ(1, pin().getOverlappingCounter());
	eth.moveTimeForwardAndInvokeEventsUs(static_cast<int>(MS2US(pulseMs)) - 100);
	EXPECT_TRUE(pin().currentLogicValue);
	eth.moveTimeForwardAndInvokeEventsUs(200);
	EXPECT_FALSE(pin().currentLogicValue);
	EXPECT_EQ(0, pin().getOverlappingCounter());

	// The top-up close does not move the next main pulse (InjectionEvent::update()).
	EXPECT_FLOAT_EQ(MainStartPhase, event->injectionStartAngle);

	// Only once per main pulse.
	setRequested(10 * missing);
	evaluateLastTooth();
	EXPECT_EQ(1, state().fuelTopUpCount);
}

TEST_F(FuelTopUp, SeparatePulseWaitsForMinimumOffTime) {
	float missing = 2 * perMs;
	registerMain(4 * missing, -2000, -300); // closed 0.3 ms ago, off time 0.8 ms
	setRequested(5 * missing);
	evaluateLastTooth();
	ASSERT_EQ(1, state().fuelTopUpCount);

	eth.moveTimeForwardAndInvokeEventsUs(400);
	EXPECT_FALSE(pin().currentLogicValue);
	eth.moveTimeForwardAndInvokeEventsUs(200);
	EXPECT_TRUE(pin().currentLogicValue);
	eth.moveTimeForwardAndInvokeEventsUs(20000);
	EXPECT_FALSE(pin().currentLogicValue);
}

TEST_F(FuelTopUp, ExtendsOpenMainPulseWithoutDeadTime) {
	float missing = 1 * perMs; // 1 ms of flow, no dead time when extending
	registerMain(4 * missing, -1000, 2000);
	openMainPulse(2000, pin());
	setRequested(5 * missing);
	evaluateLastTooth();

	EXPECT_EQ(1, state().fuelTopUpCount);
	EXPECT_EQ(1, state().fuelTopUpExtensionCount);
	EXPECT_NEAR(1, state().fuelTopUpLastPulse, 1e-4f);

	eth.moveTimeForwardAndInvokeEventsUs(10);
	EXPECT_EQ(2, pin().getOverlappingCounter());
	// The main close leaves the injector open.
	eth.moveTimeForwardAndInvokeEventsUs(2100);
	EXPECT_TRUE(pin().currentLogicValue);
	EXPECT_EQ(1, pin().getOverlappingCounter());
	// Closed 1 ms after the main close.
	eth.moveTimeForwardAndInvokeEventsUs(800);
	EXPECT_TRUE(pin().currentLogicValue);
	eth.moveTimeForwardAndInvokeEventsUs(200);
	EXPECT_FALSE(pin().currentLogicValue);
	EXPECT_EQ(0, pin().getOverlappingCounter());
}

TEST_F(FuelTopUp, ExtensionDisabledWaitsForMainClose) {
	config->fuelTopUpExtendOpen = false;
	float missing = 2 * perMs;
	registerMain(4 * missing, -1000, 2000);
	openMainPulse(2000, pin());
	setRequested(5 * missing);
	evaluateLastTooth();
	EXPECT_EQ(0, state().fuelTopUpExtensionCount);
	ASSERT_EQ(1, state().fuelTopUpCount);

	eth.moveTimeForwardAndInvokeEventsUs(2100);
	EXPECT_FALSE(pin().currentLogicValue); // main closed, off time running
	eth.moveTimeForwardAndInvokeEventsUs(800);
	EXPECT_TRUE(pin().currentLogicValue); // top-up after main close + 0.8 ms
	eth.moveTimeForwardAndInvokeEventsUs(20000);
	EXPECT_FALSE(pin().currentLogicValue);
	EXPECT_EQ(0, pin().getOverlappingCounter());
}

TEST_F(FuelTopUp, NothingBeforeTheMainPulseOrWithoutMissingFuel) {
	float missing = 2 * perMs;
	// The main pulse has not opened yet: the window is not open.
	registerMain(4 * missing, 1000, 3000);
	setRequested(5 * missing);
	evaluateLastTooth();
	EXPECT_EQ(0, state().fuelTopUpCount);
	EXPECT_TRUE(event->m_topUpPending);

	registerMain(4 * missing, -5000, -3000);
	setRequested(4 * missing); // nothing missing
	evaluateLastTooth();
	EXPECT_EQ(0, state().fuelTopUpCount);
	EXPECT_TRUE(event->m_topUpPending);
}

// The window belongs to the registered main pulse and ends at the first limit after
// it. A request that rises later belongs to the next main pulse, which already uses it:
// a top-up then would add the same fuel twice.
TEST_F(FuelTopUp, WindowEndsAtTheLimitAfterItsMainPulse) {
	float missing = 2 * perMs;
	registerMain(4 * missing, -5000, -3000);
	setRequested(5 * missing);
	evaluate(LimitPhase + 20);
	EXPECT_FALSE(event->m_topUpPending);
	evaluateLastTooth(); // later teeth no longer act
	EXPECT_EQ(0, state().fuelTopUpCount);

	// Also when no tooth fell between the limit and the next main pulse.
	registerMain(4 * missing, -5000, -3000);
	setRequested(5 * missing);
	evaluate(MainStartPhase - 10);
	EXPECT_FALSE(event->m_topUpPending);
	EXPECT_EQ(0, state().fuelTopUpCount);
}

TEST_F(FuelTopUp, MainPulseAfterIntakeValveClosingFeedsTheNextIntake) {
	// The main pulse opens at engine phase 600, after this cycle's limit (580): its fuel
	// waits in the port for the next intake stroke, which ends at 580 one cycle later.
	float missing = 2 * perMs;
	event->injectionStartAngle = 600;
	registerMain(4 * missing, -5000, -3000, 600);
	setRequested(5 * missing);
	evaluate(100); // 220 deg after the main pulse opened
	EXPECT_TRUE(event->m_topUpPending);
	EXPECT_EQ(FuelTopUpSkip::Waiting, lastSkip());
	evaluateLastTooth(500);
	EXPECT_EQ(1, state().fuelTopUpCount);
}

TEST_F(FuelTopUp, LimitAndNextMainPulseBoundThePulse) {
	float missing = 2 * perMs;
	registerMain(4 * missing, -5000, -3000);
	setRequested(5 * missing);
	// 5 deg before the limit: not even a minimum pulse fits at 1500 RPM.
	evaluateLastTooth(575);
	EXPECT_EQ(0, state().fuelTopUpCount);
	EXPECT_EQ(FuelTopUpSkip::PastLimit, lastSkip());
	EXPECT_FALSE(event->m_topUpPending);

	// The next main pulse of this cylinder starts 5 deg after now.
	registerMain(4 * missing, -5000, -3000);
	setRequested(5 * missing);
	event->injectionStartAngle = PhaseInWindow + 5;
	evaluateLastTooth();
	EXPECT_EQ(0, state().fuelTopUpCount);
	EXPECT_EQ(FuelTopUpSkip::PastLimit, lastSkip());
}

TEST_F(FuelTopUp, CapsAndMinimums) {
	float missing = 2 * perMs;
	// 50% cap of the main pulse.
	registerMain(2 * missing, -5000, -3000);
	setRequested(10 * missing);
	evaluateLastTooth();
	ASSERT_EQ(1, state().fuelTopUpCount);
	EXPECT_NEAR(missing * 1000, state().fuelTopUpLastMass, 1e-4f);
	eth.moveTimeForwardAndInvokeEventsUs(50000);

	// Below the minimum mass: kept pending, injected once the request grows.
	config->fuelTopUpMinMass = 750 * missing; // 0.75 of `missing`, in mg
	registerMain(4 * missing, -5000, -3000);
	setRequested(4.5f * missing);
	evaluateLastTooth();
	EXPECT_EQ(FuelTopUpSkip::BelowMinimum, lastSkip());
	EXPECT_TRUE(event->m_topUpPending);
	setRequested(5 * missing);
	evaluateLastTooth();
	EXPECT_EQ(2, state().fuelTopUpCount);
	eth.moveTimeForwardAndInvokeEventsUs(50000);

	// Flow time below the minimum pulse.
	config->fuelTopUpMinMass = 0;
	config->fuelTopUpMinPulse = 3;
	registerMain(4 * missing, -5000, -3000);
	setRequested(5 * missing);
	evaluateLastTooth();
	EXPECT_EQ(FuelTopUpSkip::BelowMinimum, lastSkip());
	EXPECT_EQ(2, state().fuelTopUpCount);
}

TEST_F(FuelTopUp, Gates) {
	float missing = 2 * perMs;
	auto check = [&](FuelTopUpSkip expected) {
		registerMain(4 * missing, -5000, -3000);
		setRequested(5 * missing);
		evaluate();
		EXPECT_EQ(expected, lastSkip());
		EXPECT_EQ(0, state().fuelTopUpCount);
	};

	config->fuelTopUpMaxRpm = 1000;
	check(FuelTopUpSkip::RpmTooHigh);
	config->fuelTopUpMaxRpm = 4000;

	getEngineState()->shouldUpdateInjectionTiming = false;
	check(FuelTopUpSkip::DutyTooHigh);
	getEngineState()->shouldUpdateInjectionTiming = true;

	state().fuelTopUpFault = true;
	check(FuelTopUpSkip::Fault);
	state().fuelTopUpFault = false;

	// A limit at the main pulse start leaves no window.
	setArrayValues(config->fuelTopUpLimitAngle, -400);
	registerMain(4 * missing, -5000, -3000);
	setRequested(5 * missing);
	evaluate();
	EXPECT_FALSE(event->m_topUpPending);
	EXPECT_EQ(0, state().fuelTopUpCount);
	setArrayValues(config->fuelTopUpLimitAngle, -140);

	config->fuelTopUpMode = FUEL_TOP_UP_OFF;
	registerMain(4 * missing, -5000, -3000);
	EXPECT_FALSE(event->m_topUpPending);
}

TEST_F(FuelTopUp, NotSequentialAndCranking) {
	float missing = 2 * perMs;
	engineConfiguration->injectionMode = IM_BATCH;
	ASSERT_TRUE(event->update());
	event->injectionStartAngle = 300;
	registerMain(4 * missing, -5000, -3000);
	setRequested(5 * missing);
	evaluate();
	EXPECT_EQ(FuelTopUpSkip::NotSequential, lastSkip());

	// Cranking with sequential cranking injection, so only the cranking gate applies.
	engineConfiguration->injectionMode = IM_SEQUENTIAL;
	engineConfiguration->crankingInjectionMode = IM_SEQUENTIAL;
	ASSERT_TRUE(event->update());
	event->injectionStartAngle = 300;
	engine->rpmCalculator.setRpmValue(0);
	engine->rpmCalculator.setRpmValue(200);
	ASSERT_TRUE(engine->rpmCalculator.isCranking());
	registerMain(4 * missing, -5000, -3000);
	setRequested(5 * missing);
	evaluate();
	EXPECT_EQ(FuelTopUpSkip::Cranking, lastSkip());
	EXPECT_EQ(0, state().fuelTopUpCount);
}

TEST_F(FuelTopUp, LogOnlyReportsWithoutInjecting) {
	config->fuelTopUpMode = FUEL_TOP_UP_LOG_ONLY;
	float missing = 2 * perMs;
	registerMain(4 * missing, -5000, -3000);
	setRequested(5 * missing);
	evaluateLastTooth();
	EXPECT_TRUE(state().fuelTopUpWouldInject);
	EXPECT_NEAR(missing * 1000, state().fuelTopUpLastMass, 1e-4f);
	EXPECT_EQ(0, state().fuelTopUpCount);
	EXPECT_FALSE(event->m_topUpPending);
	EXPECT_FALSE(event->m_topUpOpenScheduling.action);
	EXPECT_FALSE(event->m_topUpCloseScheduling.action);
	EXPECT_FALSE(event->m_topUpFollowing);
	eth.moveTimeForwardAndInvokeEventsUs(20000);
	EXPECT_FALSE(pin().currentLogicValue);
}

TEST_F(FuelTopUp, OpenRequiresArmedClose) {
	event->m_topUpMask = 1;
	event->m_topUpIsExtension = false;
	event->onTopUpOpen();
	EXPECT_FALSE(pin().currentLogicValue);
	EXPECT_EQ(0, pin().getOverlappingCounter());
	EXPECT_TRUE(state().fuelTopUpFault);
}

TEST_F(FuelTopUp, SeparatePulseRefusesBusyInjectorAndKeepsItsPulse) {
	float missing = 2 * perMs;
	registerMain(4 * missing, -5000, -3000);
	setRequested(5 * missing);
	evaluateLastTooth();
	ASSERT_EQ(1, state().fuelTopUpCount);

	// Another pulse holds the injector when the top-up is due.
	openMainPulse(5000, pin());
	eth.moveTimeForwardAndInvokeEventsUs(10);
	EXPECT_TRUE(state().fuelTopUpFault);
	EXPECT_EQ(FuelTopUpSkip::Fault, lastSkip());
	EXPECT_EQ(1, pin().getOverlappingCounter());
	// The top-up close was cancelled: the other pulse keeps its full length.
	eth.moveTimeForwardAndInvokeEventsUs(4000);
	EXPECT_TRUE(pin().currentLogicValue);
	eth.moveTimeForwardAndInvokeEventsUs(2000);
	EXPECT_FALSE(pin().currentLogicValue);
	EXPECT_EQ(0, pin().getOverlappingCounter());

	// A latched fault blocks top-up until the engine stops.
	registerMain(4 * missing, -5000, -3000);
	setRequested(5 * missing);
	evaluateLastTooth();
	EXPECT_EQ(FuelTopUpSkip::Fault, lastSkip());
	state().onEngineStop();
	EXPECT_FALSE(state().fuelTopUpFault);
}

TEST_F(FuelTopUp, LateExtensionIsCancelledNotFaulted) {
	float missing = 1 * perMs;
	registerMain(4 * missing, -1000, 2000);
	pin().open(getTimeNowNt());
	setRequested(5 * missing);
	evaluateLastTooth();
	ASSERT_EQ(1, state().fuelTopUpExtensionCount);
	// The main pulse closes before the extension open runs.
	pin().close(getTimeNowNt());
	eth.moveTimeForwardAndInvokeEventsUs(10);
	EXPECT_FALSE(pin().currentLogicValue);
	EXPECT_EQ(FuelTopUpSkip::LateForExtension, lastSkip());
	EXPECT_FALSE(state().fuelTopUpFault);
	eth.moveTimeForwardAndInvokeEventsUs(10000);
	EXPECT_FALSE(pin().currentLogicValue);
	EXPECT_EQ(0, pin().getOverlappingCounter());
}

TEST_F(FuelTopUp, BusyScheduling) {
	float missing = 2 * perMs;
	registerMain(4 * missing, -5000, -3000);
	setRequested(5 * missing);
	evaluateLastTooth();
	ASSERT_EQ(1, state().fuelTopUpCount);
	// A new registration before the previous top-up ran.
	registerMain(4 * missing, -5000, -3000);
	setRequested(5 * missing);
	evaluateLastTooth();
	EXPECT_EQ(FuelTopUpSkip::SchedulerBusy, lastSkip());
	EXPECT_EQ(1, state().fuelTopUpCount);
	eth.moveTimeForwardAndInvokeEventsUs(20000);
	EXPECT_FALSE(pin().currentLogicValue);
}

TEST_F(FuelTopUp, EngineStopForgetsRegistration) {
	registerMain(0.01f, -5000, -3000);
	ASSERT_TRUE(event->m_topUpPending);
	engine->onEngineStopped();
	EXPECT_FALSE(event->m_topUpPending);
	EXPECT_FLOAT_EQ(0, event->m_registeredMass);
}

TEST_F(FuelTopUp, WallFilmGetsOnlyTheDeposit) {
	engineConfiguration->complexWallModel = false;
	engineConfiguration->wwaeTau = 0.4f;
	engineConfiguration->wwaeBeta = 0.3f;
	Sensor::setMockValue(SensorType::Rpm, 1500);
	engine->module<WallFuelController>().unmock().onFastCallback();
	ASSERT_TRUE(engine->module<WallFuelController>()->getEnable());
	float beta = engine->module<WallFuelController>()->getBeta();

	float missing = 2 * perMs;
	auto& wall = event->getWallFuel();
	wall.resetWF();
	int adjustCalls = wall.invocationCounter;
	registerMain(4 * missing, -5000, -3000);
	setRequested(5 * missing);
	evaluateLastTooth();
	ASSERT_EQ(1, state().fuelTopUpCount);
	float commanded = missing / (1 - beta);
	EXPECT_NEAR(commanded * 1000, state().fuelTopUpLastMass, 1e-3f);
	EXPECT_NEAR(beta * commanded, wall.getWallFuel(), 1e-9f);
	// No extra evaporation step.
	EXPECT_EQ(adjustCalls, wall.invocationCounter);
}

// The request keeps rising tooth after tooth: the top-up is deferred to the last tooth
// from which its pulse still fits, and then covers everything missing at that tooth.
TEST_F(FuelTopUp, LateDecisionCoversTheRise) {
	config->fuelTopUpMaxPercent = 200;
	float main = 10 * perMs;
	float risePerTooth = 0.2f * perMs;
	registerMain(main, -5000, -3000);
	float requested = main;
	float firedAt = spinTeeth(PhaseInWindow, 6, [&](int) {
		requested += risePerTooth;
		setRequested(requested);
	});
	ASSERT_FALSE(std::isnan(firedAt));
	EXPECT_EQ(1, state().fuelTopUpCount);
	EXPECT_NEAR((requested - main) * 1000, state().fuelTopUpLastMass, 1e-3f);

	// It fits from this tooth, and would not have fitted (with margin) from the next one.
	float pulseUs = MS2US(state().fuelTopUpLastPulse);
	float usToLimit = (LimitPhase - firedAt) * oneDegreeUs();
	EXPECT_LE(pulseUs, usToLimit);
	EXPECT_GT(6 * oneDegreeUs() + pulseUs * 1.25f, usToLimit);
	EXPECT_GT(firedAt, PhaseInWindow + 30);

	eth.moveTimeForwardAndInvokeEventsUs(static_cast<int>(usToLimit));
	EXPECT_FALSE(pin().currentLogicValue);
	EXPECT_EQ(0, pin().getOverlappingCounter());
}

TEST_F(FuelTopUp, ExtensionDecidedAtTheLastToothBeforeMainClose) {
	float missing = 1 * perMs;
	registerMain(4 * missing, -1000, 3000);
	openMainPulse(3000, pin());
	efitick_t mainCloseNt = event->m_mainCloseNt;
	setRequested(5 * missing);
	float toothUs = 6 * oneDegreeUs();
	float firedAt = spinTeeth(PhaseInWindow, 6, [](int) {});
	ASSERT_FALSE(std::isnan(firedAt));
	ASSERT_EQ(1, state().fuelTopUpExtensionCount);
	// The next tooth would have come within the extension margin of the main close.
	float usToMainClose = NT2USF(mainCloseNt - getTimeNowNt());
	EXPECT_GT(usToMainClose, 200);
	EXPECT_LE(usToMainClose, toothUs + 200);

	// One pulse: the main close leaves the injector open for 1 ms more.
	eth.moveTimeForwardAndInvokeEventsUs(static_cast<int>(usToMainClose) + 100);
	EXPECT_TRUE(pin().currentLogicValue);
	eth.moveTimeForwardAndInvokeEventsUs(1000);
	EXPECT_FALSE(pin().currentLogicValue);
	EXPECT_EQ(0, pin().getOverlappingCounter());
}

TEST_F(FuelTopUp, InjectsWhatFitsBeforeTheLimit) {
	config->fuelTopUpMaxPercent = 200;
	float main = 10 * perMs;
	registerMain(main, -5000, -3000);
	setRequested(main + 8 * perMs); // 8 ms of flow missing
	float phase = LimitPhase - 30;
	float budgetMs = 30 * oneDegreeUs() / 1000;
	evaluateLastTooth(phase);
	ASSERT_EQ(1, state().fuelTopUpCount);
	EXPECT_EQ(FuelTopUpSkip::None, lastSkip());
	EXPECT_LE(state().fuelTopUpLastPulse, budgetMs + 0.001f);
	EXPECT_GT(state().fuelTopUpLastPulse, budgetMs * 0.9f);
	EXPECT_GT(state().fuelTopUpLastMass, 0);
	EXPECT_LT(state().fuelTopUpLastMass, 8 * perMs * 1000);
	// Booked as injected: the registration grows only by what was delivered.
	EXPECT_NEAR(main + state().fuelTopUpLastMass / 1000, event->m_registeredMass, 1e-6f);

	eth.moveTimeForwardAndInvokeEventsUs(static_cast<int>(budgetMs * 1000) + 10);
	EXPECT_FALSE(pin().currentLogicValue);
	EXPECT_EQ(0, pin().getOverlappingCounter());
}

TEST_F(FuelTopUp, ExtensionInjectsWhatFitsBeforeTheLimit) {
	config->fuelTopUpMaxPercent = 200;
	float main = 10 * perMs;
	// The main pulse closes 2 ms from now; the limit is 30 deg (3.3 ms) away.
	registerMain(main, -1000, 2000);
	openMainPulse(2000, pin());
	setRequested(main + 8 * perMs);
	float budgetUs = 30 * oneDegreeUs();
	evaluateLastTooth(LimitPhase - 30);
	ASSERT_EQ(1, state().fuelTopUpExtensionCount);
	EXPECT_NEAR((budgetUs - 2000) / 1000, state().fuelTopUpLastPulse, 0.001f);
	eth.moveTimeForwardAndInvokeEventsUs(static_cast<int>(budgetUs) + 10);
	EXPECT_FALSE(pin().currentLogicValue);
	EXPECT_EQ(0, pin().getOverlappingCounter());
}

// [tag:fuel_top_up] Following the request: while a top-up pulse is scheduled or open,
// a request that keeps rising moves its close later by the plain flow time of the new
// fuel, without a second dead time and without ever leaving the injector unarmed.
TEST_F(FuelTopUp, FollowsRisingRequestWhileOpen) {
	config->fuelTopUpMaxPercent = 200;
	float main = 4 * perMs;
	registerMain(main, -5000, -3000);
	setRequested(main + 2 * perMs);
	float phase = LimitPhase - 60;
	evaluateLastTooth(phase);
	ASSERT_EQ(1, state().fuelTopUpCount);
	ASSERT_TRUE(event->m_topUpFollowing);
	float pulseMs = engine->module<InjectorModelPrimary>()->getInjectionDuration(2 * perMs);

	eth.moveTimeForwardAndInvokeEventsUs(500);
	ASSERT_TRUE(pin().currentLogicValue);
	// Growth below the smallest move waits.
	setRequested(main + 2.01f * perMs);
	evaluate(phase + 6);
	EXPECT_EQ(0, state().fuelTopUpMoveCount);
	// One more ms of flow: the close moves 1 ms later.
	setRequested(main + 3 * perMs);
	evaluate(phase + 12);
	EXPECT_EQ(1, state().fuelTopUpMoveCount);
	EXPECT_EQ(1, state().fuelTopUpCount);
	EXPECT_NEAR(main + 3 * perMs, event->m_registeredMass, 1e-7f);
	EXPECT_NEAR(3 * perMs * 1000, state().fuelTopUpLastMass, 1e-3f);
	EXPECT_NEAR(pulseMs + 1, state().fuelTopUpLastPulse, 1e-3f);

	int closeUs = static_cast<int>(MS2US(pulseMs)) + 1000;
	eth.moveTimeForwardAndInvokeEventsUs(closeUs - 500 - 100);
	EXPECT_TRUE(pin().currentLogicValue);
	EXPECT_EQ(1, pin().getOverlappingCounter());
	eth.moveTimeForwardAndInvokeEventsUs(200);
	EXPECT_FALSE(pin().currentLogicValue);
	EXPECT_EQ(0, pin().getOverlappingCounter());
	EXPECT_FALSE(event->m_topUpFollowing);

	// Closed: a rising request does not reopen the injector.
	setRequested(main + 4 * perMs);
	evaluate(phase + 30);
	eth.moveTimeForwardAndInvokeEventsUs(5000);
	EXPECT_EQ(1, state().fuelTopUpMoveCount);
	EXPECT_FALSE(pin().currentLogicValue);
}

TEST_F(FuelTopUp, MovesTheCloseBeforeThePulseOpens) {
	config->fuelTopUpMaxPercent = 200;
	float main = 4 * perMs;
	// The main pulse closed 0.3 ms ago: the top-up opens 0.5 ms from now.
	registerMain(main, -2000, -300);
	setRequested(main + 2 * perMs);
	evaluateLastTooth();
	ASSERT_EQ(1, state().fuelTopUpCount);
	float pulseMs = engine->module<InjectorModelPrimary>()->getInjectionDuration(2 * perMs);
	setRequested(main + 3 * perMs);
	evaluate(PhaseInWindow + 6);
	EXPECT_EQ(1, state().fuelTopUpMoveCount);

	eth.moveTimeForwardAndInvokeEventsUs(600);
	EXPECT_TRUE(pin().currentLogicValue);
	int closeUs = 500 + static_cast<int>(MS2US(pulseMs)) + 1000;
	eth.moveTimeForwardAndInvokeEventsUs(closeUs - 600 - 100);
	EXPECT_TRUE(pin().currentLogicValue);
	eth.moveTimeForwardAndInvokeEventsUs(200);
	EXPECT_FALSE(pin().currentLogicValue);
	EXPECT_EQ(0, pin().getOverlappingCounter());
}

TEST_F(FuelTopUp, ExtensionFollowsRequestPastMainClose) {
	float missing = 1 * perMs;
	registerMain(4 * missing, -1000, 2000);
	openMainPulse(2000, pin());
	setRequested(5 * missing);
	evaluateLastTooth();
	ASSERT_EQ(1, state().fuelTopUpExtensionCount);

	eth.moveTimeForwardAndInvokeEventsUs(500);
	setRequested(6 * missing);
	evaluate(PhaseInWindow + 6);
	EXPECT_EQ(1, state().fuelTopUpMoveCount);
	// Main close at 2 ms, extension to 3 ms, moved to 4 ms: one continuous pulse.
	eth.moveTimeForwardAndInvokeEventsUs(3300);
	EXPECT_TRUE(pin().currentLogicValue);
	EXPECT_EQ(1, pin().getOverlappingCounter());
	eth.moveTimeForwardAndInvokeEventsUs(300);
	EXPECT_FALSE(pin().currentLogicValue);
	EXPECT_EQ(0, pin().getOverlappingCounter());
}

TEST_F(FuelTopUp, FollowingRespectsTheMaximumPercent) {
	// At most 50 % of the 4 ms main pulse in total: 2 ms of flow.
	float main = 4 * perMs;
	registerMain(main, -5000, -3000);
	setRequested(main + 1.5f * perMs);
	float phase = LimitPhase - 80;
	evaluateLastTooth(phase);
	ASSERT_EQ(1, state().fuelTopUpCount);
	eth.moveTimeForwardAndInvokeEventsUs(200);
	setRequested(main + 3 * perMs);
	evaluate(phase + 6);
	EXPECT_EQ(1, state().fuelTopUpMoveCount);
	EXPECT_NEAR(main + 2 * perMs, event->m_registeredMass, 1e-7f);
	setRequested(main + 4 * perMs);
	evaluate(phase + 12);
	EXPECT_EQ(1, state().fuelTopUpMoveCount);
	eth.moveTimeForwardAndInvokeEventsUs(20000);
	EXPECT_FALSE(pin().currentLogicValue);
	EXPECT_EQ(0, pin().getOverlappingCounter());
}

TEST_F(FuelTopUp, FollowingEndsBeforeTheLimit) {
	config->fuelTopUpMaxPercent = 200;
	float main = 10 * perMs;
	registerMain(main, -5000, -3000);
	setRequested(main + 2 * perMs);
	float phase = LimitPhase - 40;
	evaluateLastTooth(phase);
	ASSERT_EQ(1, state().fuelTopUpCount);
	// Far more than fits: the close moves to the limit, 39 deg after the next tooth.
	setRequested(main + 15 * perMs);
	evaluate(phase + 1);
	EXPECT_EQ(1, state().fuelTopUpMoveCount);
	int budgetUs = static_cast<int>(39 * oneDegreeUs());
	eth.moveTimeForwardAndInvokeEventsUs(budgetUs - 50);
	EXPECT_TRUE(pin().currentLogicValue);
	eth.moveTimeForwardAndInvokeEventsUs(100);
	EXPECT_FALSE(pin().currentLogicValue);
	EXPECT_EQ(0, pin().getOverlappingCounter());
	// Booked as injected: the registration grew only by what fitted.
	EXPECT_LT(event->m_registeredMass, main + 15 * perMs);
}

TEST_F(FuelTopUp, FollowingCanBeDisabled) {
	config->fuelTopUpFollowRequest = false;
	float main = 4 * perMs;
	registerMain(main, -5000, -3000);
	setRequested(main + 2 * perMs);
	evaluateLastTooth(LimitPhase - 60);
	ASSERT_EQ(1, state().fuelTopUpCount);
	EXPECT_FALSE(event->m_topUpFollowing);
	float pulseMs = engine->module<InjectorModelPrimary>()->getInjectionDuration(2 * perMs);
	setRequested(main + 3 * perMs);
	evaluate(LimitPhase - 54);
	EXPECT_EQ(0, state().fuelTopUpMoveCount);
	eth.moveTimeForwardAndInvokeEventsUs(static_cast<int>(MS2US(pulseMs)) + 100);
	EXPECT_FALSE(pin().currentLogicValue);
}

TEST_F(FuelTopUp, NewMainPulseStopsFollowing) {
	float main = 4 * perMs;
	registerMain(main, -5000, -3000);
	setRequested(main + 2 * perMs);
	evaluateLastTooth(LimitPhase - 60);
	ASSERT_TRUE(event->m_topUpFollowing);
	float pulseMs = engine->module<InjectorModelPrimary>()->getInjectionDuration(2 * perMs);
	// The next main pulse registers while the top-up is still open.
	registerMain(main, 5000, 7000);
	EXPECT_FALSE(event->m_topUpFollowing);
	setRequested(main + 3 * perMs);
	evaluate(LimitPhase - 54);
	EXPECT_EQ(0, state().fuelTopUpMoveCount);
	eth.moveTimeForwardAndInvokeEventsUs(static_cast<int>(MS2US(pulseMs)) + 100);
	EXPECT_FALSE(pin().currentLogicValue);
	EXPECT_EQ(0, pin().getOverlappingCounter());
}

TEST(FuelTopUpDefaults, FourPointLimitCurveOverTheWorkingRange) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	ASSERT_EQ(4u, efi::size(config->fuelTopUpRpmBins));
	const int expectedRpm[] = {1000, 2000, 3000, 4000};
	for (size_t i = 0; i < efi::size(config->fuelTopUpRpmBins); i++) {
		EXPECT_EQ(expectedRpm[i], config->fuelTopUpRpmBins[i]) << i;
		EXPECT_EQ(-140, config->fuelTopUpLimitAngle[i]) << i;
	}
	EXPECT_EQ(4000, config->fuelTopUpMaxRpm);
	// Above the last point the curve holds its last value.
	EXPECT_FLOAT_EQ(-140, interpolate2d(6000, config->fuelTopUpRpmBins, config->fuelTopUpLimitAngle));

	// Tunes that predate the setting have an all-zero axis and are seeded, still off.
	setArrayValues(config->fuelTopUpRpmBins, 0);
	setArrayValues(config->fuelTopUpLimitAngle, 0);
	config->fuelTopUpMode = FUEL_TOP_UP_OFF;
	EXPECT_TRUE(applyDefaultsOrFixAfterBurn(nullptr));
	EXPECT_EQ(4000, config->fuelTopUpRpmBins[3]);
	EXPECT_EQ(-140, config->fuelTopUpLimitAngle[0]);
	EXPECT_EQ(FUEL_TOP_UP_OFF, config->fuelTopUpMode);
}

// Random main pulses, requests and timings: whatever the top-up decides, every
// injector ends closed and no counter is left raised.
TEST_F(FuelTopUp, StressNeverLeavesInjectorOpen) {
	std::mt19937 rng(31337);
	std::uniform_real_distribution<float> unit(0, 1);
	float base = 4 * perMs;
	for (int i = 0; i < 3000; i++) {
		config->fuelTopUpExtendOpen = unit(rng) < 0.5f;
		int startUs = -4000 + static_cast<int>(unit(rng) * 5000);
		int closeUs = startUs + 500 + static_cast<int>(unit(rng) * 4000);
		bool mainOpenNow = startUs <= 0 && closeUs > 0;
		registerMain(base, startUs, closeUs);
		if (mainOpenNow && pin().getOverlappingCounter() == 0) {
			openMainPulse(closeUs, pin());
		}
		setRequested(base * (1 + unit(rng)));
		float phase = 330 + unit(rng) * 270;
		evaluate(phase, 1 + unit(rng) * 120);
		if (unit(rng) < 0.5f) {
			// The request keeps rising: a scheduled or open top-up may move its close.
			eth.moveTimeForwardAndInvokeEventsUs(static_cast<int>(unit(rng) * 3000));
			setRequested(base * (1 + 2 * unit(rng)));
			evaluate(phase + unit(rng) * 30, 6);
		}
		eth.moveTimeForwardAndInvokeEventsUs(100 + static_cast<int>(unit(rng) * 6000));
		ASSERT_GE(pin().getOverlappingCounter(), 0);
		if (state().fuelTopUpFault) {
			state().onEngineStop();
		}
	}
	eth.moveTimeForwardAndInvokeEventsUs(100000);
	EXPECT_EQ(0, pin().getOverlappingCounter());
	EXPECT_FALSE(pin().currentLogicValue);
	EXPECT_FALSE(event->m_topUpOpenScheduling.action);
	EXPECT_FALSE(event->m_topUpCloseScheduling.action);
	EXPECT_GT(state().fuelTopUpCount, 0);
	EXPECT_GT(state().fuelTopUpExtensionCount, 0);
	EXPECT_GT(state().fuelTopUpMoveCount, 0);
}

// Whole trigger path: 60-2 wheel, sequential inline-4. A fuel request that rises
// mid-cycle gets top-up pulses for cylinders whose main pulse already ran; a steady
// request gets none.
static int spinWithRequestStep(fuel_top_up_mode_e mode) {
	EngineTestHelper eth(engine_type_e::MINIMAL_PINS);
	Sensor::setMockValue(SensorType::Clt, 80);
	Sensor::setMockValue(SensorType::Iat, 30);
	Sensor::setMockValue(SensorType::Map, 40);
	Sensor::setMockValue(SensorType::Tps1, 10);
	Sensor::setMockValue(SensorType::BatteryVoltage, 13.5f);
	// [tag:lambdaTable] Fuel-pipeline fixtures set the target explicitly.
	setTable(config->lambdaTable, 1.0f);
	EXPECT_CALL(*eth.mockAirmass, getAirmass(testing::_, testing::_))
		.WillRepeatedly(testing::Return(AirmassResult{0.15f, 40.0f}));
	setDefaultFuelTopUp();
	config->fuelTopUpMode = mode;
	config->fuelTopUpMinPulse = 0;
	config->fuelTopUpMinMass = 0;
	config->fuelTopUpMaxPercent = 200;

	auto& ae = engine->module<TpsAccelEnrichment>().unmock();
	ae.fuelTopUpCount = 0;
	testSpinEngineUntilData spinInfo = { 0, 0, 0 };
	// The helper's RPM is double the crank-mounted wheel speed, see test_scheduling_pool.cpp.
	eth.spin60_2UntilDeg(spinInfo, 2 * 1500, 720 * 4);
	EXPECT_NEAR(1500, Sensor::getOrZero(SensorType::Rpm), 20);
	engine->periodicFastCallback();
	// Let every cylinder register a main pulse with this request first.
	eth.spin60_2UntilDeg(spinInfo, 2 * 1500, 720 * 6);
	ae.fuelTopUpCount = 0;
	eth.spin60_2UntilDeg(spinInfo, 2 * 1500, 720 * 8);
	EXPECT_EQ(0, ae.fuelTopUpCount) << "steady request needs no top-up";

	// The request rises mid-cycle.
	eth.spin60_2UntilDeg(spinInfo, 2 * 1500, 720 * 8 + 100);
	EXPECT_CALL(*eth.mockAirmass, getAirmass(testing::_, testing::_))
		.WillRepeatedly(testing::Return(AirmassResult{0.30f, 95.0f}));
	engine->periodicFastCallback();
	eth.spin60_2UntilDeg(spinInfo, 2 * 1500, 720 * 10);

	EXPECT_FALSE(ae.fuelTopUpFault);
	eth.moveTimeForwardAndInvokeEventsUs(100000);
	for (size_t i = 0; i < engineConfiguration->cylindersCount; i++) {
		EXPECT_EQ(0, enginePins.injectors[i].getOverlappingCounter()) << i;
	}
	return ae.fuelTopUpCount;
}

TEST(FuelTopUpIntegration, RequestStepGetsTopUpOnlyWhenEnabled) {
	EXPECT_GT(spinWithRequestStep(FUEL_TOP_UP_INJECT), 0);
	EXPECT_EQ(0, spinWithRequestStep(FUEL_TOP_UP_OFF));
}
