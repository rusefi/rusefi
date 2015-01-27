#include "rusefi_types.h"

typedef struct {
	/**
	 * offset 0
	 */
	float baseFuel;
	/**
	 * This value controls what RPM values we consider 'cranking' (any RPM below 'crankingRpm')
	 * Anything above 'crankingRpm' would be 'running'
	 * offset 4
	 */
	int16_t rpm;
	/**
	 * need 4 byte alignment
	 * offset 6
	 */
	uint8_t alignmentFill[2];
	/** total size 8*/
} cranking_parameters_s;

typedef struct {
	/**
	 * kPa value at zero volts
	 * offset 0
	 */
	float valueAt0;
	/**
	 * kPa value at 5 volts
	 * offset 4
	 */
	float valueAt5;
	/**
	 * offset 8
	 */
	air_pressure_sensor_type_e type;
	/**
	 * offset 12
	 */
	adc_channel_e hwChannel;
	/** total size 16*/
} air_pressure_sensor_config_s;

/**
 * @brief MAP averaging configuration

 */
typedef struct {
	/**
	 * offset 0
	 */
	float samplingAngleBins[MAP_ANGLE_SIZE];
	/**
	 * @brief MAP averaging sampling start angle, by RPM
	 * offset 32
	 */
	float samplingAngle[MAP_ANGLE_SIZE];
	/**
	 * offset 64
	 */
	float samplingWindowBins[MAP_WINDOW_SIZE];
	/**
	 * @brief MAP averaging angle duration, by RPM
	 * offset 96
	 */
	float samplingWindow[MAP_WINDOW_SIZE];
	/**
	 * offset 128
	 */
	air_pressure_sensor_config_s sensor;
	/** total size 144*/
} MAP_sensor_config_s;

/**
 * @brief Trigger wheel(s) configuration

 */
typedef struct {
	/**
	 * offset 0
	 */
	trigger_type_e type;
	/**
	 * offset 4
	 */
	int customIsSynchronizationNeeded;
	/**
	 * offset 8
	 */
	int customTotalToothCount;
	/**
	 * offset 12
	 */
	int customSkippedToothCount;
	/**
	 * offset 16
	 */
	float customSyncRatioFrom;
	/**
	 * offset 20
	 */
	float customSyncRatioTo;
	/**
	 * offset 24
	 */
	int customUseRiseEdge;
	/** total size 28*/
} trigger_config_s;

typedef struct {
	/**
	 * offset 0
	*/
	adc_channel_e afrAdcChannel;
	/**
	 * offset 4
	*/
	float v1;
	/**
	 * offset 8
	*/
	float value1;
	/**
	 * offset 12
	*/
	float v2;
	/**
	 * offset 16
	*/
	float value2;
/** total size 20*/
} afr_sensor_s;

