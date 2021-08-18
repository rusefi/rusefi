/**
 * @author Andrey Belomutskiy, (c) 2012-2020
 *
 * EGO Exhaust Gas Oxygen, also known as AFR Air/Fuel Ratio :)
 *
 * todo: rename this class? refactor since there is also CJ125?
 *
 * rusEfi has three options for wideband:
 * 1) integration with external widebands using liner analog signal wire
 * 2) 8-point interpolation curve to emulate a wide-band with a narrow-band sensor.
 * 3) CJ125 internal wideband controller is known to work with both 4.2 and 4.9
 *
 */
#include "pch.h"

#include "cyclic_buffer.h"

#if EFI_CJ125
#include "cj125.h"
#endif /* EFI_CJ125 */

#ifdef EFI_NARROW_EGO_AVERAGING
// Needed by narrow EGOs (see updateEgoAverage()).
// getAfr() is called at ~50Hz, so we store at most (1<<3)*32 EGO values for ~5 secs.
#define EGO_AVG_SHIFT 3
#define EGO_AVG_BUF_SIZE 32 // 32*sizeof(float)

static bool useAveraging = false;
// Circular running-average buffer, used by CIC-like averaging filter
static cyclic_buffer<float, EGO_AVG_BUF_SIZE> egoAfrBuf;
// Total ego iterations (>240 days max. for 10ms update period)
static int totalEgoCnt = 0;
// We need this to calculate the real number of values stored in the buffer.
static int prevEgoCnt = 0;

// todo: move it to engineConfiguration
static const float stoichAfr = STOICH_RATIO;
static const float maxAfrDeviation = 5.0f;	// 9.7..19.7
static const int minAvgSize = (EGO_AVG_BUF_SIZE / 2);	// ~0.6 sec for 20ms period of 'fast' callback, and it matches a lag time of most narrow EGOs
static const int maxAvgSize = (EGO_AVG_BUF_SIZE - 1);	// the whole buffer

#ifdef EFI_NARROW_EGO_AVERAGING
// we store the last measured AFR value to predict the current averaging window size
static float lastAfr = stoichAfr;
#endif

void initEgoAveraging(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	// Our averaging is intended for use only with Narrow EGOs.
	if (CONFIG(afr_type) == ES_NarrowBand) {
		totalEgoCnt = prevEgoCnt = 0;
		egoAfrBuf.clear();
		useAveraging = true;
	}
}

static float updateEgoAverage(float afr) {
	// use a variation of cascaded integrator-comb (CIC) filtering
	totalEgoCnt++;
	int localBufPos = (totalEgoCnt >> EGO_AVG_SHIFT) % EGO_AVG_BUF_SIZE;
	int localPrevBufPos = ((totalEgoCnt - 1) >> EGO_AVG_SHIFT) % EGO_AVG_BUF_SIZE;
	
	// reset old buffer cell
	if (localPrevBufPos != localBufPos) {
		egoAfrBuf.elements[localBufPos] = 0;
		// Remove (1 << EGO_AVG_SHIFT) elements from our circular buffer (except for the 1st cycle).
		if (totalEgoCnt >= (EGO_AVG_BUF_SIZE << EGO_AVG_SHIFT))
			prevEgoCnt += (1 << EGO_AVG_SHIFT);
	}
	// integrator stage
	egoAfrBuf.elements[localBufPos] += afr;
	
	// Change window size depending on |AFR-stoich| deviation.
	// The narrow EGO is very noisy when AFR is close to shoich.
	// And we need the fastest EGO response when AFR has the extreme deviation (way too lean/rich).
	float avgSize = maxAvgSize;//interpolateClamped(minAvgSize, maxAfrDeviation, maxAvgSize, 0.0f, absF(lastAfr - stoichAfr));
	// choose the number of recently filled buffer cells for averaging
	int avgCnt = (int)efiRound(avgSize, 1.0f) << EGO_AVG_SHIFT;
	// limit averaging count to the real stored count
	int startAvgCnt = maxI(totalEgoCnt - avgCnt, prevEgoCnt);
	
	// return moving average of N last sums
	float egoAfrSum = 0;
	for (int i = (totalEgoCnt >> EGO_AVG_SHIFT); i >= (startAvgCnt >> EGO_AVG_SHIFT); i--) {
		egoAfrSum += egoAfrBuf.elements[i % EGO_AVG_BUF_SIZE];
	}
	// we divide by a real number of stored values to get an exact average
	return egoAfrSum / float(totalEgoCnt - startAvgCnt);
}
#else
void initEgoAveraging(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
}
#endif

