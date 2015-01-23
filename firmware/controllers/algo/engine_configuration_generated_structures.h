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

/**
 * @brief Thermistor curve parameters

 */
typedef struct {
	/**
	 * these values are in Celcuus
	 * offset 0
	 */
	float tempC_1;
	/**
	 * offset 4
	 */
	float tempC_2;
	/**
	 * offset 8
	 */
	float tempC_3;
	/**
	 * offset 12
	 */
	float resistance_1;
	/**
	 * offset 16
	 */
	float resistance_2;
	/**
	 * offset 20
	 */
	float resistance_3;
	/**
	 * offset 24
	 */
	float bias_resistor;
	/**
	 * offset 28
	 */
	float s_h_a;
	/**
	 * offset 32
	 */
	float s_h_b;
	/**
	 * offset 36
	 */
	float s_h_c;
	/** total size 40*/
} ThermistorConf;


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
