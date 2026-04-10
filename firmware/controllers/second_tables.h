// file second_tables.h

#pragma once

#include "page_4_generated.h"

void initSecondTables();

page4_s* secondTablesGetState();

// TunerStudio interface — returns the raw page4_s data (no container wrapper).
void* secondTablesGetTsPage();
size_t secondTablesGetTsPageSize();

// Storage interface — returns the CRC-wrapped container for persistence.
// Call secondTablesPrepareForStorage() first to compute the CRC.
void secondTablesPrepareForStorage();
const uint8_t* secondTablesGetStoragePtr();
size_t secondTablesGetStorageSize();
