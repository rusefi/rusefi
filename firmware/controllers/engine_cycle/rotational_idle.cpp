bool shouldEngageRotationalIdle() {
	if (config->rot_idle_auto_engage_clt_enable && Sensor::getOrZero(SensorType::Clt) > config->rot_idle_auto_engage_clt) {
		return true;
	}

	if (config->rot_idle_auto_engage && Sensor::getOrZero(SensorType::DriverThrottleIntent) <= config->rot_idle_max_tps) {
		return true; // tps is below the maximum
	}

	return false;
}
