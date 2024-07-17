/*
 * @author Matthew Kennedy, (c) 2019
 */

#include "pch.h"

#include "thermistor_func.h"
#include "thermistors.h"
#include "functional_sensor.h"
#include "init.h"

TEST(thermistor, Thermistor1) {
	ThermistorFunc tf;
	thermistor_conf_s tc = {32, 75, 120, 9500, 2100, 1000, 0};
	tf.configure(tc);

	SensorResult t = tf.convert(2100);
	ASSERT_TRUE(t.Valid);
	ASSERT_NEAR(75, t.Value, EPS2D);

	ASSERT_NEAR(-0.003, tf.m_a, EPS4D);
	ASSERT_NEAR(0.001, tf.m_b, EPS4D);
	ASSERT_NEAR(0.0, tf.m_c, EPS5D);
}

TEST(thermistor, ThermistorNeon) {
	ThermistorFunc tf;
	// 2003 Neon sensor
	thermistor_conf_s tc = {0, 30, 100, 32500, 7550, 700, 0};
	tf.configure(tc);

	SensorResult t = tf.convert(38000);
	ASSERT_TRUE(t.Valid);
	ASSERT_NEAR(-2.7983, t.Value, EPS4D);

	assertEqualsM("A", 0.0009, tf.m_a);
	assertEqualsM("B", 0.0003, tf.m_b);
	ASSERT_NEAR(0.0, tf.m_c, EPS4D);
}

TEST(thermistor, PtcAirCooledMotorcycle) {
	// data from https://static.chipdip.ru/lib/033/DOC001033132.pdf
	thermistor_conf_s tc = {0, 100, 200, 486, 975, 1679, 0};

	ThermistorFunc tf;
	tf.configure(tc);

	// calibrated points should be almost perfect
	ASSERT_NEAR(tf.convert(486).value_or(0), 0, 0.1);
	ASSERT_NEAR(tf.convert(975).value_or(0), 100, 0.1);
	ASSERT_NEAR(tf.convert(1679).value_or(0), 200, 0.1);

	// Other points should be pretty good
	ASSERT_NEAR(tf.convert(414).value_or(0), -20, 2);
	ASSERT_NEAR(tf.convert(704).value_or(0), 50, 2);
	ASSERT_NEAR(tf.convert(1300).value_or(0), 150, 2);
	ASSERT_NEAR(tf.convert(1846).value_or(0), 220, 2);
}

TEST(Thermistor, Option1) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE, [](engine_configuration_s* engineConfiguration) {
                                                 			engineConfiguration->auxTempSensor1.adcChannel = EFI_ADC_12;; // arbitrary
                                                 	});


    setAtSensor(&engineConfiguration->auxTempSensor1, /*temp low*/-13.9, 73300, /*temp mid*/23.7, 10630 , /*temp high*/ 60, 2280);
	initNewSensors();

	FunctionalSensor * aatSensor = (FunctionalSensor*)Sensor::getSensorOfType(SensorType::AuxTemp1);
    ASSERT_TRUE(aatSensor != nullptr);
    thermistor_t *tFuncAat = (thermistor_t *)aatSensor->getFunction();

    ThermistorFunc *thermistorFuncAat = tFuncAat->getPtr<ThermistorFunc>();

    ASSERT_NEAR(60, thermistorFuncAat->convert(2280).Value, EPS2D);
    ASSERT_NEAR(49.83, thermistorFuncAat->convert(3413).Value, EPS2D);
}

TEST(Thermistor, Option2) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE, [](engine_configuration_s* engineConfiguration) {
                                                 			engineConfiguration->auxTempSensor1.adcChannel = EFI_ADC_12;; // arbitrary
                                                 	});


    setAtSensor(&engineConfiguration->auxTempSensor1, /*temp low*/-13.9, 73300, /*temp mid*/23.5, 53100 , /*temp high*/ 60, 2280);

  EXPECT_FATAL_ERROR(
	  initNewSensors()
	);

	FunctionalSensor * aatSensor = (FunctionalSensor*)Sensor::getSensorOfType(SensorType::AuxTemp1);
    ASSERT_TRUE(aatSensor == nullptr);
}


TEST(Thermistor, Option3) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE, [](engine_configuration_s* engineConfiguration) {
                                                 			engineConfiguration->auxTempSensor1.adcChannel = EFI_ADC_12;; // arbitrary
                                                 	});


    setAtSensor(&engineConfiguration->auxTempSensor1, /*temp low*/-13.9, 73300, /*temp mid*/45, 3810, /*temp high*/ 90, 952);

	  initNewSensors();

	FunctionalSensor * aatSensor = (FunctionalSensor*)Sensor::getSensorOfType(SensorType::AuxTemp1);
    ASSERT_TRUE(aatSensor != nullptr);
    thermistor_t *tFuncAat = (thermistor_t *)aatSensor->getFunction();

    ThermistorFunc *thermistorFuncAat = tFuncAat->getPtr<ThermistorFunc>();

    ASSERT_NEAR(59.9064, thermistorFuncAat->convert(2280).Value, EPS2D);
    ASSERT_NEAR(48.043, thermistorFuncAat->convert(3413).Value, EPS2D);
    ASSERT_NEAR(-9.17, thermistorFuncAat->convert(53100).Value, EPS2D);
}

