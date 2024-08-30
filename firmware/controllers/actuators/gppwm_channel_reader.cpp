//
// Created by kifir on 5/15/24.
//

#include "pch.h"

#include "gppwm_channel_reader.h"

expected<float> readGppwmChannel(gppwm_channel_e channel) {
	switch (channel) {
	case GPPWM_Zero:
		return 0;
	case GPPWM_Rpm:
		return Sensor::get(SensorType::Rpm);
	case GPPWM_Tps:
		return Sensor::get(SensorType::Tps1);
	case GPPWM_Map:
		return Sensor::get(SensorType::Map);
	case GPPWM_Clt:
		return Sensor::get(SensorType::Clt);
	case GPPWM_Iat:
		return Sensor::get(SensorType::Iat);
	case GPPWM_LuaGauge1:
		return Sensor::get(SensorType::LuaGauge1);
	case GPPWM_LuaGauge2:
		return Sensor::get(SensorType::LuaGauge2);
	case GPPWM_FuelLoad:
		return getFuelingLoad();
	case GPPWM_IgnLoad:
		return getIgnitionLoad();
	case GPPWM_AuxTemp1:
		return Sensor::get(SensorType::AuxTemp1);
	case GPPWM_AuxTemp2:
		return Sensor::get(SensorType::AuxTemp2);
	case GPPWM_AccelPedal:
		return Sensor::get(SensorType::AcceleratorPedal);
	case GPPWM_Vbatt:
		return Sensor::get(SensorType::BatteryVoltage);
#if EFI_SHAFT_POSITION_INPUT
	case GPPWM_VVT_1I:
		return engine->triggerCentral.getVVTPosition(/*bankIndex*/0, /*camIndex*/0);
	case GPPWM_VVT_1E:
		return engine->triggerCentral.getVVTPosition(/*bankIndex*/0, /*camIndex*/1);
	case GPPWM_VVT_2I:
		return engine->triggerCentral.getVVTPosition(/*bankIndex*/1, /*camIndex*/0);
	case GPPWM_VVT_2E:
		return engine->triggerCentral.getVVTPosition(/*bankIndex*/1, /*camIndex*/1);
#else
	case GPPWM_VVT_1I:
	case GPPWM_VVT_1E:
	case GPPWM_VVT_2I:
	case GPPWM_VVT_2E:
		return 0;
#endif // EFI_SHAFT_POSITION_INPUT
	case GPPWM_EthanolPercent:
		return Sensor::get(SensorType::FuelEthanolPercent);
	case GPPWM_AuxLinear1:
		return Sensor::get(SensorType::AuxLinear1);
	case GPPWM_AuxLinear2:
		return Sensor::get(SensorType::AuxLinear2);
	case GPPWM_AuxLinear3:
		return Sensor::get(SensorType::AuxLinear3);
	case GPPWM_AuxLinear4:
		return Sensor::get(SensorType::AuxLinear4);
	case GPPWM_GppwmOutput1:
		return (float)engine->outputChannels.gppwmOutput[0];
	case GPPWM_GppwmOutput2:
		return (float)engine->outputChannels.gppwmOutput[1];
	case GPPWM_GppwmOutput3:
		return (float)engine->outputChannels.gppwmOutput[2];
	case GPPWM_GppwmOutput4:
		return (float)engine->outputChannels.gppwmOutput[3];
	case GPPWM_DetectedGear:
#if EFI_VEHICLE_SPEED
		return Sensor::get(SensorType::DetectedGear);
#else
    	return 0;
#endif // EFI_VEHICLE_SPEED
	case GPPWM_BaroPressure:
		return Sensor::get(SensorType::BarometricPressure);
	case GPPWM_Egt1:
		return Sensor::get(SensorType::EGT1);
	case GPPWM_Egt2:
		return Sensor::get(SensorType::EGT2);
	}

	return unexpected;
}
