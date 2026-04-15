// file second_tables.cpp

#include "pch.h"
#include "second_tables.h"
#include "extra_flash_pages.h"

static constexpr uint32_t PAGE4_DATA_VERSION = 1;

using page4_container_s = ExtraPageContainer<page4_s, PAGE4_DATA_VERSION>;

static_assert(sizeof(page4_container_s) % 32 == 0,
	"page4_container_s must be 32-byte aligned for STM32H7 flash writes");

static page4_container_s secondTablesContainer;

void secondTablesSetDefaults() {
	secondTablesContainer.data = {};

	// Start with the primary tables/bins as defaults
	copyTable(secondTablesContainer.data.secondVeTable, config->veTable);
	copyArray(secondTablesContainer.data.secondVeLoadBins, config->veLoadBins);
	copyArray(secondTablesContainer.data.secondVeRpmBins, config->veRpmBins);

	setLinearCurve(secondTablesContainer.data.secondVeBlendBins, 0, 100);
	setLinearCurve(secondTablesContainer.data.secondVeBlendValues, 0, 100);

	copyTable(secondTablesContainer.data.secondIgnitionTable, config->ignitionTable);
	copyArray(secondTablesContainer.data.secondIgnitionLoadBins, config->ignitionLoadBins);
	copyArray(secondTablesContainer.data.secondIgnitionRpmBins, config->ignitionRpmBins);

	setLinearCurve(secondTablesContainer.data.secondIgnitionBlendBins, 0, 100);
	setLinearCurve(secondTablesContainer.data.secondIgnitionBlendValues, 0, 100);
}

void loadSecondTables() {
#if EFI_CONFIGURATION_STORAGE
	if (storageRead(EFI_SECOND_TABLES_RECORD_ID,
			secondTablesGetStoragePtr(),
			secondTablesGetStorageSize()) == StorageStatus::Ok
		&& secondTablesIsValid()) {
		return;
		}
#endif
	secondTablesSetDefaults();
}

bool secondTablesIsValid() {
	return secondTablesContainer.isValid();
}

page4_s* secondTablesGetState() {
	return &secondTablesContainer.data;
}

void* secondTablesGetTsPage() {
	return (void*)&secondTablesContainer.data;
}

size_t secondTablesGetTsPageSize() {
	return sizeof(page4_s);
}

void secondTablesPrepareForStorage() {
	secondTablesContainer.prepareForStorage();
}

uint8_t* secondTablesGetStoragePtr() {
	return (uint8_t*)&secondTablesContainer;
}

size_t secondTablesGetStorageSize() {
	return sizeof(secondTablesContainer);
}
