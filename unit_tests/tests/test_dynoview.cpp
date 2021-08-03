#include "engine_test_helper.h"
#include "engine_controller.h"
#include "dynoview.h"
#include "vehicle_speed.h"

#include <gtest/gtest.h>


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
    INJECT_ENGINE_REFERENCE(&dut);

    // Test Speed trashold
    engineConfiguration->vehicleWeight = 900; 
    eth.moveTimeForwardMs(50);
	
    setMockVehicleSpeed(18.0 PASS_ENGINE_PARAMETER_SUFFIX);
    dut.update(ICU);

    eth.moveTimeForwardAndInvokeEventsSec(20);
    setMockVehicleSpeed(126.0 PASS_ENGINE_PARAMETER_SUFFIX);
    dut.update(ICU);

    ASSERT_EQ(1.5, dut.getAcceleration()); 
}

TEST(DynoView, algo) {
    WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

    DynoView dut;
    INJECT_ENGINE_REFERENCE(&dut);

    // Test Speed trashold
    engineConfiguration->vehicleWeight = 900; 

    //to capture vss
    setMockVehicleSpeed(35*3.6 PASS_ENGINE_PARAMETER_SUFFIX);
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
    INJECT_ENGINE_REFERENCE(&dut);

    // Test Speed trashold
    engineConfiguration->vehicleWeight = 900; //kg
    engine->rpmCalculator.mockRpm = 2200;
    eth.moveTimeForwardMs(50);
	
    setMockVehicleSpeed(50.0 PASS_ENGINE_PARAMETER_SUFFIX);
    dut.update(CAN);

    //delay 50ms
    eth.moveTimeForwardMs(50);
    setMockVehicleSpeed(50.252 PASS_ENGINE_PARAMETER_SUFFIX);
    dut.update(CAN);

    ASSERT_EQ(1259, dut.getEngineForce());
    printResults(&dut);
}


TEST(DynoView, VSS_Torque) {
    WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

    DynoView dut;
    INJECT_ENGINE_REFERENCE(&dut);

    // Test Speed trashold
    engineConfiguration->vehicleWeight = 900; //kg
    engine->rpmCalculator.mockRpm = 2200;
    eth.moveTimeForwardMs(50);
	
    setMockVehicleSpeed(80.0 PASS_ENGINE_PARAMETER_SUFFIX);
    dut.update(CAN);

    //delay 50ms
    eth.moveTimeForwardMs(50);
    setMockVehicleSpeed(80.504 PASS_ENGINE_PARAMETER_SUFFIX);
    dut.update(CAN);

    ASSERT_EQ(242, dut.getEngineTorque());
    printResults(&dut);
}

