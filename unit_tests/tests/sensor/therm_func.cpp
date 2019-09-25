
/*
TEST(sensors, thermistors) {

	ThermistorMath tm;
	{
		setThermistorConfiguration(&tc, 32, 9500, 75, 2100, 120, 1000);
		tm.setConfig(&tc.config);
		float t = tm.getKelvinTemperatureByResistance(2100);
		ASSERT_FLOAT_EQ(75 + KELV, t);

		ASSERT_NEAR(-0.003, tm.s_h_a, EPS4D);
		ASSERT_NEAR(0.001, tm.s_h_b, EPS4D);
		ASSERT_NEAR(0.0, tm.s_h_c, EPS5D);

	}

	{
		// 2003 Neon sensor
		setThermistorConfiguration(&tc, 0, 32500, 30, 7550, 100, 700);
		tm.setConfig(&tc.config);

		float t = tm.getKelvinTemperatureByResistance(38000);
		ASSERT_NEAR(-2.7983, t - KELV, EPS4D);

		assertEqualsM("A", 0.0009, tm.s_h_a);
		assertEqualsM("B", 0.0003, tm.s_h_b);
		ASSERT_NEAR(0.0, tm.s_h_c, EPS4D);
	}
}*/
