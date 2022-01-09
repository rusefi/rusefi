#include "pch.h"

void hellenWbo() {
	engineConfiguration->enableAemXSeries = true;
}

void setHellenDefaultVrThresholds() {
	for (int i = 0;i<VR_THRESHOLD_COUNT;i++) {
		setLinearCurve(engineConfiguration->vrThreshold[i].rpmBins, 600, 7000, 100);
		setLinearCurve(engineConfiguration->vrThreshold[i].values, 0.6, 1.2, 0.1);
	}
}
