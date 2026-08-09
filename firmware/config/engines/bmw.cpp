#include "pch.h"
#include "bmw.h"
#include "defaults.h"

#ifdef HW_HELLEN
#include "hellen_meta.h"
#endif // HW_HELLEN

void bmwM52() {
    engineConfiguration->ignitionMode = IM_WASTED_SPARK;
    engineConfiguration->cylindersCount = 6;
    engineConfiguration->displacement = 3;
    engineConfiguration->firingOrder = FO_1_5_3_6_2_4;
}


/**
 DISA
 200Hz
 Close Flap ~20% Duty Cycle Motor holds flap Closed
 Open Flap ~80% Duty CycleMotor holds flap Open
 Default / Idle Fault0% or 100% Duty CycleFail-safe / Inactive state

 Low RPM: Both flaps closed to create long runners for optimal low-end torque.

 Mid RPM: Large flap opens to transition to medium runner length.

 High RPM: Both flaps open to maximize airflow through short, direct paths for top-end horsepower.

 */
void bmwN52() {
    engineConfiguration->canNbcType = CAN_BUS_BMW_E90;

    strcpy(engineConfiguration->engineMake, ENGINE_MAKE_BMW);
    strcpy(engineConfiguration->engineCode, "N52");
    engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS;
    engineConfiguration->cylindersCount = 6;
    engineConfiguration->displacement = 3;
    engineConfiguration->firingOrder = FO_1_5_3_6_2_4;
    engineConfiguration->trigger.type = trigger_type_e::TT_TOOTHED_WHEEL_60_2;
    engineConfiguration->globalTriggerAngleOffset = 90;

    engineConfiguration->vvtMode[0] = VVT_BMW_VANOS_RELUCTOR;
    engineConfiguration->vvtMode[1] = VVT_BMW_VANOS_RELUCTOR;

#ifdef HW_HELLEN_SUPER_UAEFI
    engineConfiguration->map.sensor.hwChannel = MM100_IN_MAP1_ANALOG;
//     engineConfiguration->vehicleSpeedSensorInputPin = Gpio::Unassigned;
// 	  engineConfiguration->camInputs[1] = Gpio::MM100_IN_D3;
#endif // HW_HELLEN_SUPER_UAEFI

    // TPS/PPS calibration captured from a real N52 throttle body + pedal (see "super N52" reference tune).
    // TPS min/max are stored as 10-bit ADC counts, so convert the measured volts.
    // Throttle 1: primary min/max, then secondary min/max.
    setTPS1Calibration(convertVoltageTo10bitADC(4.545), convertVoltageTo10bitADC(0.58),
                       convertVoltageTo10bitADC(0.75), convertVoltageTo10bitADC(4.72));
    // Pedal: primary up/WOT, then secondary up/WOT (volts).
    setPPSCalibration(0.625, 2.230, 0.947, 4.197);
}
