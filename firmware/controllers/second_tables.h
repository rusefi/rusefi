// file second_tables.h

#pragma once

#include "page_4_generated.h"

page4_s* secondTablesGetState();

void loadSecondTables();

// Set default values (copies primary tables).
void secondTablesSetDefaults();

// Returns true if the in-RAM container has a valid version + CRC.
bool secondTablesIsValid();

// TunerStudio interface — returns the raw page4_s data (no container wrapper).
void* secondTablesGetTsPage();
size_t secondTablesGetTsPageSize();

// Storage interface — returns the CRC-wrapped container for persistence.
// Call secondTablesPrepareForStorage() first to compute the CRC before writes.
void secondTablesPrepareForStorage();
uint8_t* secondTablesGetStoragePtr();
size_t secondTablesGetStorageSize();
