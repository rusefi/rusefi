#include "pch.h"
#include "unit_test_framework.h"
#include "board_overrides.h"
#include <iterator>

// Compile the production board hooks against the real configuration structure.
// Keep board entry points separate from the host's own board implementation.
namespace levin {
#include "../../../firmware/config/boards/levin/board_configuration.cpp"
}

TEST(LevinBoard, Defaults) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	levin::levinDefaultConfiguration();
	levin::levinConfigOverrides();

	const Gpio injectors[] = {Gpio::B15, Gpio::A8, Gpio::B13, Gpio::B14,
		Gpio::E13, Gpio::B12, Gpio::E7, Gpio::E10};
	const Gpio coils[] = {Gpio::C13, Gpio::E6, Gpio::E5, Gpio::E4,
		Gpio::E3, Gpio::E2, Gpio::B9, Gpio::D12};
	for (size_t i = 0; i < 8; i++) {
		EXPECT_EQ(injectors[i], engineConfiguration->injectionPins[i]);
		EXPECT_EQ(coils[i], engineConfiguration->ignitionPins[i]);
	}
	for (size_t i = 8; i < std::size(engineConfiguration->injectionPins); i++) {
		EXPECT_EQ(Gpio::Unassigned, engineConfiguration->injectionPins[i]);
		EXPECT_EQ(Gpio::Unassigned, engineConfiguration->ignitionPins[i]);
	}
	EXPECT_EQ(Gpio::D3, engineConfiguration->triggerInputPins[0]);
	EXPECT_EQ(Gpio::D4, engineConfiguration->camInputs[0]);
	EXPECT_EQ(2490, engineConfiguration->clt.config.bias_resistor);
	EXPECT_EQ(2490, engineConfiguration->iat.config.bias_resistor);
	EXPECT_EQ(EFI_ADC_1, engineConfiguration->clt.adcChannel);
	EXPECT_EQ(EFI_ADC_0, engineConfiguration->iat.adcChannel);
	EXPECT_EQ(EFI_ADC_3, engineConfiguration->map.sensor.hwChannel);
	EXPECT_EQ(EFI_ADC_8, engineConfiguration->afr.hwChannel);
	EXPECT_EQ(MT_CUSTOM, engineConfiguration->baroSensor.type);
	EXPECT_NEAR(126.6624f, engineConfiguration->baroSensor.highValue, 0.001f);
}

TEST(LevinBoard, FixedWiringPreservesExternalReassignments) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	levin::levinDefaultConfiguration();
	// Import a foreign tune with different internal connections.
	engineConfiguration->idle.stepperDirectionPin = Gpio::E0;
	engineConfiguration->idle.stepperStepPin = Gpio::E1;
	engineConfiguration->stepperEnablePin = Gpio::E2;
	engineConfiguration->useHbridgesToDriveIdleStepper = true;
	engineConfiguration->useRawOutputToDriveIdleStepper = true;
	engineConfiguration->canRxPin = Gpio::B8;
	engineConfiguration->canTxPin = Gpio::B9;
	engineConfiguration->can2RxPin = Gpio::B12;
	engineConfiguration->can2TxPin = Gpio::B13;
	engineConfiguration->spi3sckPin = Gpio::B3;
	engineConfiguration->spi3misoPin = Gpio::B4;
	engineConfiguration->spi3mosiPin = Gpio::B5;
	engineConfiguration->sdCardCsPin = Gpio::A15;
	engineConfiguration->sdCardSpiDevice = SPI_DEVICE_1;
	engineConfiguration->baroSensor.hwChannel = EFI_ADC_12;
	engineConfiguration->vbattAdcChannel = EFI_ADC_13;
	engineConfiguration->uartConsoleSerialSpeed = 9600;
	// Deliberate user changes to connector channels and sensor calibration.
	engineConfiguration->fanPin = Gpio::E10;
	engineConfiguration->injectionPins[7] = Gpio::Unassigned;
	engineConfiguration->ignitionPins[7] = Gpio::Unassigned;
	engineConfiguration->triggerInputPins[0] = Gpio::B6;
	engineConfiguration->camInputs[0] = Gpio::D13;
	engineConfiguration->idle.solenoidPin = Gpio::D15;
	engineConfiguration->clt.config.bias_resistor = 2700;
	levin::levinConfigOverrides();

	EXPECT_EQ(Gpio::B10, engineConfiguration->idle.stepperDirectionPin);
	EXPECT_EQ(Gpio::B11, engineConfiguration->idle.stepperStepPin);
	EXPECT_EQ(Gpio::A15, engineConfiguration->stepperEnablePin);
	EXPECT_FALSE(engineConfiguration->useHbridgesToDriveIdleStepper);
	EXPECT_FALSE(engineConfiguration->useRawOutputToDriveIdleStepper);
	EXPECT_EQ(Gpio::D0, engineConfiguration->canRxPin);
	EXPECT_EQ(Gpio::D1, engineConfiguration->canTxPin);
	EXPECT_EQ(Gpio::Unassigned, engineConfiguration->can2RxPin);
	EXPECT_EQ(Gpio::Unassigned, engineConfiguration->can2TxPin);
	EXPECT_EQ(Gpio::C10, engineConfiguration->spi3sckPin);
	EXPECT_EQ(Gpio::C11, engineConfiguration->spi3misoPin);
	EXPECT_EQ(Gpio::C12, engineConfiguration->spi3mosiPin);
	EXPECT_EQ(Gpio::D2, engineConfiguration->sdCardCsPin);
	EXPECT_EQ(SPI_DEVICE_3, engineConfiguration->sdCardSpiDevice);
	EXPECT_EQ(EFI_ADC_5, engineConfiguration->baroSensor.hwChannel);
	EXPECT_EQ(EFI_ADC_4, engineConfiguration->vbattAdcChannel);
	EXPECT_EQ(115200, engineConfiguration->uartConsoleSerialSpeed);
	EXPECT_EQ(Gpio::E10, engineConfiguration->fanPin);
	EXPECT_EQ(Gpio::Unassigned, engineConfiguration->injectionPins[7]);
	EXPECT_EQ(Gpio::Unassigned, engineConfiguration->ignitionPins[7]);
	EXPECT_EQ(Gpio::B6, engineConfiguration->triggerInputPins[0]);
	EXPECT_EQ(Gpio::D13, engineConfiguration->camInputs[0]);
	EXPECT_EQ(Gpio::D15, engineConfiguration->idle.solenoidPin);
	EXPECT_EQ(2700, engineConfiguration->clt.config.bias_resistor);
}
