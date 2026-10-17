#pragma once

void initSpeedometer();
void speedoUpdate();

// Bench test: pulse the speedo output at a fixed frequency for a few seconds, ignoring VehicleSpeed.
void startSpeedoBenchTest(float freqHz);