bool hasAfrSensor(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	if (CONFIG(enableAemXSeries) || CONFIG(enableInnovateLC2)) {
		return true;
	}

#if EFI_CJ125 && HAL_USE_SPI
	if (CONFIG(isCJ125Enabled)) {
		return cjHasAfrSensor(PASS_ENGINE_PARAMETER_SIGNATURE);
	}
#endif /* EFI_CJ125 && HAL_USE_SPI */
	return isAdcChannelValid(engineConfiguration->afr.hwChannel);
}

extern float InnovateLC2AFR;

float getAfr(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
#if EFI_AUX_SERIAL
	if (CONFIG(enableInnovateLC2))
		return InnovateLC2AFR;
#endif

#if EFI_CJ125 && HAL_USE_SPI
	if (CONFIG(isCJ125Enabled)) {
		return cjGetAfr(PASS_ENGINE_PARAMETER_SIGNATURE);
	}
#endif /* EFI_CJ125 && HAL_USE_SPI */
	afr_sensor_s * sensor = &CONFIG(afr);

	if (!isAdcChannelValid(engineConfiguration->afr.hwChannel)) {
		return 0;
	}

	float volts = getVoltageDivided("ego", sensor->hwChannel PASS_ENGINE_PARAMETER_SUFFIX);

	if (CONFIG(afr_type) == ES_NarrowBand) {
		float afr = interpolate2d(volts, engineConfiguration->narrowToWideOxygenBins, engineConfiguration->narrowToWideOxygen);
#ifdef EFI_NARROW_EGO_AVERAGING
		if (useAveraging)
			afr = updateEgoAverage(afr);
		return (lastAfr = afr);
#else
		return afr;
#endif
	}

	return interpolateMsg("AFR", sensor->v1, sensor->value1, sensor->v2, sensor->value2, volts)
			+ engineConfiguration->egoValueShift;
}

static void initEgoSensor(afr_sensor_s *sensor, ego_sensor_e type) {

	switch (type) {
	case ES_BPSX_D1:
		/**
		 * This decodes BPSX D1 Wideband Controller analog signal
		 */
		sensor->v1 = 0;
		sensor->value1 = 9;
		sensor->v2 = 5;
		sensor->value2 = 19;
		break;

	case ES_Innovate_MTX_L:
		sensor->v1 = 0;
		sensor->value1 = 7.35;
		sensor->v2 = 5;
		sensor->value2 = 22.39;
		break;
	case ES_14Point7_Free:
		sensor->v1 = 0;
		sensor->value1 = 9.996;
		sensor->v2 = 5;
		sensor->value2 = 19.992;
		break;
		// technically 14Point7 and PLX use the same scale
	case ES_PLX:
		sensor->v1 = 0;
		sensor->value1 = 10;
		sensor->v2 = 5;
		sensor->value2 = 20;
		break;
	case ES_NarrowBand:
		sensor->v1 = 0.1;
		sensor->value1 = 15;
		sensor->v2 = 0.9;
		sensor->value2 = 14;
		break;
	default:
		firmwareError(CUSTOM_EGO_TYPE, "Unexpected EGO %d", type);
		break;
	}
}

void setEgoSensor(ego_sensor_e type DECLARE_CONFIG_PARAMETER_SUFFIX) {
	CONFIG(afr_type) = type;
	initEgoSensor(&engineConfiguration->afr, type);
}
