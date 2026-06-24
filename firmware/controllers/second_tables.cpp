// file second_tables.cpp

#include "pch.h"
#include "second_tables.h"
#include "extra_flash_pages.h"

static constexpr uint32_t PAGE4_DATA_VERSION = 1;

using page4_container_s = ExtraPageContainer<page4_s, PAGE4_DATA_VERSION>;

static_assert(sizeof(page4_container_s) % 32 == 0,
	"page4_container_s must be 32-byte aligned for STM32H7 flash writes");

// Page 4 must not grow past its fixed flash slot into the Lua page that sits just above it.
// config_page_4.txt is actively edited (second VE/ignition/torque tables), so this guards
// against silently corrupting the Lua page when new fields are added — bump LUA_PAGE_SECTOR_OFFSET.
#if (EFI_STORAGE_INT_FLASH == TRUE) && (EFI_STORAGE_MFS != TRUE) && !EFI_SIMULATOR
static_assert(PAGE4_SECTOR_OFFSET + sizeof(page4_container_s) <= LUA_PAGE_SECTOR_OFFSET,
	"page4 grew into the Lua page region — bump LUA_PAGE_SECTOR_OFFSET");
#endif

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