typedef struct {
	/**
	 * offset 0
	*/
	brain_pin_e idleValvePin;
	/**
	 * offset 4
	*/
	pin_output_mode_e idleValvePinMode;
	/**
	 * offset 8
	*/
	brain_pin_e fuelPumpPin;
	/**
	 * offset 12
	*/
	pin_output_mode_e fuelPumpPinMode;
	/**
	 * offset 16
	*/
	brain_pin_e injectionPins[INJECTION_PIN_COUNT];
	/**
	 * offset 64
	*/
	pin_output_mode_e injectionPinMode;
	/**
	 * offset 68
	*/
	brain_pin_e ignitionPins[IGNITION_PIN_COUNT];
	/**
	 * offset 116
	*/
	pin_output_mode_e ignitionPinMode;
	/**
	 * offset 120
	*/
	brain_pin_e malfunctionIndicatorPin;
	/**
	 * offset 124
	*/
	pin_output_mode_e malfunctionIndicatorPinMode;
	/**
	 * offset 128
	*/
	brain_pin_e fanPin;
	/**
	 * offset 132
	*/
	pin_output_mode_e fanPinMode;
	/**
	 * offset 136
	*/
	brain_pin_e electronicThrottlePin1;
	/**
	 * offset 140
	*/
	pin_output_mode_e electronicThrottlePin1Mode;
	/**
	 * some cars have a switch to indicate that clutch pedal is all the way down
	 * offset 144
	*/
	brain_pin_e clutchDownPin;
	/**
	 * offset 148
	*/
	pin_input_mode_e clutchDownPinMode;
	/**
	 * offset 152
	*/
	brain_pin_e alternatorControlPin;
	/**
	 * offset 156
	*/
	pin_output_mode_e alternatorControlPinMode;
	/**
	 * offset 160
	*/
	brain_pin_e HD44780_rs;
	/**
	 * offset 164
	*/
	brain_pin_e HD44780_e;
	/**
	 * offset 168
	*/
	brain_pin_e HD44780_db4;
	/**
	 * offset 172
	*/
	brain_pin_e HD44780_db5;
	/**
	 * offset 176
	*/
	brain_pin_e HD44780_db6;
	/**
	 * offset 180
	*/
	brain_pin_e HD44780_db7;
	/**
	 * offset 184
	*/
	brain_pin_e gps_rx_pin;
	/**
	 * offset 188
	*/
	brain_pin_e gps_tx_pin;
	/**
	 * offset 192
	*/
	int idleSolenoidFrequency;
	/**
	 * offset 196
	*/
	int triggerSimulatorFrequency;
	/**
	 * Digital Potentiometer is used by stock ECU stimulation code
	 * offset 200
	*/
	spi_device_e digitalPotentiometerSpiDevice;
	/**
	 * offset 204
	*/
	brain_pin_e digitalPotentiometerChipSelect[DIGIPOT_COUNT];
	/**
	 * offset 220
	*/
	adc_channel_mode_e adcHwChannelEnabled[HW_MAX_ADC_INDEX];
	/**
	 * offset 284
	*/
	brain_pin_e triggerInputPins[3];
	/**
	 * offset 296
	*/
	brain_pin_e mainRelayPin;
	/**
	 * offset 300
	*/
	int idleThreadPeriod;
	/**
	 * offset 304
	*/
	int consoleLoopPeriod;
	/**
	 * offset 308
	*/
	int lcdThreadPeriod;
	/**
	 * offset 312
	*/
	int tunerStudioThreadPeriod;
	/**
	 * offset 316
	*/
	int generalPeriodicThreadPeriod;
	/**
	 * offset 320
	*/
	uint32_t tunerStudioSerialSpeed;
	/**
	 * offset 324
	*/
	brain_pin_e boardTestModeJumperPin;
	/**
	 * offset 328
	*/
	can_device_mode_e canDeviceMode;
	/**
	 * offset 332
	*/
	brain_pin_e canTxPin;
	/**
	 * offset 336
	*/
	brain_pin_e canRxPin;
	/**
	 * offset 340
	*/
	brain_pin_e triggerSimulatorPins[TRIGGER_SIMULATOR_PIN_COUNT];
	/**
	 * offset 352
	*/
	pin_output_mode_e triggerSimulatorPinModes[TRIGGER_SIMULATOR_PIN_COUNT];
	/**
	 * offset 364
	*/
	brain_pin_e o2heaterPin;
	/**
	 * offset 368
	*/
	pin_output_mode_e o2heaterPinModeTodO;

	/** offset 372 bit 0 */
	uint32_t is_enabled_spi_1 : 1;
	/** offset 372 bit 1 */
	uint32_t is_enabled_spi_2 : 1;
	/** offset 372 bit 2 */
	uint32_t is_enabled_spi_3 : 1;
	/** offset 372 bit 3 */
	uint32_t isSdCardEnabled : 1;
	/** offset 372 bit 4 */
	uint32_t isFastAdcEnabled : 1;
	/** offset 372 bit 5 */
	uint32_t isEngineControlEnabled : 1;
	/** offset 372 bit 6 */
	uint32_t isHip9011Enabled : 1;
	/**
	 * offset 376
	*/
	brain_pin_e logicAnalyzerPins[LOGIC_ANALYZER_CHANNEL_COUNT];
	/**
	 * default or inverted input
	 * offset 392
	*/
	uint8_t logicAnalyzerMode[LOGIC_ANALYZER_CHANNEL_COUNT];
	/**
	 * offset 396
	*/
	int unrealisticRpmThreashold;
	/**
	 * offset 400
	*/
	pin_output_mode_e mainRelayPinMode;
	/**
	 * offset 404
	*/
	egt_cs_array_t max31855_cs;
	/**
	 * offset 408
	*/
	spi_device_e max31855spiDevice;
	/**
	 * offset 412
	*/
	brain_pin_e fsioPins[LE_COMMAND_COUNT];
	/**
	 * offset 476
	*/
	pin_output_mode_e gpioPinModes[LE_COMMAND_COUNT];
	/**
	 * offset 540
	*/
	brain_pin_e joystickPins[JOYSTICK_PIN_COUNT];
	/**
	 * This pin is used for debugging - snap a logic analyzer on it and see if it's ever high
	 * offset 560
	*/
	brain_pin_e triggerErrorPin;
	/**
	 * offset 564
	*/
	pin_output_mode_e triggerErrorPinMode;
	/**
	 * value between 0 and 1
	 * offset 568
	*/
	float idleSolenoidPwm;
	/**
	 * offset 572
	*/
	brain_pin_e acRelayPin;
	/**
	 * offset 576
	*/
	pin_output_mode_e acRelayPinMode;
	/**
	 * offset 580
	*/
	int16_t fsioFrequency[LE_COMMAND_COUNT];
	/**
	 * offset 612
	*/
	brain_pin_e hip9011CsPin;
	/**
	 * offset 616
	*/
	brain_pin_e hip9011IntHoldPin;
	/**
	 * offset 620
	*/
	brain_pin_e hip9011OutPin;
	/**
	 * offset 624
	*/
	float fsio_setting[LE_COMMAND_COUNT];
	/**
	 * offset 688
	*/
	brain_pin_e spi1mosiPin;
	/**
	 * offset 692
	*/
	brain_pin_e spi1misoPin;
	/**
	 * offset 696
	*/
	brain_pin_e spi1sckPin;
	/**
	 * offset 700
	*/
	brain_pin_e spi2mosiPin;
	/**
	 * offset 704
	*/
	brain_pin_e spi2misoPin;
	/**
	 * offset 708
	*/
	brain_pin_e spi2sckPin;
	/**
	 * offset 712
	*/
	brain_pin_e spi3mosiPin;
	/**
	 * offset 716
	*/
	brain_pin_e spi3misoPin;
	/**
	 * offset 720
	*/
	brain_pin_e spi3sckPin;
	/**
	 * offset 724
	*/
	float hip9011Gain;
	/**
	 * offset 728
	*/
	brain_pin_e joystickCenterPin;
	/**
	 * offset 732
	*/
	brain_pin_e joystickAPin;
	/**
	 * offset 736
	*/
	brain_pin_e joystickBPin;
	/**
	 * offset 740
	*/
	brain_pin_e joystickCPin;
	/**
	 * offset 744
	*/
	brain_pin_e joystickDPin;
	/**
	 * offset 748
	*/
	analog_chart_e analogChartMode;
	/**
	 * offset 752
	*/
	int unusedbs[6];
	/**
	 * offset 776
	*/
	le_formula_t le_formulas[LE_COMMAND_COUNT];
	/** total size 3976*/
} board_configuration_s;
