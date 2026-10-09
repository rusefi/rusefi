#include "pch.h"
#include "board_overrides.h"

Gpio getCommsLedPin() { return Gpio::Unassigned; }
Gpio getRunningLedPin() { return Gpio::Unassigned; }
Gpio getWarningLedPin() { return Gpio::Unassigned; }

// These connections are physical PCB wiring, not user-assignable outputs.
static void levinConfigOverrides() {
    engineConfiguration->idle.stepperDirectionPin = Gpio::B10;
    engineConfiguration->idle.stepperStepPin = Gpio::B11;
    engineConfiguration->stepperEnablePin = Gpio::A15;
    engineConfiguration->stepperEnablePinMode = OM_DEFAULT;
    engineConfiguration->useHbridgesToDriveIdleStepper = false;
    engineConfiguration->useRawOutputToDriveIdleStepper = false;
    engineConfiguration->uartConsoleSerialSpeed = 115200;
    engineConfiguration->canRxPin = Gpio::D0;
    engineConfiguration->canTxPin = Gpio::D1;
    engineConfiguration->can2RxPin = Gpio::Unassigned;
    engineConfiguration->can2TxPin = Gpio::Unassigned;
    engineConfiguration->spi3sckPin = Gpio::C10;
    engineConfiguration->spi3misoPin = Gpio::C11;
    engineConfiguration->spi3mosiPin = Gpio::C12;
    engineConfiguration->is_enabled_spi_3 = true;
    engineConfiguration->sdCardSpiDevice = SPI_DEVICE_3;
    engineConfiguration->sdCardCsPin = Gpio::D2;
    engineConfiguration->sdCardCsPinMode = OM_DEFAULT;
    engineConfiguration->vbattAdcChannel = EFI_ADC_4;
    engineConfiguration->baroSensor.hwChannel = EFI_ADC_5;
}

static void levinDefaultConfiguration() {
    // Suggested assignments only: external channels remain reassignable.
    const Gpio injectors[] = {Gpio::B15, Gpio::A8, Gpio::B13, Gpio::B14,
        Gpio::E13, Gpio::B12, Gpio::E7, Gpio::E10};
    const Gpio coils[] = {Gpio::C13, Gpio::E6, Gpio::E5, Gpio::E4,
        Gpio::E3, Gpio::E2, Gpio::B9, Gpio::D12};
    for (auto &pin : engineConfiguration->injectionPins) { pin = Gpio::Unassigned; }
    for (auto &pin : engineConfiguration->ignitionPins) { pin = Gpio::Unassigned; }
    for (unsigned i = 0; i < 8; i++) {
        engineConfiguration->injectionPins[i] = injectors[i];
        engineConfiguration->ignitionPins[i] = coils[i];
    }
    engineConfiguration->triggerInputPins[0] = Gpio::D3;
    engineConfiguration->triggerInputPins[1] = Gpio::Unassigned;
    engineConfiguration->camInputs[0] = Gpio::D4;
    engineConfiguration->clt.config.bias_resistor = 2490;
    engineConfiguration->iat.config.bias_resistor = 2490;
    engineConfiguration->clt.adcChannel = EFI_ADC_1;
    engineConfiguration->iat.adcChannel = EFI_ADC_0;
    engineConfiguration->tps1_1AdcChannel = EFI_ADC_2;
    engineConfiguration->map.sensor.hwChannel = EFI_ADC_3;
    engineConfiguration->afr.hwChannel = EFI_ADC_8;
    engineConfiguration->fuelPumpPin = Gpio::E11;
    engineConfiguration->fanPin = Gpio::E9;
    engineConfiguration->tachOutputPin = Gpio::E8;
    engineConfiguration->idle.solenoidPin = Gpio::D10;
    engineConfiguration->secondSolenoidPin = Gpio::D9;
    engineConfiguration->uartConsoleSerialSpeed = 115200;
    engineConfiguration->useStepperIdle = false;
    engineConfiguration->stepperDirectionPinMode = OM_INVERTED;
    engineConfiguration->adcVcc = 3.3f;
    engineConfiguration->analogInputDividerCoefficient = 1.5151515f;
    engineConfiguration->vbattDividerCoeff = 7.0f;
    engineConfiguration->baroSensor.type = MT_CUSTOM;
    engineConfiguration->baroSensor.lowValue = 0;
    engineConfiguration->baroSensor.highValue = 1.0f / 0.007895f;
    engineConfiguration->is_enabled_spi_1 = false;
    engineConfiguration->is_enabled_spi_2 = false;
    engineConfiguration->triggerSimulatorPins[0] = Gpio::Unassigned;
    engineConfiguration->triggerSimulatorPins[1] = Gpio::Unassigned;
    setDefaultSdCardParameters();
}

void setup_custom_board_overrides() {
    custom_board_DefaultConfiguration = levinDefaultConfiguration;
    custom_board_ConfigOverrides = levinConfigOverrides;
}
