/*
 * @file test_flash.cpp
 *
 * @date: jul 15, 2025
 * @author FDSoftware
 */


#include "pch.h"
#include "storage.h"
#include "board_overrides.h"
#include "engine_test_helper.h"
#include "spark_logic.h"

bool canFlashWhileRunning = true;

bool mcuCanFlashWhileRunning() { return canFlashWhileRunning; }

TEST(Storage, AllowWriteID) {
    EngineTestHelper eth(engine_type_e::TEST_ENGINE);
    engine->rpmCalculator.setStopSpinning();

    // Settings record with MCU that can flash while running
    engine->triggerCentral.directSelfStimulation = false;
    // Mock that MCU can flash while running
    canFlashWhileRunning = true;
    EXPECT_TRUE(storageAllowWriteID(EFI_SETTINGS_RECORD_ID));

    // Settings record with MCU that cannot flash while running,
    // but engine is in self-stimulation mode
    canFlashWhileRunning = false;
    engine->triggerCentral.directSelfStimulation = true;
    EXPECT_TRUE(storageAllowWriteID(EFI_SETTINGS_RECORD_ID));

    // Settings record with MCU that cannot flash while running,
    // engine not in self-stimulation but stopped
    engine->triggerCentral.directSelfStimulation = false;
    engine->rpmCalculator.setStopSpinning();
    EXPECT_TRUE(storageAllowWriteID(EFI_SETTINGS_RECORD_ID));

    // Settings record with MCU that cannot flash while running,
    // engine running (should not allow write)
    engine->rpmCalculator.setRpmValue(1000);
    EXPECT_FALSE(storageAllowWriteID(EFI_SETTINGS_RECORD_ID));

    // Non-settings record (should always allow write)
    EXPECT_TRUE(storageAllowWriteID(EFI_LTFT_RECORD_ID));
    EXPECT_TRUE(storageAllowWriteID((StorageItemId)123)); // Some random ID
}

class StorageBoardGate : public testing::Test {
protected:
	std::optional<setup_custom_bool_type> savedGate;
	bool savedCanFlash;

	void SetUp() override {
		savedGate = custom_board_allowFlashNow;
		savedCanFlash = canFlashWhileRunning;
	}

	void TearDown() override {
		custom_board_allowFlashNow = savedGate;
		canFlashWhileRunning = savedCanFlash;
	}
};

TEST_F(StorageBoardGate, DefersAllRecordsWhileRunning) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	canFlashWhileRunning = false;
	custom_board_allowFlashNow = []() {
		return engine->triggerCentral.directSelfStimulation || engine->rpmCalculator.isStopped();
	};
	engine->triggerCentral.directSelfStimulation = false;
	engine->rpmCalculator.setRpmValue(1000);
	EXPECT_FALSE(storageAllowWriteID(EFI_SETTINGS_RECORD_ID));
	EXPECT_FALSE(storageAllowWriteID(EFI_SETTINGS_BACKUP_RECORD_ID));
	// Every MFS record must honor the board's flash gate.
	EXPECT_FALSE(storageAllowWriteID(EFI_LTFT_RECORD_ID));
	EXPECT_FALSE(storageAllowWriteID(static_cast<StorageItemId>(123)));

	engine->rpmCalculator.setStopSpinning();
	EXPECT_TRUE(storageAllowWriteID(EFI_LTFT_RECORD_ID));
	EXPECT_TRUE(storageAllowWriteID(EFI_SETTINGS_RECORD_ID));

	engine->rpmCalculator.setRpmValue(1000);
	engine->triggerCentral.directSelfStimulation = true;
	EXPECT_TRUE(storageAllowWriteID(EFI_LTFT_RECORD_ID));
	EXPECT_TRUE(storageAllowWriteID(EFI_SETTINGS_RECORD_ID));
}

TEST_F(StorageBoardGate, NoHookPreservesExternalStorageWrites) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	custom_board_allowFlashNow.reset();
	canFlashWhileRunning = false;
	engine->triggerCentral.directSelfStimulation = false;
	engine->rpmCalculator.setRpmValue(1000);
	EXPECT_FALSE(storageAllowWriteID(EFI_SETTINGS_RECORD_ID));
	EXPECT_TRUE(storageAllowWriteID(EFI_LTFT_RECORD_ID));

	canFlashWhileRunning = true;
	EXPECT_TRUE(storageAllowWriteID(EFI_SETTINGS_RECORD_ID));
}

TEST_F(StorageBoardGate, SyncLossAllowsSettingsWriteDuringActiveDwell) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	canFlashWhileRunning = false; // The production STM32F4 capability.
	custom_board_allowFlashNow.reset();
	engine->triggerCentral.directSelfStimulation = false;
	engineConfiguration->cylindersCount = 1;
	engineConfiguration->firingOrder = FO_1;
	engineConfiguration->ignitionMode = IM_ONE_COIL;
	engineConfiguration->enableTrailingSparks = false;
	engine->updateTriggerConfiguration();
	engine->rpmCalculator.setRpmValue(1000);
	engine->ignitionState.sparkDwell = 3;
	engine->ignitionState.dwellDurationAngle = 18;
	setArrayValues(engine->engineState.timingAdvance, -30.0f);
	initializeIgnitionActions();
	// Supply the recent-edge timestamp normally recorded by handleShaftSignal.
	engine->triggerCentral.m_lastEventTimer.reset();
	onTriggerEventSparkLogic(1000, getTimeNowNt(), 0, 90);
	eth.moveTimeForwardUs(2500);
	ASSERT_EQ(1, eth.executeActions());
	ASSERT_TRUE(enginePins.coils[0].getLogicValue());
	ASSERT_FALSE(storageAllowWriteID(EFI_SETTINGS_RECORD_ID));

	// Decoder sync loss sets cached RPM to zero while the crank edge remains
	// recent and the timed spark is still due. No simulated flash erase runs.
	engine->OnTriggerSynchronizationLost();
	ASSERT_TRUE(engine->rpmCalculator.isStopped());
	ASSERT_TRUE(engine->triggerCentral.engineMovedRecently());
	engine->efiWatchdog();
	EXPECT_TRUE(enginePins.coils[0].getLogicValue());
	EXPECT_EQ(1, engine->scheduler.size());
	EXPECT_TRUE(storageAllowWriteID(EFI_SETTINGS_RECORD_ID));
	EXPECT_TRUE(storageAllowWriteID(EFI_SETTINGS_BACKUP_RECORD_ID));

	// With no flash stall, the queued protective discharge still completes.
	eth.moveTimeForwardUs(3000);
	ASSERT_EQ(1, eth.executeActions());
	EXPECT_FALSE(enginePins.coils[0].getLogicValue());
}
