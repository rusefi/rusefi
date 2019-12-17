/*
 * @author Matthew Kennedy, (c) 2019
 */

#include "unit_test_framework.h"
#include "thermistor_func.h"
#include "thermistors.h"

TEST(thermistor, Thermistor1) {
	ThermistorFunc tf;
	thermistor_conf_s tc = {32, 75, 120, 9500, 2100, 1000, 0};
	tf.configure(tc);

	SensorResult t = tf.convert(2100);
	ASSERT_TRUE(t.Valid);
	ASSERT_FLOAT_EQ(75, t.Value);

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
