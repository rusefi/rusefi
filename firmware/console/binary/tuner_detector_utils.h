//
// Created by kifir on 2/27/25.
//

#pragma once

class TunerDetectorUtils {
public:
    static bool isTuningDetectorUndefined();
    static uint8_t getUserEnteredTuningDetector();
    static void setUserEnteredTuningDetector(uint8_t tuningDetector);
};