#pragma once

#include "can_sensor.h"

#include "wideband_state_generated.h"

// Following codes "extend" wbo::Fault codes
// "silent" of wboFaultCodeList
#define HACK_SILENT_VALUE 1
// todo: suggest values 1 and 2 into the official WB source fault enum?
#define HACK_CRANKING_VALUE 2
// todo: static_cast<uint8_t>(Fault::LegacyProtocol);
#define HACK_VALID_AEM 7
#define HACK_INVALID_AEM 8
#define HACK_INVALID_RE	9

class AemXSeriesWideband : public CanSensorBase, public wideband_state_s {
public:
	AemXSeriesWideband(uint8_t sensorIndex, SensorType type);

	bool acceptFrame(const CANRxFrame& frame) const override final;

	void refreshState(void);

protected:
	// Dispatches to one of the three decoders below
	void decodeFrame(const CANRxFrame& frame, efitick_t nowNt) override;

	// Decode an actual AEM controller, or a rusEFI controller sending AEM format
	bool decodeAemXSeries(const CANRxFrame& frame, efitick_t nowNt);

	// Decode rusEFI custom format
	void decodeRusefiStandard(const CANRxFrame& frame, efitick_t nowNt);
	void decodeRusefiDiag(const CANRxFrame& frame);

private:
	bool isHeaterAllowed();

	const uint8_t m_sensorIndex;
	// raw fault code from sensor
	uint8_t m_faultCode;
	bool m_isAem;
	bool m_isValid;
	// Used for AEM sensor only
	bool m_isFault;
};
