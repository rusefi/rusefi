/**
 * @file    eficonsole.h
 * @brief   Console package entry point header
 *
 * @date Nov 15, 2012
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "engine_module.h"

void initializeConsole();
void startUsbConsole();
void startWifiConsole();

struct EthernetConsoleModule final : public EngineModule {
	void initNoConfiguration() override;
};
