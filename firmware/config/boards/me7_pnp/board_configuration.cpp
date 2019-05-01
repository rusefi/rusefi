/**
 * @file boards/ME7_PNP/board_configuration.cpp
 *
 * @brief In this file we can override engine_configuration.cpp.
 *
 * @author Andrey Belomutskiy, (c) 2012-2019
 */
#include "global.h"
#include "engine.h"
#include "engine_math.h"
#include "allsensors.h"
#include "fsio_impl.h"
#include "engine_configuration.h"
#include "smart_gpio.h"

EXTERN_ENGINE
;

static void
vag_18_Turbo (DECLARE_ENGINE_PARAMETER_SIGNATURE)
{
  setOperationMode (engineConfiguration, FOUR_STROKE_CRANK_SENSOR/*FOUR_STROKE_CAM_SENSOR*/);
  engineConfiguration->trigger.type = TT_TOOTHED_WHEEL_60_2;
  engineConfiguration->useOnlyRisingEdgeForTrigger = true;
  setAlgorithm (LM_SPEED_DENSITY PASS_CONFIG_PARAMETER_SUFFIX);

  engineConfiguration->specs.cylindersCount = 4;
  engineConfiguration->specs.firingOrder = FO_1_3_4_2;
  engineConfiguration->injector.flow = 205;
  engineConfiguration->specs.displacement = 1.839;
  engineConfiguration->globalTriggerAngleOffset = 114;	// the end
  engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS; // IM_WASTED_SPARK
  engineConfiguration->crankingInjectionMode = IM_SEQUENTIAL;
  engineConfiguration->injectionMode = IM_SEQUENTIAL; //IM_BATCH;// IM_SEQUENTIAL;

  setCommonNTCSensor (&engineConfiguration->clt);
  engineConfiguration->clt.config.bias_resistor = 2700;
  setCommonNTCSensor (&engineConfiguration->iat);
  engineConfiguration->iat.config.bias_resistor = 2700;
  engineConfiguration->baroSensor.hwChannel = EFI_ADC_NONE;
  engineConfiguration->throttlePedalPositionAdcChannel = EFI_ADC_NONE;
  engineConfiguration->canNbcType = CAN_BUS_NBC_VAG;
  engineConfiguration->canReadEnabled = true;
  engineConfiguration->canWriteEnabled = true;
  boardConfiguration->canDeviceMode = CD_USE_CAN1;
  boardConfiguration->canTxPin = GPIOB_6;
  boardConfiguration->canRxPin = GPIOB_12;

  engineConfiguration->tpsAdcChannel = EFI_ADC_4; // PA2
  engineConfiguration->map.sensor.hwChannel = EFI_ADC_7;
  engineConfiguration->clt.adcChannel = EFI_ADC_6;
  engineConfiguration->iat.adcChannel = EFI_ADC_2;
  engineConfiguration->afr.hwChannel = EFI_ADC_NONE;
  engineConfiguration->throttlePedalPositionAdcChannel = EFI_ADC_13;

}
void
setPinConfigurationOverrides (void)
{
  boardConfiguration->injectionPins[0] = GPIOG_15; // #1
  boardConfiguration->injectionPins[1] = GPIOG_14; // #2
  boardConfiguration->injectionPins[2] = GPIOG_9; // #3
  boardConfiguration->injectionPins[3] = GPIOC_9; // #4
  boardConfiguration->injectionPins[4] = GPIOE_0; // #5
  boardConfiguration->injectionPins[5] = GPIOE_1; // #6
  boardConfiguration->injectionPins[6] = GPIOB_9; // #7
  boardConfiguration->injectionPins[7] = GPIOB_8; // #8

  boardConfiguration->ignitionPins[0] = GPIOG_3;
  boardConfiguration->ignitionPins[1] = GPIOG_4;
  boardConfiguration->ignitionPins[2] = GPIOG_5;
  boardConfiguration->ignitionPins[3] = GPIOG_6;
  boardConfiguration->ignitionPins[4] = GPIOG_8;
  boardConfiguration->ignitionPins[5] = GPIOC_7;
  boardConfiguration->ignitionPins[6] = GPIOC_6;
  boardConfiguration->ignitionPins[7] = GPIOG_7;
  boardConfiguration->ignitionPins[8] = GPIO_UNASSIGNED;
  boardConfiguration->ignitionPins[9] = GPIO_UNASSIGNED;

  boardConfiguration->fuelPumpPin = TLE8888_PIN_20;
  boardConfiguration->mainRelayPin = GPIO_UNASSIGNED;
  boardConfiguration->idle.solenoidPin = GPIO_UNASSIGNED;
  boardConfiguration->fanPin = GPIO_UNASSIGNED;
  boardConfiguration->clutchDownPin = GPIOD_11;



}
void
setBoardConfigurationOverrides (void)
{

	 boardConfiguration->ignitionPinMode = OM_INVERTED;
	  boardConfiguration->injectionPinMode = OM_OPENDRAIN_INVERTED;

	  setAlgorithm (LM_SPEED_DENSITY PASS_CONFIG_PARAMETER_SUFFIX);

	  boardConfiguration->fsio_setting[1] = 6500; // #2 RPM threshold
	  boardConfiguration->fsio_setting[2] = 105; // #3 CLT threshold
	  boardConfiguration->fsio_setting[3] = 12.0; // #4 voltage threshold

	  boardConfiguration->is_enabled_spi_1 = true;
	   boardConfiguration->is_enabled_spi_2 = false;
	   boardConfiguration->is_enabled_spi_3 = true;
	   engineConfiguration->tle8888spiDevice = SPI_DEVICE_1;




  // not used
  engineConfiguration->dizzySparkOutputPin = GPIO_UNASSIGNED;
  engineConfiguration->externalKnockSenseAdc = EFI_ADC_NONE;
  engineConfiguration->displayMode = DM_NONE;
  boardConfiguration->HD44780_rs = GPIO_UNASSIGNED;
  boardConfiguration->HD44780_e = GPIO_UNASSIGNED;
  boardConfiguration->HD44780_db4 = GPIO_UNASSIGNED;
  boardConfiguration->HD44780_db5 = GPIO_UNASSIGNED;
  boardConfiguration->HD44780_db6 = GPIO_UNASSIGNED;
  boardConfiguration->HD44780_db7 = GPIO_UNASSIGNED;
  for (int i = 0; i < DIGIPOT_COUNT; i++)
    {
      boardConfiguration->digitalPotentiometerChipSelect[i] = GPIO_UNASSIGNED;
    }
  boardConfiguration->triggerSimulatorPins[1] = GPIO_UNASSIGNED;
  boardConfiguration->triggerSimulatorPins[2] = GPIO_UNASSIGNED;
  boardConfiguration->triggerSimulatorPinModes[1] = OM_DEFAULT;
  boardConfiguration->triggerSimulatorPinModes[2] = OM_DEFAULT;
  boardConfiguration->vehicleSpeedSensorInputPin = GPIO_UNASSIGNED;
  boardConfiguration->boardTestModeJumperPin = GPIO_UNASSIGNED;
  boardConfiguration->acRelayPin = GPIO_UNASSIGNED;
  boardConfiguration->digitalPotentiometerSpiDevice = SPI_NONE;
  boardConfiguration->max31855spiDevice = SPI_NONE;
  boardConfiguration->acRelayPin = GPIO_UNASSIGNED;
  boardConfiguration->digitalPotentiometerSpiDevice = SPI_NONE;
  boardConfiguration->max31855spiDevice = SPI_NONE;
  engineConfiguration->warningLedPin = GPIO_UNASSIGNED;
  engineConfiguration->runningLedPin = GPIO_UNASSIGNED;
  engineConfiguration->warningLedPin = GPIO_UNASSIGNED;
  boardConfiguration->useStepperIdle = false;
  boardConfiguration->idle.stepperDirectionPin = GPIO_UNASSIGNED;
  boardConfiguration->idle.stepperStepPin = GPIO_UNASSIGNED;
  engineConfiguration->stepperEnablePin = GPIO_UNASSIGNED;
  engineConfiguration->stepperEnablePinMode = OM_DEFAULT;
  boardConfiguration->triggerSimulatorPins[0] = GPIO_UNASSIGNED;
  boardConfiguration->triggerSimulatorPins[1] = GPIO_UNASSIGNED;
  boardConfiguration->triggerSimulatorPins[2] = GPIO_UNASSIGNED;
  boardConfiguration->triggerSimulatorPinModes[1] = OM_DEFAULT;
  boardConfiguration->triggerSimulatorPinModes[2] = OM_DEFAULT;
  boardConfiguration->vehicleSpeedSensorInputPin = GPIO_UNASSIGNED;
  boardConfiguration->boardTestModeJumperPin = GPIO_UNASSIGNED;

  boardConfiguration->injectionPins[8] = GPIO_UNASSIGNED;
  boardConfiguration->injectionPins[9] = GPIO_UNASSIGNED;
  boardConfiguration->injectionPins[10] = GPIO_UNASSIGNED;
  boardConfiguration->injectionPins[11] = GPIO_UNASSIGNED;

  /////////////////////////////////////////////////////////

#if EFI_FSIO
  setFsio (12, GPIOF_12, "0" PASS_ENGINE_PARAMETER_SUFFIX);
  setFsio (14, GPIOF_13, "1" PASS_ENGINE_PARAMETER_SUFFIX);
#endif
  CONFIG(etb1_use_two_wires) = true;
  CONFIGB(etb1.directionPin1) = GPIOF_15;
  CONFIGB(etb1.directionPin2) = GPIOF_14;
  boardConfiguration->isHip9011Enabled = false;

  // ETB #2
  // DIS PE5
  // EN  PE6
  // IN1 PE2
  // IN2 PE4
  // SF  PE3
#if EFI_FSIO
  setFsio (13, GPIOE_5, "0" PASS_ENGINE_PARAMETER_SUFFIX);
  setFsio (15, GPIOE_6, "1" PASS_ENGINE_PARAMETER_SUFFIX);
#endif
  CONFIG(etb2_use_two_wires) = true;
  CONFIG(etb2.directionPin1) = GPIOE_2;
  CONFIG(etb2.directionPin2) = GPIOE_4;

  engineConfiguration->etb.pFactor = 1.07;
  engineConfiguration->etb.iFactor = 0.18;
  engineConfiguration->etb.dFactor = 0.24;
  engineConfiguration->etb.offset = 80;
  engineConfiguration->etb_iTermMin = -300;
  engineConfiguration->etb_iTermMax = 300;

  engineConfiguration->activateAuxPid1 = true; // todo: remove this field?
  engineConfiguration->auxPidFrequency[0] = 300;
  engineConfiguration->auxPidPins[0] = GPIOE_3; // VVT solenoid control
  //	/**
  //	 * set_fsio_setting 1 0.55
  //	 */
  boardConfiguration->fsio_setting[0] = 0.0;
//	setFsioExt(0, GPIOE_3, "0 fsio_setting", 400 PASS_ENGINE_PARAMETER_SUFFIX);
  boardConfiguration->fsio_setting[1] = 6500; // #2 RPM threshold
  boardConfiguration->fsio_setting[2] = 105; // #3 CLT threshold
  boardConfiguration->fsio_setting[3] = 12.0; // #4 voltage threshold

}

void
setSerialConfigurationOverrides (void)
{
  boardConfiguration->useSerialPort = true;
  engineConfiguration->binarySerialTxPin = GPIOB_10;
  engineConfiguration->binarySerialRxPin = GPIOB_11;
  engineConfiguration->consoleSerialTxPin = GPIOB_10;
  engineConfiguration->consoleSerialRxPin = GPIOB_11;

}

void
setSdCardConfigurationOverrides (void)
{
}

