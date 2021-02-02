package com.rusefi;

public class ProteusHardwareTest extends RusefiTestBase {
	@Test
	public void testVbatt() {
		float vbatt = SensorCentral.getInstance().getValue(Sensor.VBATT);

		// allow some tolerance for the supply voltage...
		assertGreater(vbatt, 11.5);
		assertLess(vbatt, 12.5);
	}
}
