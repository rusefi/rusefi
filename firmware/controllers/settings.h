/**
 * @file settings.h
 * @brief This file is about configuring engine via the human-readable protocol
 *
 * @date Dec 30, 2012
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

void initSettings();
void printSpiState();
void printConfiguration();

bool isHwQcMode();
void setHwQcMode();

void setEngineTypeAndSave(int value);
void setEngineType(int value, bool isWriteToFlash = true);
void readPin(const char *pinName);

void printDateTime();
void setDateTime(const char * const isoDateTime);

void setLuaScript(const char *luaScript);
