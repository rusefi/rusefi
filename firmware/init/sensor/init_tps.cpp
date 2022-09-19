#include "pch.h"

#include "adc_subscription.h"
#include "functional_sensor.h"
#include "redundant_sensor.h"
#include "redundant_ford_tps.h"
#include "proxy_sensor.h"
#include "linear_func.h"
#include "tps.h"

struct TpsConfig {
	adc_channel_e channel;
	float closed;
	float open;
	float min;
	float max;
};

class FuncSensPair {
public:
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

		AdcSubscription::SubscribeSensor(m_sens, cfg.channel, 200);

		return m_sens.Register();
	}

	void unsubscribe() {
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
			return false;
		}

		float scaledClosed = cfg.closed / m_func.getDivideInput();
		float scaledOpen = cfg.open / m_func.getDivideInput();

		float split = absF(scaledOpen - scaledClosed);

		// If the voltage for closed vs. open is very near, something is wrong with your calibration
		if (split < 0.5f) {
			firmwareError(OBD_TPS_Configuration, "\"%s\" problem: open %.2f/closed %.2f cal values are too close together. Check your calibration and wiring!", name(),
					cfg.open,
					cfg.closed);
			return false;
		}

		m_func.configure(
			cfg.closed, 0,
			cfg.open, 100, 
			cfg.min, cfg.max
		);

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

	void init(bool isFordTps, RedundantFordTps* fordTps, const TpsConfig& primary, const TpsConfig& secondary) {
		bool hasFirst = m_pri.init(primary);
		if (!hasFirst) {
			// no input if we have no first channel
			return;
		}

		{
			// Check that the primary and secondary aren't too close together - if so, the user may have done
			// an unsafe thing where they wired a single sensor to both inputs. Don't do that!
			bool hasBothSensors = isAdcChannelValid(primary.channel) && isAdcChannelValid(secondary.channel);
			bool tooCloseClosed = absF(primary.closed - secondary.closed) < 0.2f;
			bool tooCloseOpen = absF(primary.open - secondary.open) < 0.2f;

			if (hasBothSensors && tooCloseClosed && tooCloseOpen) {
				firmwareError(OBD_TPS_Configuration, "Configuration for redundant pair %s/%s are too similar - did you wire one sensor to both inputs...?", m_pri.name(), m_sec.name());
				return;
			}
		}

		bool hasSecond = m_sec.init(secondary);

		if (isFordTps && fordTps) {
			// we have a secondary
			fordTps->configure(5.0f, 52.6f);
			fordTps->Register();
		} else {
			// not ford TPS
			m_redund.configure(5.0f, !hasSecond);
			m_redund.Register();
		}
	}

	void unsubscribe() {
		m_pri.unsubscribe();
		m_sec.unsubscribe();
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
static RedundantPair tps1(tps1p, tps1s, SensorType::Tps1);
static RedundantPair tps2(tps2p, tps2s, SensorType::Tps2);

// Used only in case of weird Ford-style ETB TPS
static RedundantFordTps fordTps1(SensorType::Tps1, SensorType::Tps1Primary, SensorType::Tps1Secondary);
static RedundantFordTps fordTps2(SensorType::Tps2, SensorType::Tps2Primary, SensorType::Tps2Secondary);

// Pedal sensors and redundancy
static FuncSensPair pedalPrimary(1, SensorType::AcceleratorPedalPrimary);
static FuncSensPair pedalSecondary(1, SensorType::AcceleratorPedalSecondary);
static RedundantPair pedal(pedalPrimary, pedalSecondary, SensorType::AcceleratorPedal);

// This sensor indicates the driver's throttle intent - Pedal if we have one, TPS if not.
static ProxySensor driverIntent(SensorType::DriverThrottleIntent);

// These sensors are TPS-like, so handle them in here too
static FuncSensPair wastegate(PACK_MULT_VOLTAGE, SensorType::WastegatePosition);
static FuncSensPair idlePos(PACK_MULT_VOLTAGE, SensorType::IdlePosition);

void initTps() {
	percent_t min = engineConfiguration->tpsErrorDetectionTooLow;
	percent_t max = engineConfiguration->tpsErrorDetectionTooHigh;

	if (!engineConfiguration->consumeObdSensors) {
		bool isFordTps = engineConfiguration->useFordRedundantTps;

		tps1.init(isFordTps, &fordTps1,
			{ engineConfiguration->tps1_1AdcChannel, (float)engineConfiguration->tpsMin, (float)engineConfiguration->tpsMax, min, max },
			{ engineConfiguration->tps1_2AdcChannel, (float)engineConfiguration->tps1SecondaryMin, (float)engineConfiguration->tps1SecondaryMax, min, max }
		);

		tps2.init(isFordTps, &fordTps2,
			{ engineConfiguration->tps2_1AdcChannel, (float)engineConfiguration->tps2Min, (float)engineConfiguration->tps2Max, min, max },
			{ engineConfiguration->tps2_2AdcChannel, (float)engineConfiguration->tps2SecondaryMin, (float)engineConfiguration->tps2SecondaryMax, min, max }
		);

		// Pedal sensors
		pedal.init(false, nullptr,
			{ engineConfiguration->throttlePedalPositionAdcChannel, engineConfiguration->throttlePedalUpVoltage, engineConfiguration->throttlePedalWOTVoltage, min, max },
			{ engineConfiguration->throttlePedalPositionSecondAdcChannel, engineConfiguration->throttlePedalSecondaryUpVoltage, engineConfiguration->throttlePedalSecondaryWOTVoltage, min, max }
		);

		// TPS-like stuff that isn't actually a TPS
		wastegate.init({ engineConfiguration->wastegatePositionSensor, (float)engineConfiguration->wastegatePositionMin, (float)engineConfiguration->wastegatePositionMax, min, max });
		idlePos.init({ engineConfiguration->idlePositionSensor, (float)engineConfiguration->idlePositionMin, (float)engineConfiguration->idlePositionMax, min, max });
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
	tps1.unsubscribe();
	tps2.unsubscribe();
	pedal.unsubscribe();

	wastegate.unsubscribe();
	idlePos.unsubscribe();
}
