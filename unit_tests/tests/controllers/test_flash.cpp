/*
 * @file test_flash.cpp
 *
 * @date: jul 15, 2025
 * @author FDSoftware
 */


#include "pch.h"
#include "storage.h"
#include "storage_detail.h"
#include "engine_test_helper.h"

namespace {

class TestReadStorage : public SettingStorageBase {
public:
	TestReadStorage(StorageStatus result, uint8_t value)
		: m_result(result)
		, m_value(value) {
	}

	bool isReady() override {
		return true;
	}

	bool isIdSupported(size_t) override {
		return true;
	}

	StorageStatus store(size_t, const uint8_t*, size_t) override {
		return StorageStatus::NotSupported;
	}

	StorageStatus read(size_t, uint8_t* ptr, size_t size) override {
		readCount++;
		if (size > 0) {
			ptr[0] = m_value;
		}
		return m_result;
	}

	StorageStatus format() override {
		return StorageStatus::NotSupported;
	}

	int readCount = 0;

private:
	StorageStatus m_result;
	uint8_t m_value;
};

} // namespace

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

TEST(Storage, HigherPrioritySuccessfulReadStopsFallback) {
	TestReadStorage lowerPriority(StorageStatus::Failed, 0x22);
	TestReadStorage higherPriority(StorageStatus::Ok, 0x11);
	SettingStorageBase *storages[] = { &lowerPriority, &higherPriority };

	uint8_t value = 0;
	EXPECT_EQ(StorageStatus::Ok, storage_detail::readFirstSuccessful(
		storages, efi::size(storages), EFI_LTFT_RECORD_ID, &value, sizeof(value)));
	EXPECT_EQ(0x11, value);
	EXPECT_EQ(0, lowerPriority.readCount);
	EXPECT_EQ(1, higherPriority.readCount);
}

TEST(Storage, FailedHigherPriorityReadFallsBack) {
	TestReadStorage lowerPriority(StorageStatus::Ok, 0x33);
	TestReadStorage higherPriority(StorageStatus::Failed, 0x22);
	SettingStorageBase *storages[] = { &lowerPriority, &higherPriority };

	uint8_t value = 0;
	EXPECT_EQ(StorageStatus::Ok, storage_detail::readFirstSuccessful(
		storages, efi::size(storages), EFI_LTFT_RECORD_ID, &value, sizeof(value)));
	EXPECT_EQ(0x33, value);
	EXPECT_EQ(1, lowerPriority.readCount);
	EXPECT_EQ(1, higherPriority.readCount);
}
