// file lua_config_page.h
//
// The Lua script lives on its own dedicated TunerStudio page (page5_s) instead of
// consuming the limited 64Kb page-1 calibration budget. See #7911. The wiring mirrors
// second_tables.{cpp,h} / the extra_flash_pages framework.

#pragma once

#include "page_5_generated.h"

page5_s* luaConfigPageGetState();

void loadLuaConfigPage();

// Set default values (empty script). The actual default script for an engine type
// is installed afterwards by setLuaScript() during applyEngineType(), which runs
// after resetExtraPages()/luaConfigPageSetDefaults().
void luaConfigPageSetDefaults();

// Returns true if the in-RAM container has a valid version + CRC.
bool luaConfigPageIsValid();

// TunerStudio page interface — returns the raw page5_s data (no container wrapper).
void* luaConfigPageGetTsPage();
size_t luaConfigPageGetTsPageSize();

// Storage interface — returns the CRC-wrapped container for persistence.
// Call luaConfigPagePrepareForStorage() first to compute the CRC before writes.
void luaConfigPagePrepareForStorage();
uint8_t* luaConfigPageGetStoragePtr();
size_t luaConfigPageGetStorageSize();
