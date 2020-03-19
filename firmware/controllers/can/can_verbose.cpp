
#include "globalaccess.h"
#if EFI_CAN_SUPPORT

#include "engine.h"

#include "scaled_channel.h"
#include "can_msg_tx.h"
#include "sensor.h"
#include "allsensors.h"
#include "fuel_math.h"

EXTERN_ENGINE;

struct Speeds {
    uint16_t rpm;
    scaled_angle timing;
    scaled_channel<uint8_t, 2> injDuty;
    uint8_t pad[3];
};

static void populateFrame(Speeds& msg) {
    auto rpm = GET_RPM();
    msg.rpm = rpm;

    auto timing = engine->engineState.timingAdvance;
    msg.timing = timing > 360 ? timing - 720 : timing;

    msg.injDuty = getInjectorDutyCycle(rpm);
}

struct PedalAndTps {
    scaled_percent pedal;
    scaled_percent tps1;
    scaled_percent tps2;
    uint8_t pad[2];
};

static void populateFrame(PedalAndTps& msg)
{
    msg.pedal = getPedalPosition();
    msg.tps1 = Sensor::get(SensorType::Tps1).value_or(-1);
    msg.tps2 = Sensor::get(SensorType::Tps2).value_or(-1);
}

struct Temperatures {
    scaled_temperature clt;
    scaled_temperature iat;
    scaled_temperature aux1;
    scaled_temperature aux2;
};

static void populateFrame(Temperatures& msg) {
    msg.clt = getCoolantTemperature();
    msg.iat = getIntakeAirTemperature();

    // todo: does aux even work?
    msg.aux1 = 0;
    msg.aux2 = 0;
}

struct Sensors {
    scaled_afr afr;
    scaled_pressure oilPressure;
    scaled_angle vvtPos;
    scaled_voltage vbatt;
};

static void populateFrame(Sensors& msg) {
    msg.afr = getAfr();
    msg.oilPressure = Sensor::get(SensorType::OilPressure).value_or(-1);
    msg.vvtPos = engine->triggerCentral.getVVTPosition();
    msg.vbatt = getVBatt();
}

struct EngineLoad {
    scaled_pressure map;
    scaled_channel<uint16_t, 1000> cylAirmass;
    scaled_channel<uint16_t, 100> estAirflow;

    uint8_t pad[2];
};

static void populateFrame(EngineLoad& msg) {
    msg.map = getMap();
    msg.cylAirmass = engine->engineState.sd.airMassInOneCylinder;
    msg.estAirflow = engine->engineState.airFlow;
}

struct Fueling {
    scaled_ms fuel_pulse;
    scaled_percent stft;
    uint8_t pad[4];
};

static void populateFrame(Fueling& msg) {
    msg.fuel_pulse = ENGINE(actualLastInjection);

    // todo
    msg.stft = 0;
}

void sendCanVerbose() {
    auto base = CONFIG(verboseCanBaseAddress);

    transmitStruct<Speeds>      (base + 0);
    transmitStruct<PedalAndTps> (base + 1);
    transmitStruct<Temperatures>(base + 2);
    transmitStruct<Sensors>     (base + 3);
    transmitStruct<EngineLoad>  (base + 4);
    transmitStruct<Fueling>     (base + 5);
}

#endif // EFI_CAN_SUPPORT
