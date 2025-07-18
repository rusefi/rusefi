/*
 * @file test_flash.cpp
 *
 * @date: jul 15, 2025
 * @author FDSoftware
 */


#include "pch.h"
#include "storage.h"
#include "engine_test_helper.h"

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
