// file lua_config_page.cpp

#include "pch.h"
#include "lua_config_page.h"
#include "extra_flash_pages.h"

static constexpr uint32_t PAGE5_DATA_VERSION = 1;

using page5_container_s = ExtraPageContainer<page5_s, PAGE5_DATA_VERSION>;

static_assert(sizeof(page5_container_s) % 32 == 0,
	"page5_container_s must be 32-byte aligned for STM32H7 flash writes");

// On internal-flash boards the Lua page is piggy-backed into the primary settings
// sector at LUA_PAGE_SECTOR_OFFSET. The Lua page is by far the largest extra page
// (LUA_SCRIPT_SIZE is up to tens of KB per board) so it must fit below the 128 KB
// sector top — this also guarantees it never reaches the next-sector backup config
// copy on H7/F4. Failing this is a per-board compile error: lower LUA_SCRIPT_SIZE.
#if (EFI_STORAGE_INT_FLASH == TRUE) && (EFI_STORAGE_MFS != TRUE) && !EFI_SIMULATOR
static_assert(LUA_PAGE_SECTOR_OFFSET + sizeof(page5_container_s) <= 128u * 1024u,
	"Lua config page does not fit in the flash sector — reduce LUA_SCRIPT_SIZE for this board");
#endif

// Place the container in the same RAM region the persistent config uses (CCM where available,
// see persistent_store.cpp's PERSISTENT_LOCATION). luaScript was previously a field inside
// persistentState, so keeping the extracted page in CCM_OPTIONAL preserves the per-region RAM
// balance — otherwise CCM boards (F4/F7/H7, e.g. nucleo_f429) overflow main SRAM by LUA_SCRIPT_SIZE
// while CCM gains the same slack.
static page5_container_s luaConfigPageContainer CCM_OPTIONAL;

void luaConfigPageSetDefaults() {
	luaConfigPageContainer.data = {};
}

void loadLuaConfigPage() {
#if EFI_CONFIGURATION_STORAGE
	if (storageRead(EFI_LUA_PAGE_RECORD_ID,
			luaConfigPageGetStoragePtr(),
			luaConfigPageGetStorageSize()) == StorageStatus::Ok
		&& luaConfigPageIsValid()) {
		return;
	}
#endif
	luaConfigPageSetDefaults();
}

bool luaConfigPageIsValid() {
	return luaConfigPageContainer.isValid();
}

page5_s* luaConfigPageGetState() {
	return &luaConfigPageContainer.data;
}

void* luaConfigPageGetTsPage() {
	return (void*)&luaConfigPageContainer.data;
}

size_t luaConfigPageGetTsPageSize() {
	return sizeof(page5_s);
}

void luaConfigPagePrepareForStorage() {
	luaConfigPageContainer.prepareForStorage();
}

uint8_t* luaConfigPageGetStoragePtr() {
	return (uint8_t*)&luaConfigPageContainer;
}

size_t luaConfigPageGetStorageSize() {
	return sizeof(luaConfigPageContainer);
}
