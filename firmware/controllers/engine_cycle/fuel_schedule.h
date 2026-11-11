/**
 * @file fuel_schedule.h
 */

#pragma once

#include "global.h"
#include "efi_gpio.h"
#include "scheduler.h"
#include "fl_stack.h"
#include "trigger_structure.h"
#include "wall_fuel.h"

// [tag:fuel_top_up] Why the last top-up evaluation did not inject (fuelTopUpSkipReason).
// Keep the numbers in sync with the fuelTopUpHelp text in transient_fueling.ini.
enum class FuelTopUpSkip : uint8_t {
	None = 0,
	NotSequential = 1,
	Cranking = 2,
	RpmTooHigh = 3,
	DutyTooHigh = 4,
	BelowMinimum = 5,
	PastLimit = 6,
	// Fuel is missing and a later tooth still leaves room: the decision is deferred.
	Waiting = 7,
	SchedulerBusy = 8,
	Fault = 9,
	LateForExtension = 10,
};

class InjectionEvent {
public:
	InjectionEvent() = default;

	bool update();

	// Call this every decoded trigger tooth.  It will schedule any relevant events for this injector.
	void onTriggerTooth(efitick_t nowNt, float currentPhase, float nextPhase);

	// [tag:wall_fuel_telemetry] SD log field addresses must be constant expressions in production.
	constexpr WallFuel& getWallFuel() {
		return wallFuel;
	}

	void setIndex(uint8_t index) {
		ownIndex = index;
	}

	CylinderMask calculateInjectorOutputMask() const;
	uint8_t getIndex() const { return ownIndex; }

	// [tag:fuel_top_up] Top-up pulse, see InjectionEvent::evaluateTopUp().
	void evaluateTopUp(efitick_t nowNt, float currentPhase, float nextPhase);
	void onTopUpOpen();
	void onTopUpClose();
	void resetTopUp();

private:
	// Update the injection start angle
	bool updateInjectionAngle();

	// Compute the injection start angle, compensating for injection duration and injection phase settings.
	expected<float> computeInjectionAngle() const;

	/**
	 * This is a performance optimization for IM_SIMULTANEOUS fuel strategy.
	 * It's more efficient to handle all injectors together if that's the case
	 */
	bool isSimultaneous = false;
	uint8_t ownIndex = 0xF;
	uint8_t cylinderNumber = 0;
	injection_mode_e m_injectionMode = IM_SEQUENTIAL;

	WallFuel wallFuel{};

#if EFI_UNIT_TEST
public:
#endif
	// Records the main pulse that a top-up pulse may complement; startAngle is the engine
	// phase at which it opens.
	void registerMainPulse(float massGrams, float startAngle, efitick_t startNt, efitick_t closeNt);
	// Times relative to nowNt, in microseconds.
	bool scheduleTopUp(efitick_t nowNt, float massGrams, float usToLimit, float usToNextStart, float usToNextTooth);
	// Moves a pending top-up close later while the request keeps rising; endBudgetUs is
	// the time from nowNt by which the pulse must have ended.
	bool followTopUp(efitick_t nowNt, float requestedGrams, float endBudgetUs);

	// [tag:fuel_top_up] Pre-film mass of the current main pulse plus any top-up added to it.
	float m_registeredMass = 0;
	// Pre-film mass of the main pulse alone: the base of fuelTopUpMaxPercent.
	float m_mainMass = 0;
	// Close time of the scheduled top-up, moved later while following the request.
	efitick_t m_topUpCloseNt = 0;
	// A top-up is scheduled or open and its close may still be moved later.
	bool m_topUpFollowing = false;
	// The top-up window runs from this main pulse's start to the first limit angle after it.
	float m_mainStartAngle = 0;
	efitick_t m_mainStartNt = 0;
	efitick_t m_mainCloseNt = 0;
	// A main pulse was registered and no top-up was injected for it yet.
	bool m_topUpPending = false;
	bool m_topUpIsExtension = false;
	// The outputs opened by the top-up, so that its close acts on the same pins.
	CylinderMask m_topUpMask = 0;
	// Dedicated storage: top-up events never use the shared scheduling pool.
	scheduling_s m_topUpOpenScheduling;
	scheduling_s m_topUpCloseScheduling;

public:
	float injectionStartAngle = 0;
};

// [tag:injection_action_snapshot] Every action carries the output selection made
// when the pulse was scheduled. A mode change can rebuild InjectionEvent before
// either close action runs.
struct InjectorContext {
	scheduler_arg_t raw = 0;

	static constexpr CylinderMask OutputMask = (1u << MAX_CYLINDER_COUNT) - 1;
	static constexpr scheduler_arg_t EventIndexShift = MAX_CYLINDER_COUNT;
	static constexpr scheduler_arg_t Stage2Flag = 1u << (MAX_CYLINDER_COUNT + 4);

	constexpr InjectorContext(CylinderMask mask, uint8_t eventIndex, bool stage2Active)
		: raw((mask & OutputMask) | ((eventIndex & 0xF) << EventIndexShift) | (stage2Active ? Stage2Flag : 0)) {}

	constexpr explicit InjectorContext(scheduler_arg_t value) : raw(value) {}

	constexpr CylinderMask outputsMask() const { return raw & OutputMask; }
	constexpr uint8_t eventIndex() const { return (raw >> EventIndexShift) & 0xF; }
	constexpr bool stage2Active() const { return raw & Stage2Flag; }
};

static_assert(MAX_CYLINDER_COUNT <= sizeof(CylinderMask) * 8,
	"CylinderMask must have one bit per cylinder");
static_assert(MAX_CYLINDER_COUNT <= 12);

void startInjection(scheduler_arg_t arg);
void endInjection(scheduler_arg_t arg);
void endInjectionStage2(scheduler_arg_t arg);


/**
 * This class knows about when to inject fuel
 */
class FuelSchedule {
public:
	FuelSchedule();

	// Call this function if something happens that requires a rebuild, like a change to the trigger pattern
	void invalidate();

	// Call this every trigger tooth.  It will schedule all required injector events.
	void onTriggerTooth(efitick_t nowNt, float currentPhase, float nextPhase);

	/**
	 * this method schedules all fuel events for an engine cycle
	 * Calculate injector opening angle, pins, and mode for all injectors
	 */
	void addFuelEvents();

	/**
	 * injection events, per cylinder
	 */
	InjectionEvent elements[MAX_CYLINDER_COUNT];
	bool isReady = false;

	// [tag:fuel_top_up] Engine stop or sync loss: forget registered main pulses.
	void resetTopUp();
};

FuelSchedule * getFuelSchedule();
