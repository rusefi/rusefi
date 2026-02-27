// file AemXSeriesLambda.h

#pragma once

#include "can_sensor.h"

#include "wideband_state_generated.h"

// Following codes "extend" wbo::Fault codes
#define HACK_INVALID_AEM 8

// todo: static_cast<uint8_t>(Fault::LegacyProtocol);

#define RUSEFI_WIDEBAND_VERSION_MIN	0xA0

class AemXSeriesWideband : public CanSensorBase, public wideband_state_s {
public:
	AemXSeriesWideband(uint8_t sensorIndex, SensorType type);

	bool acceptFrame(const size_t busIndex, const CANRxFrame& frame) const override final;

	void refreshState(void);

	void refreshSmoothedLambda(float lambda);

  bool hasSeenRx = false;

protected:
	// Dispatches to one of the three decoders below
	void decodeFrame(const CANRxFrame& frame, efitick_t nowNt) override;

	// Decode an actual AEM controller, or a rusEFI controller sending AEM format
	bool decodeAemXSeries(const CANRxFrame& frame, efitick_t nowNt);

	// Decode rusEFI custom format
	bool decodeRusefiStandard(const CANRxFrame& frame, efitick_t nowNt);
	void decodeRusefiDiag(const CANRxFrame& frame);

private:
	can_wbo_type_e sensorType() const;
	uint32_t getReCanId() const;
	uint32_t getAemCanId() const;
	bool isHeaterAllowed();

	const uint8_t m_sensorIndex;
	// raw fault code from sensor
	uint8_t m_stateCode;
	// Used by both AEM and rusEFI WBO
	bool m_afrIsValid;
	// Used for AEM sensor only
	bool m_isFault;
	// Last valid packed received, for wbo::Fault::CanSilent state
	efitick_t m_lastUpdate = 0;
};
