#include "pch.h"

#include "adc_subscription.h"
#include "functional_sensor.h"
#include "redundant_sensor.h"
#include "redundant_ford_tps.h"
#include "proxy_sensor.h"
#include "linear_func.h"
#include "tps.h"
#include "auto_generated_sensor.h"
#include "defaults.h"

struct TpsConfig {
	adc_channel_e channel;
	float closed;
	float open;
	float min;
	float max;
};

PUBLIC_API_WEAK float getFuncPairAllowedSplit() {
  return 0.5f;
}

class FuncSensPair {
public:
AdcSubscriptionEntry *adc = nullptr;
	FuncSensPair(float divideInput, SensorType type)
		: m_func(divideInput)
		, m_sens(type, MS2NT(10))
	{
		m_sens.setFunction(m_func);
	}

	bool init(const TpsConfig& cfg) {
		// If the configuration was invalid, don't continue to configure the sensor
		if (!configure(cfg)) {
			return false;
		}

		adc = AdcSubscription::SubscribeSensor(m_sens, cfg.channel, /*lowpassCutoffHz*/ 200);

		return m_sens.Register();
	}

	void deinit() {
		AdcSubscription::UnsubscribeSensor(m_sens);
	}

	SensorType type() const {
		return m_sens.type();
	}

	const char* name() const {
		return m_sens.getSensorName();
	}

private:
	bool configure(const TpsConfig& cfg) {
		// Only configure if we have a channel
		if (!isAdcChannelValid(cfg.channel)) {
#if EFI_UNIT_TEST
	printf("Configured NO hardware %s\n", name());
#endif
			return false;
		}

		float scaledClosed = cfg.closed / m_func.getDivideInput();
		float scaledOpen = cfg.open / m_func.getDivideInput();

		float split = std::abs(scaledOpen - scaledClosed);

		// If the voltage for closed vs. open is very near, something is wrong with your calibration
		if (split < getFuncPairAllowedSplit()) {
			firmwareError(ObdCode::OBD_TPS_Configuration, "\"%s\" problem: open %.2f/closed %.2f cal values are too close together. Check your calibration and wiring!", name(),
					cfg.open,
					cfg.closed);
			return false;
		}

		m_func.configure(
			cfg.closed, 0,
			cfg.open, POSITION_FULLY_OPEN,
			cfg.min, cfg.max
		);

#if EFI_UNIT_TEST
	printf("Configured YES %s\n", name());
#endif
		return true;
	}

	LinearFunc m_func;
	FunctionalSensor m_sens;
};

struct RedundantPair {
public:
	RedundantPair(FuncSensPair& pri, FuncSensPair& sec, SensorType outputType)
		: m_pri(pri)
		, m_sec(sec)
		, m_redund(outputType, m_pri.type(), m_sec.type())
	{
	}

	void init(bool isFordTps, RedundantFordTps* fordTps, float secondaryMaximum, const TpsConfig& primary, const TpsConfig& secondary, bool allowIdenticalSensors = false) {
		bool hasFirst = m_pri.init(primary);
		if (!hasFirst) {
			// no input if we have no first channel
			return;
		}

		if (!allowIdenticalSensors) {
			// Check that the primary and secondary aren't too close together - if so, the user may have done
			// an unsafe thing where they wired a single sensor to both inputs. Don't do that!
			bool hasBothSensors = isAdcChannelValid(primary.channel) && isAdcChannelValid(secondary.channel);
			bool tooCloseClosed = std::abs(primary.closed - secondary.closed) < 0.2f;
			bool tooCloseOpen = std::abs(primary.open - secondary.open) < 0.2f;

			if (hasBothSensors && tooCloseClosed && tooCloseOpen) {
				firmwareError(ObdCode::OBD_TPS_Configuration, "Configuration for redundant pair %s/%s are too similar - did you wire one sensor to both inputs...?", m_pri.name(), m_sec.name());
				return;
			}
		}

		bool hasSecond = m_sec.init(secondary);

        if (engineConfiguration->etbSplit <= 0 || engineConfiguration->etbSplit > MAX_TPS_PPS_DISCREPANCY) {
            engineConfiguration->etbSplit = MAX_TPS_PPS_DISCREPANCY;
        }

		if (isFordTps && fordTps) {
			// we have a secondary
			fordTps->configure(engineConfiguration->etbSplit, secondaryMaximum);
			fordTps->Register();
		} else {
			// not ford TPS
			m_redund.configure(engineConfiguration->etbSplit, !hasSecond);
#if EFI_UNIT_TEST
printf("init m_redund.Register() %s\n", getSensorType(m_redund.type()));
#endif
			m_redund.Register();
		}
	}

	void deinit(bool isFordTps, RedundantFordTps* fordTps) {
		m_pri.deinit();
		m_sec.deinit();

		if (isFordTps && fordTps) {
			fordTps->unregister();
		} else {
			m_redund.unregister();
		}

	}

	void updateUnfilteredRawValues() {
	  engine->outputChannels.rawRawPpsPrimary = m_pri.adc == nullptr ? 0 : m_pri.adc->sensorVolts;
	  engine->outputChannels.rawRawPpsSecondary = m_sec.adc == nullptr ? 0 : m_sec.adc->sensorVolts;
	}

private:
	FuncSensPair& m_pri;
	FuncSensPair& m_sec;

	RedundantSensor m_redund;
};

static FuncSensPair tps1p(TPS_TS_CONVERSION, SensorType::Tps1Primary);
static FuncSensPair tps1s(TPS_TS_CONVERSION, SensorType::Tps1Secondary);
static FuncSensPair tps2p(TPS_TS_CONVERSION, SensorType::Tps2Primary);
static FuncSensPair tps2s(TPS_TS_CONVERSION, SensorType::Tps2Secondary);

