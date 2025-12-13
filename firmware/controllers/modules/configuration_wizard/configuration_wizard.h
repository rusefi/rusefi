/*
 * @file configurationWizard.h
 *
 * @date: nov 11, 2025
 * @author FDSoftware
 */

#pragma once

#include "generated_rusefi_config_hashes.h"


class ConfigurationWizard {
public:
    static void onConfigOnStartUpOrBurn(bool isRunningOnBurn);
    static void onSlowCallback();
};

void vinStrategy(bool isRunningOnBurn);
void vinStrategyOnSlowCallback();
