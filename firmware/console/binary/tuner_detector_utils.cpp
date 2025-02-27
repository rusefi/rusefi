//
// Created by kifir on 2/27/25.
//

#include "pch.h"

#include "tuner_detector_utils.h"

bool TunerDetectorUtils::isTuningDetectorUndefined() {
    return (engineConfiguration->tuningDetector == 0);
}

uint8_t TunerDetectorUtils::getUserEnteredTuningDetector() {
    return engineConfiguration->tuningDetector + TUNING_DETECTOR_PERSISTENCE_OFFSET;
}

void TunerDetectorUtils::setUserEnteredTuningDetector(const uint8_t tuningDetector) {
    engineConfiguration->tuningDetector = tuningDetector - TUNING_DETECTOR_PERSISTENCE_OFFSET;
}