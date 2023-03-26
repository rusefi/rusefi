

void setSbc() {
	engineConfiguration->specs.cylindersCount = 8;
	engineConfiguration->specs.firingOrder = FO_1_8_4_3_6_5_7_2;
	engineConfiguration->specs.displacement = 5.2;

	engineConfiguration->useStepperIdle = true;
	// 8 cylinders one coil HEI trigger
}

void set8chanSbc() {
    setSbc();
}

void setProteusSbc() {
    setSbc();
}