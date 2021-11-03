/*
 * @file high_pressure_fuel_pump.cpp
 *
 *
 * todo: there is some similarity with aux_valves.cpp and even map_averaging.cpp maybe reduce code duplication?
 *
 * @date Nov 5, 2020
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "high_pressure_fuel_pump.h"
#include "spark_logic.h"

//#if EFI_HPFP
#if 1

class HpfpActor {
public:
    uint8_t m_lobe_index;
    float m_I_sum;
    float m_pressureTarget;

    AngleBasedEvent m_open;
    AngleBasedEvent m_close;

	DECLARE_ENGINE_PTR;

    void start();

private:
    void scheduleNextCycle();
    angle_t findNextLobe();
    float calcFuelPercent(int rpm);
    float calcPI(int rpm, float calc_fuel_percent);

    static void hpfpPinTurnOn(HpfpActor *) {
        enginePins.hpfpValve.setHigh();
    }

    static void hpfpPinTurnOff(HpfpActor *current) {
        RegisteredNamedOutputPin *output = &enginePins.hpfpValve;
        output->setLow();

        current->scheduleNextCycle();
    }
};

static HpfpActor actor; // TODO: Instantiate one per bank? Need more sensors....

template<int TColNum, int TRowNum, typename vType, typename kType,
         typename TValueMultiplier = efi::ratio<1>>
auto makeMap3D(vType table[TRowNum][TColNum], kType rowBins[TRowNum], kType columnBins[TColNum]) {
    Map3D<TColNum, TRowNum, vType, kType, TValueMultiplier> map;
    map.init(table, rowBins, columnBins);
    return map;
}

static auto getTargetPressureMap()
{
    // TODO: Not sure why it's necessary to pass the size, it should be derivable.  Maybe has to do with C-style array instead of std::array -- investigate converting over
    return makeMap3D<HPFP_TARGET_SIZE, HPFP_TARGET_SIZE>(
        engineConfiguration->hpfpTarget,
        engineConfiguration->hpfpTargetRpmBins,
        engineConfiguration->hpfpTargetLoadBins);
}

static auto getCompensationMap()
{
    return makeMap3D<HPFP_COMPENSATION_SIZE, HPFP_COMPENSATION_SIZE>(
        engineConfiguration->hpfpCompensation,
        engineConfiguration->hpfpCompensationRpmBins,
        engineConfiguration->hpfpCompensationLoadBins);
}

angle_t HpfpActor::findNextLobe() {
    // Which lobe are we on?
    int next_index = m_lobe_index + 1;
    // Note, this will be insufficient if the # of cam lobes is
    // dynamically changed rapidly by more than 2x, but it will
    // correct itself rather quickly.
    if (next_index >= engineConfiguration->hpfpCamLobes) {
        next_index -= engineConfiguration->hpfpCamLobes;
    }
    m_lobe_index = next_index;

    // Calculate impact of VVT
    angle_t vvt = 0;
    if (engineConfiguration->hpfpCam != HPFP_CAM_NONE) {
        // TODO: Is the sign correct here?  + means ATDC?
        vvt = engine->triggerCentral.getVVTPosition(
            (engineConfiguration->hpfpCam - 1) / 2 & 1, // Bank
            (engineConfiguration->hpfpCam - 1) & 1); // Cam  TODO: 0=intake?
    }

    return engineConfiguration->hpfpPeakPos + vvt +
        next_index * 720 / engineConfiguration->hpfpCamLobes;
}

// As a percent of the full pump stroke
float HpfpActor::calcFuelPercent(int rpm) {
    float fuel_requested_cc_per_cycle =
        ENGINE(injectionMass)[0] * (1.f / 0.72f) /* TODO: g/cc fuel density, copied from convertToGramsPerSecond, should be moved to config for different fuel types? */  * engineConfiguration->specs.cylindersCount; // TODO: handle multiple banks?
    float fuel_requested_cc_per_lobe = fuel_requested_cc_per_cycle / engineConfiguration->hpfpCamLobes;
    return 100.f *
        fuel_requested_cc_per_lobe / engineConfiguration->hpfpPumpVolume +
        getCompensationMap().getValue(rpm, fuel_requested_cc_per_lobe);
}

float HpfpActor::calcPI(int rpm, float calc_fuel_percent) {
    m_pressureTarget = std::max<float> (
        m_pressureTarget - engineConfiguration->hpfpTargetDecay, // TODO: decay at dt, not per lobe
        getTargetPressureMap().getValue(rpm,
                                        Sensor::get(SensorType::Map).Value)); // TODO: allow other load axis, like we claim to
    float pressureError = m_pressureTarget - Sensor::get(SensorType::FuelPressureHigh).Value;

    float p_control = pressureError * engineConfiguration->hpfpPidP;
    float i_control = m_I_sum + pressureError * engineConfiguration->hpfpPidI;
    i_control = clampF(-calc_fuel_percent, i_control, 100.f - calc_fuel_percent);
    m_I_sum = i_control;
    return p_control + i_control;
}

void HpfpActor::scheduleNextCycle() {
#if EFI_UNIT_TEST
    Engine *engine = engine;
	EXPAND_Engine;
#endif /* EFI_UNIT_TEST */

    // TODO: All this math should be moved to a slower task/thread/callback/etc

    angle_t lobe = findNextLobe();

    // Pressure current/target calculation
    int rpm = ENGINE(rpmCalculator).getRpm();

    // Math based on fuel requested
    float fuel_requested_percent = calcFuelPercent(rpm);

    // Apply PI control
    fuel_requested_percent += calcPI(rpm, fuel_requested_percent);

    // Convert to degrees
    float angle_requested = interpolate2d(fuel_requested_percent,
                                          engineConfiguration->hpfpLobeProfileQuantityBins,
                                          engineConfiguration->hpfpLobeProfileAngle);

    //float deadtime = engineConfiguration->hpfpDeadtime(); // TODO: should be ms not angle

	// very basic control strategy
    if (angle_requested > engineConfiguration->hpfpMinAngle) {
        scheduleOrQueue(&m_open, TRIGGER_EVENT_UNDEFINED, getTimeNowNt(),
                        lobe - angle_requested, // TODO: handle 0 underflow?
                        {hpfpPinTurnOn, this }
                        PASS_ENGINE_PARAMETER_SUFFIX);
	}

    // Always schedule this, even if we aren't opening the valve this
    // time, since this will schedule the next lobe.
    scheduleOrQueue(&m_close, TRIGGER_EVENT_UNDEFINED, getTimeNowNt(),
                    lobe - angle_requested + engineConfiguration->hpfpActivationAngle, // TODO: handle 0 underflow
                    { hpfpPinTurnOff, this }
                    PASS_ENGINE_PARAMETER_SUFFIX);
}

void HpfpActor::start()
{
    m_lobe_index = 0;
    m_I_sum = 0;
    m_pressureTarget = 0;
    scheduleNextCycle();
}

void initHPFP(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	if (!isBrainPinValid(engineConfiguration->hpfpValvePin)) {
		return;
	}

    INJECT_ENGINE_REFERENCE(&actor);
    actor.start();
}

#endif // EFI_HPFP
