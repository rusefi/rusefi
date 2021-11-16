#include "pch.h"

#include "dynoview.h"

void printResults(DynoView *dut) {
#ifdef DBG_TESTS    
    std::cerr.precision(32);
    std::cerr << "Acceleration m/s " << dut->getAcceleration() << std::endl; 
    std::cerr << "Car force in N " << dut->getEngineForce() << std::endl;
    std::cerr << "Car power in KW "<< dut->getEnginePower() << std::endl;
    std::cerr << "Car HP " << dut->getEngineHP() << std::endl;
    std::cerr << "Car Torque at wheel Nm " << dut->getEngineTorque() << std::endl;
#else
    (void)dut;
#endif
}


TEST(DynoView, VSS_T1) {
    WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

    DynoView dut;
    dut.inject();

    // Test Speed threshold
    engineConfiguration->vehicleWeight = 900; 
    eth.moveTimeForwardMs(50);
	
	Sensor::setMockValue(SensorType::VehicleSpeed, 18.0);
    dut.update(ICU);

    eth.moveTimeForwardAndInvokeEventsSec(20);
	Sensor::setMockValue(SensorType::VehicleSpeed, 126.0);
    dut.update(ICU);

    ASSERT_EQ(1.5, dut.getAcceleration()); 
}

TEST(DynoView, algo) {
    WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

    DynoView dut;
    dut.inject();

    // Test Speed threshold
    engineConfiguration->vehicleWeight = 900; 

    //to capture vss
	Sensor::setMockValue(SensorType::VehicleSpeed, 35*3.6);
    dut.update(ICU);

    dut.setAcceleration(1.5);
    dut.updateHP();

    ASSERT_EQ(1.5, dut.getAcceleration());
    ASSERT_EQ(1350, dut.getEngineForce());
    ASSERT_EQ(47, dut.getEnginePower());
    ASSERT_EQ(63, dut.getEngineHP());
    
    printResults(&dut);

}

TEST(DynoView, VSS_fast) {
    WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

    DynoView dut;
    dut.inject();

    // Test Speed threshold
    engineConfiguration->vehicleWeight = 900; //kg
    engine->rpmCalculator.mockRpm = 2200;
    eth.moveTimeForwardMs(50);
	
	Sensor::setMockValue(SensorType::VehicleSpeed, 50.0);
    dut.update(CAN);

    //delay 50ms
    eth.moveTimeForwardMs(50);
	Sensor::setMockValue(SensorType::VehicleSpeed, 50.252);
    dut.update(CAN);

    ASSERT_EQ(1259, dut.getEngineForce());
    printResults(&dut);
}


TEST(DynoView, VSS_Torque) {
    WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

    DynoView dut;
    dut.inject();

    // Test Speed threshold
    engineConfiguration->vehicleWeight = 900; //kg
    engine->rpmCalculator.mockRpm = 2200;
    eth.moveTimeForwardMs(50);
	
	Sensor::setMockValue(SensorType::VehicleSpeed, 80.0);
    dut.update(CAN);

    //delay 50ms
    eth.moveTimeForwardMs(50);
	Sensor::setMockValue(SensorType::VehicleSpeed, 80.504);
    dut.update(CAN);

    ASSERT_EQ(242, dut.getEngineTorque());
    printResults(&dut);
}

