/*
 * @file test_cam_phase_drift.cpp
 *
 * The cam-phase continuity cross-check (custom_board_vvtDriftLimit):
 * a fixed cam must report the same VVT phase every cam revolution. A
 * crank-sync basis error (a false sync that slipped past the position gate)
 * shifts the reading by the sync error, so a jump beyond the board's limit
 * forces the crank decoder to desync and re-sync cleanly on the next real
 * gap. m74_9 sets 15 degrees (the 21129 cam is belt-driven, no VVT phaser).
 */

#include "pch.h"
#include "board_overrides.h"

// steady 400 RPM: 60 slots * 2.5 ms = 150 ms per crank revolution
static constexpr float steadySlotMs = 2.5f;

// One full 60-2 revolution: 58 real teeth, then the missing-teeth gap.
static void fire60_2Revolution(EngineTestHelper& eth, float slotMs, float gapRatio) {
	for (int i = 0; i < 58; i++) {
		eth.fireRise(slotMs / 2);
		eth.fireFall(slotMs / 2);
	}

	// the next call's first rise lands gapRatio * slotMs after tooth 57's rise
	eth.moveTimeForwardUs(MS2US((gapRatio - 1.0f) * slotMs));
}

TEST(trigger, camPhaseDriftForcesCrankResync) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setCrankOperationMode();
	eth.setTriggerType(trigger_type_e::TT_TOOTHED_WHEEL_60_2);
	engineConfiguration->alwaysInstantRpm = true;

	engineConfiguration->vvtMode[0] = VVT_SINGLE_TOOTH;
	engineConfiguration->engineSyncCam = 0;
	engineConfiguration->vvtOffsets[0] = 0;

	custom_board_vvtDriftLimit = []() { return 15.0f; };
	// the override global persists across tests in this binary - clean up
	struct ResetOverride {
		~ResetOverride() { custom_board_vvtDriftLimit = std::nullopt; }
	} resetOverride;

	// sync the crank
	fire60_2Revolution(eth, steadySlotMs, 3.0f);
	fire60_2Revolution(eth, steadySlotMs, 3.0f);
	fire60_2Revolution(eth, steadySlotMs, 3.0f);

	ASSERT_TRUE(engine->triggerCentral.triggerState.getShaftSynchronized()) << "crank synced";

	// first cam event: establishes the phase reference (and full phase sync)
	hwHandleVvtCamSignal(true, getTimeNowNt(), 0);

	// two crank revolutions later the cam reports the same phase: no reaction
	fire60_2Revolution(eth, steadySlotMs, 3.0f);
	fire60_2Revolution(eth, steadySlotMs, 3.0f);
	size_t warningsBefore = getRecentWarnings()->getCount();
	hwHandleVvtCamSignal(true, getTimeNowNt(), 0);

	ASSERT_EQ(warningsBefore, getRecentWarnings()->getCount()) << "stable cam phase must not fire";
	ASSERT_TRUE(engine->triggerCentral.triggerState.getShaftSynchronized()) << "no desync on stable phase";

	// now read the cam 30 degrees late (720 deg cycle = 300 ms at 400 rpm,
	// 30 deg = 12.5 ms): the phase reference jumps beyond the 15 deg limit
	fire60_2Revolution(eth, steadySlotMs, 3.0f);
	fire60_2Revolution(eth, steadySlotMs, 3.0f);
	eth.moveTimeForwardUs(MS2US(12.5f));
	hwHandleVvtCamSignal(true, getTimeNowNt(), 0);

	ASSERT_EQ(warningsBefore + 1, getRecentWarnings()->getCount()) << "one phase-jump warning";
	EXPECT_EQ(ObdCode::CUSTOM_VVT_PHASE_JUMP, getRecentWarnings()->get(warningsBefore).Code);
	ASSERT_FALSE(engine->triggerCentral.triggerState.getShaftSynchronized()) << "crank must desync on the cam phase jump";

	// the next real gap re-syncs cleanly (the gap of the first fired revolution
	// is checked at the first rise of the second one)
	fire60_2Revolution(eth, steadySlotMs, 3.0f);
	fire60_2Revolution(eth, steadySlotMs, 3.0f);
	ASSERT_TRUE(engine->triggerCentral.triggerState.getShaftSynchronized()) << "crank re-synced";
}
