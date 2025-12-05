/*
 * @file configurationWizard.h
 *
 * @date: nov 11, 2025
 * @author FDSoftware
 */

#pragma once

class ConfigurationWizard {
public:
    static void onConfigOnStartUpOrBurn(bool isRunningOnBurn);
    static void onSlowCallback();
};

void vinStrategy(bool isRunningOnBurn);
void vinStrategyOnSlowCallback();
