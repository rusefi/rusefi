/**
 * @file	electronic_throttle_impl.h
 *
 * @date Dec 7, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

// include the "public" ETB interface
#include "electronic_throttle.h"

#include "sensor.h"
#include "efi_pid.h"
#include "error_accumulator.h"
#include "electronic_throttle_generated.h"
#include "tunerstudio_calibration_channel.h"

/**
 * Hard code ETB update speed.
 * Since this is a safety critical system with no real reason for a user to ever need to change the update rate,
 * it's locked to 500hz, along with the ADC.
 * https://en.wikipedia.org/wiki/Nyquist%E2%80%93Shannon_sampling_theorem
 */
#define ETB_LOOP_FREQUENCY 500
#define DEFAULT_ETB_PWM_FREQUENCY 800

class EtbController : public IEtbController, public electronic_throttle_s {
public:
	bool init(dc_function_e function, DcMotor *motor, pid_s *pidParameters, const ValueProvider3D* pedalMap) override;
	void setIdlePosition(percent_t pos) override;
	void setWastegatePosition(percent_t pos) override;
	void reset(const char *reason) override;

	// Update the controller's state: read sensors, send output, etc
	void update() override;

	// Called when the configuration may have changed.  Controller will
	// reset if necessary.
	void onConfigurationChange(pid_s* previousConfiguration);

	// Print this throttle's status.
	void showStatus();

	// Helpers for individual parts of throttle control
	expected<percent_t> observePlant() override;

	expected<percent_t> getSetpoint() override;
	expected<percent_t> getSetpointEtb();
	expected<percent_t> getSetpointWastegate() const;
	expected<percent_t> getSetpointIdleValve() const;

	expected<percent_t> getOpenLoop(percent_t target) override;
	expected<percent_t> getClosedLoop(percent_t setpoint, percent_t observation) override;
	expected<percent_t> getClosedLoopAutotune(percent_t setpoint, percent_t actualThrottlePosition);

	dc_function_e getFunction() const { return m_function; }

	void checkJam(percent_t setpoint, percent_t observation);

	void setOutput(expected<percent_t> outputValue) override;

	// Used to inspect the internal PID controller's state
	const pid_state_s& getPidState() const override { return m_pid; };

	// Override if this throttle needs special per-throttle adjustment (bank-to-bank trim, for example)
	virtual percent_t getThrottleTrim(float /*rpm*/, percent_t /*targetPosition*/) const {
		return 0;
	}

  // pedal-based part of ETB target, without idle and other interventions
  float getCurrentTarget() const override {
    return etbCurrentTarget;
  }

	bool isEtbMode() const override {
		return m_function == DC_Throttle1 || m_function == DC_Throttle2;
	}

	// Lua throttle adjustment
	void setLuaAdjustment(percent_t adjustment) override;
	float getLuaAdjustment() const;

	float prevOutput = 0;

protected:
	bool hadTpsError = false;
	bool hadPpsError = false;

	DcMotor* getMotor() { return m_motor; }

private:
	dc_function_e m_function = DC_None;
	SensorType m_positionSensor = SensorType::Invalid;
	DcMotor *m_motor = nullptr;
	Pid m_pid;
	bool m_shouldResetPid = false;

	ErrorAccumulator m_targetErrorAccumulator;

	/**
	 * @return true if OK, false if should be disabled
	 */
	bool checkStatus();

	Timer m_jamDetectTimer;

	// Pedal -> target map
	const ValueProvider3D* m_pedalProvider = nullptr;

	float m_idlePosition = 0;

	// This is set if automatic PID cal should be run
	bool m_isAutotune = false;

	// Autotune helpers
	bool m_lastIsPositive = false;
	Timer m_cycleTimer;
	float m_minCycleTps = 0;
	float m_maxCycleTps = 0;
	// Autotune measured parameters: gain and ultimate period
	// These are set to correct order of magnitude starting points
	// so we converge more quickly on the correct values
	float m_a = 8;
	float m_tu = 0.1f;

#if EFI_TUNER_STUDIO
	uint8_t m_autotuneCounter = 0;
	uint8_t m_autotuneCurrentParam = 0;
#endif

	Timer m_luaAdjustmentTimer;

	efitimeus_t lastTickUs;
};

void etbPidReset();

class EtbController1 : public EtbController { };

class EtbController2 : public EtbController {
public:
	EtbController2(const ValueProvider3D& throttle2TrimTable)
		: m_throttle2Trim(throttle2TrimTable)
	{
	}

	percent_t getThrottleTrim(float rpm, percent_t /*targetPosition*/) const override;

private:
	const ValueProvider3D& m_throttle2Trim;
};

template <typename TBase>
class EtbImpl final : public TBase {
private:
	enum class ACPhase {
		Stopped,

		Start,

		// Drive the motor open
		Open,

		// Drive the motor closed
		Close,

		// Write learned values to TS
		TransmitPrimaryMax,
		TransmitPrimaryMin,
		TransmitSecondaryMax,
		TransmitSecondaryMin,
	};

public:
	template <typename... TArgs>
	EtbImpl(TArgs&&... args) : TBase(std::forward<TArgs>(args)...) { }

	void update() override {
#if EFI_TUNER_STUDIO
		if (m_autocalPhase != ACPhase::Stopped) {
			ACPhase nextPhase = doAutocal(m_autocalPhase);

			// if we changed phase, reset the phase timer
			if (m_autocalPhase != nextPhase) {
				m_autocalTimer.reset();
				m_autocalPhase = nextPhase;
			}
		} else
#endif /* EFI_TUNER_STUDIO */

		{
			TBase::update();
		}
	}