// Used in case of "normal", non-Ford ETB TPS
static RedundantPair analogTps1(tps1p, tps1s, SensorType::Tps1);
static RedundantPair tps2(tps2p, tps2s, SensorType::Tps2);

#if EFI_SENT_SUPPORT
SentTps sentTps;
#endif

// Used only in case of weird Ford-style ETB TPS
static RedundantFordTps fordTps1(SensorType::Tps1, SensorType::Tps1Primary, SensorType::Tps1Secondary);
static RedundantFordTps fordTps2(SensorType::Tps2, SensorType::Tps2Primary, SensorType::Tps2Secondary);
static RedundantFordTps fordPps(SensorType::AcceleratorPedal, SensorType::AcceleratorPedalPrimary, SensorType::AcceleratorPedalSecondary);

// Pedal sensors and redundancy
static FuncSensPair pedalPrimary(1, SensorType::AcceleratorPedalPrimary);
static FuncSensPair pedalSecondary(1, SensorType::AcceleratorPedalSecondary);
static RedundantPair pedal(pedalPrimary, pedalSecondary, SensorType::AcceleratorPedal);

void updateUnfilteredRawPedal() {
  pedal.updateUnfilteredRawValues();
}

// This sensor indicates the driver's throttle intent - Pedal if we have one, TPS if not.
static ProxySensor driverIntent(SensorType::DriverThrottleIntent);

// These sensors are TPS-like, so handle them in here too
static FuncSensPair wastegate(1, SensorType::WastegatePosition);
static FuncSensPair idlePos(PACK_MULT_VOLTAGE, SensorType::IdlePosition);

void initTps() {
    criticalAssertVoid(engineConfiguration != nullptr, "null engineConfiguration");
	percent_t minTpsPps = engineConfiguration->tpsErrorDetectionTooLow;
	percent_t maxTpsPps = engineConfiguration->tpsErrorDetectionTooHigh;

	if (!engineConfiguration->consumeObdSensors) {
		bool isFordTps = engineConfiguration->useFordRedundantTps;
		bool isFordPps = engineConfiguration->useFordRedundantPps;

		float tpsSecondaryMaximum = engineConfiguration->tpsSecondaryMaximum;
		if (tpsSecondaryMaximum < 20) {
			// don't allow <20% split point
			tpsSecondaryMaximum = 20;
		}


#if EFI_SENT_SUPPORT
        if (isDigitalTps1()) {
            sentTps.Register();
        } else
#endif
        {
		    analogTps1.init(isFordTps, &fordTps1, tpsSecondaryMaximum,
    			{ engineConfiguration->tps1_1AdcChannel, (float)engineConfiguration->tpsMin, (float)engineConfiguration->tpsMax, minTpsPps, maxTpsPps },
	    		{ engineConfiguration->tps1_2AdcChannel, (float)engineConfiguration->tps1SecondaryMin, (float)engineConfiguration->tps1SecondaryMax, minTpsPps, maxTpsPps }
		    );
		}

		tps2.init(isFordTps, &fordTps2, tpsSecondaryMaximum,
			{ engineConfiguration->tps2_1AdcChannel, (float)engineConfiguration->tps2Min, (float)engineConfiguration->tps2Max, minTpsPps, maxTpsPps },
			{ engineConfiguration->tps2_2AdcChannel, (float)engineConfiguration->tps2SecondaryMin, (float)engineConfiguration->tps2SecondaryMax, minTpsPps, maxTpsPps }
		);

		float ppsSecondaryMaximum = engineConfiguration->ppsSecondaryMaximum;
		if (ppsSecondaryMaximum < 20) {
			// don't allow <20% split point
			ppsSecondaryMaximum = 20;
		}

	// Pedal sensors
	pedal.init(isFordPps, &fordPps, ppsSecondaryMaximum,
		{ engineConfiguration->throttlePedalPositionAdcChannel, engineConfiguration->throttlePedalUpVoltage, engineConfiguration->throttlePedalWOTVoltage, minTpsPps, maxTpsPps },
		{ engineConfiguration->throttlePedalPositionSecondAdcChannel, engineConfiguration->throttlePedalSecondaryUpVoltage, engineConfiguration->throttlePedalSecondaryWOTVoltage, minTpsPps, maxTpsPps },
		engineConfiguration->allowIdenticalPps
	);

		// TPS-like stuff that isn't actually a TPS
		wastegate.init({ engineConfiguration->wastegatePositionSensor, engineConfiguration->wastegatePositionClosedVoltage, engineConfiguration->wastegatePositionOpenedVoltage, minTpsPps, maxTpsPps });
		idlePos.init({ engineConfiguration->idlePositionChannel, (float)engineConfiguration->idlePositionMin, (float)engineConfiguration->idlePositionMax, minTpsPps, maxTpsPps });
	}

	// Route the pedal or TPS to driverIntent as appropriate
	if (isAdcChannelValid(engineConfiguration->throttlePedalPositionAdcChannel)) {
		driverIntent.setProxiedSensor(SensorType::AcceleratorPedal);
	} else {
		driverIntent.setProxiedSensor(SensorType::Tps1);
	}

	driverIntent.Register();
}

void deinitTps() {
	bool isFordTps = activeConfiguration.useFordRedundantTps;
	bool isFordPps = activeConfiguration.useFordRedundantPps;

	analogTps1.deinit(isFordTps, &fordTps1);
	tps2.deinit(isFordTps, &fordTps2);
	pedal.deinit(isFordPps, &fordPps);

#if EFI_SENT_SUPPORT
	sentTps.unregister();
#endif

	wastegate.deinit();
	idlePos.deinit();
}
