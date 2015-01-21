typedef struct {
	/**
	 * offset 0
	 */
	float baseCrankingFuel;
	/**
	 * This value controls what RPM values we consider 'cranking' (any RPM below 'crankingRpm')
	 * Anything above 'crankingRpm' would be 'running'
	 * offset 4
	 */
	int16_t crankingRpm;
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
