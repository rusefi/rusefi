/*
 * @file configurationWizard.cpp
 *
 * @date: nov 11, 2025
 * @author FDSoftware
 */

#include "configuration_wizard.h"

void ConfigurationWizard::onConfigOnStartUpOrBurn(bool isRunningOnBurn) {
    vinStrategy(isRunningOnBurn);
}