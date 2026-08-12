/*
 * @file test_can_bmw_e46.cpp
 *
 * @date: dic 30, 2025
 * @author FDSoftware
 */


#include "pch.h"
#include "can_msg_tx.h"
#include "can.h"
#include "can_dash.h"
#include "can_bmw.h"

TEST(CanDash, BMW_e46) {
    EngineTestHelper eth(engine_type_e::TEST_ENGINE);
    engineConfiguration->canNbcType = CAN_BUS_BMW_E46;

    engine->rpmCalculator.setRpmValue(1500);
    Sensor::setMockValue(SensorType::Clt, 70);

    CanCycle cycle(0);

    updateDash(cycle);

    EXPECT_EQ(txCanBuffer.getCount(), 2);

    // First frame: CAN_BMW_E46_RPM (0x316)
    CANTxFrame frame1 = txCanBuffer.get();
    EXPECT_EQ(frame1.SID, CAN_BMW_E46_RPM);
    EXPECT_EQ(frame1.data8[0], 0x05); // ASC message
    EXPECT_EQ(frame1.data8[1], 0x0C); // Indexed Engine Torque
    // data8[2-3]: RPM 1500 * 6.4 = 9600 = 0x25 | 0x80
    EXPECT_EQ(frame1.data8[2], 0x80);
    EXPECT_EQ(frame1.data8[3], 0x25);
    EXPECT_EQ(frame1.data8[4], 0x0C);
    EXPECT_EQ(frame1.data8[5], 0x15);
    EXPECT_EQ(frame1.data8[6], 0x00);
    EXPECT_EQ(frame1.data8[7], 0x35);

    // Second frame: CAN_BMW_E46_DME2 (0x329)
    CANTxFrame frame2 = txCanBuffer.get();
    EXPECT_EQ(frame2.SID, CAN_BMW_E46_DME2);
    EXPECT_EQ(frame2.data8[0], 0x11);
    // data8[1]: CLT converted (70 + 48.373) / 0.75; = 157.83 ≈ 0x9D
    EXPECT_EQ(frame2.data8[1], 0x9D);
    EXPECT_EQ(frame2.data8[2], 0x00); // baro sensor
    EXPECT_EQ(frame2.data8[3], 0x08);
    EXPECT_EQ(frame2.data8[4], 0x00); // TPS_VIRT_CRU_CAN
    EXPECT_EQ(frame2.data8[5], 0x00); // not used, TPS
    EXPECT_EQ(frame2.data8[6], 0x00); // brake system status
    EXPECT_EQ(frame2.data8[7], 0x00); // not used
}