	void autoCalibrateTps(bool reportToTs) override {
		// Only auto calibrate throttles
		if (TBase::getFunction() == DC_Throttle1 || TBase::getFunction() == DC_Throttle2 || TBase::getFunction() == DC_Wastegate) {
			m_isAutocalTs = reportToTs;
			m_autocalPhase = ACPhase::Start;
		}
	}

	ACPhase doAutocal(ACPhase phase) {
		// Don't allow if engine is running!
		if (Sensor::getOrZero(SensorType::Rpm) > 0) {
			efiPrintf(" ****************** ERROR: Not while RPM ********************");
			return ACPhase::Stopped;
		}

		auto motor = TBase::getMotor();
		if (!motor) {
			efiPrintf(" ****************** ERROR: No DC motor ********************");
			return ACPhase::Stopped;
		}

		TBase::etbErrorCode = (uint8_t)EtbStatus::AutoCalibrate;

		auto myFunction = TBase::getFunction();

		switch (phase) {
		case ACPhase::Start:
			// Open the throttle
			motor->set(0.5f);
			motor->enable();
			return ACPhase::Open;
		case ACPhase::Open:
			if (m_autocalTimer.hasElapsedMs(1000)) {
				// Capture open position
				m_primaryMax = Sensor::getRaw(functionToTpsSensorPrimary(myFunction));
				m_secondaryMax = Sensor::getRaw(functionToTpsSensorSecondary(myFunction));

				// Next: close the throttle
				motor->set(-0.5f);
				return ACPhase::Close;
			}
			break;
		case ACPhase::Close:
			if (m_autocalTimer.hasElapsedMs(1000)) {
				// Capture closed position
				m_primaryMin = Sensor::getRaw(functionToTpsSensorPrimary(myFunction));
				m_secondaryMin = Sensor::getRaw(functionToTpsSensorSecondary(myFunction));

				// Disable the motor, we're done
				motor->disable("autotune");

				// Check that the calibrate actually moved the throttle
				if (std::abs(m_primaryMax - m_primaryMin) < 0.5f) {
					firmwareError(ObdCode::OBD_TPS_Configuration, "Auto calibrate failed, check your wiring!\r\nClosed voltage: %.1fv Open voltage: %.1fv", m_primaryMin, m_primaryMax);
					return ACPhase::Stopped;
				}

				if (!m_isAutocalTs) {
					if (myFunction == DC_Throttle1) {
						engineConfiguration->tpsMin = m_primaryMin;
						engineConfiguration->tpsMax = m_primaryMax;
						engineConfiguration->tps1SecondaryMin = m_secondaryMin;
						engineConfiguration->tps1SecondaryMax = m_secondaryMax;
					} else if (myFunction == DC_Throttle2) {
						engineConfiguration->tps2Min = m_primaryMin;
						engineConfiguration->tps2Max = m_primaryMax;
						engineConfiguration->tps2SecondaryMin = m_secondaryMin;
						engineConfiguration->tps2SecondaryMax = m_secondaryMax;
					} else if (myFunction == DC_Wastegate) {
						engineConfiguration->wastegatePositionClosedVoltage = m_primaryMin;
						engineConfiguration->wastegatePositionOpenedVoltage = m_primaryMax;
					} else {
						/* TODO */
					}
					return ACPhase::Stopped;
				}

				// Next: start transmitting results
				tsCalibrationSetData(functionToCalModePriMax(myFunction), m_primaryMax);
				return ACPhase::TransmitPrimaryMax;
			}
			break;
		case ACPhase::TransmitPrimaryMax:
			if (tsCalibrationIsIdle()) {
				tsCalibrationSetData(functionToCalModePriMin(myFunction), m_primaryMin);
				return ACPhase::TransmitPrimaryMin;
			}
			break;
		case ACPhase::TransmitPrimaryMin:
			if (tsCalibrationIsIdle()) {
				tsCalibrationSetData(functionToCalModeSecMax(myFunction), m_secondaryMax);
				// No secondary sensor?
				if (tsCalibrationIsIdle())
					return ACPhase::Stopped;
				return ACPhase::TransmitSecondaryMax;
			}
			break;
		case ACPhase::TransmitSecondaryMax:
			if (tsCalibrationIsIdle()) {
				tsCalibrationSetData(functionToCalModeSecMin(myFunction), m_secondaryMin);
				return ACPhase::TransmitSecondaryMin;
			}
			break;
		case ACPhase::TransmitSecondaryMin:
			if (tsCalibrationIsIdle()) {
				// Done!
				return ACPhase::Stopped;
			}
			break;
		case ACPhase::Stopped: break;
		}

		// by default, stay in the same phase
		return phase;
	}

private:
	ACPhase m_autocalPhase = ACPhase::Stopped;
	Timer m_autocalTimer;
	// Report calibrated values to TS, if false - set directly to config
	bool m_isAutocalTs;

	float m_primaryMax;
	float m_secondaryMax;
	float m_primaryMin;
	float m_secondaryMin;
};

extern EtbImpl<EtbController1> etb1;
extern EtbImpl<EtbController2> etb2;

static constexpr electronic_throttle_s const* etbData1_ptr = &etb1;
static constexpr electronic_throttle_s const* etbData2_ptr = &etb2;

// To be used by LogFields, for LiveData fragments see non constexpr getLiveData() in electronic_throttle.cpp
template<typename T, size_t idx>
consteval electronic_throttle_s const* getLiveDataConstexpr() requires std::is_same_v<T, electronic_throttle_s> {
#if EFI_ELECTRONIC_THROTTLE_BODY

	static_assert(idx < ETB_COUNT);

	if constexpr (idx == 0) {
		return etbData1_ptr;
	}

	return etbData2_ptr;
#else
	return nullptr;
#endif
}
