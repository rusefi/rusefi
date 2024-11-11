#include "pch.h"

#include "can_msg_tx.h"
#include "can.h"
#include "can_listener.h"
#include "obd2.h"


TEST(CanObd2, obdSendPacket)
{
    EngineTestHelper eth(engine_type_e::TEST_ENGINE);

    obdSendPacket(0, PID_RPM, 1, 0xff, 0);
    EXPECT_TRUE(txCanBuffer.getCount());

    CANTxFrame frame = txCanBuffer.get();

    EXPECT_EQ(frame.data8[0], 3);		// correct data size
    EXPECT_EQ(frame.data8[1], 0X40);	// correct header
    EXPECT_EQ(frame.data8[2], PID_RPM); // correct PID

    // clear shared buffer
    txCanBuffer.clear();
    EXPECT_FALSE(txCanBuffer.getCount());
}

TEST(CanObd2, handleGetDataRequest_SUPPORTED_PIDS_01_20)
{
    EngineTestHelper eth(engine_type_e::TEST_ENGINE);

    CANRxFrame frame;
    frame.data8[2] = PID_SUPPORTED_PIDS_REQUEST_01_20;

    handleGetDataRequest(frame, 0);
    CANTxFrame rxFrame = txCanBuffer.get();

    EXPECT_EQ(rxFrame.data8[0], 6);		// correct data size
    EXPECT_EQ(rxFrame.data8[1], 0X41);	// correct header
    EXPECT_EQ(rxFrame.data8[2], PID_SUPPORTED_PIDS_REQUEST_01_20); // correct PID

    // clear shared buffer
    txCanBuffer.clear();
    EXPECT_FALSE(txCanBuffer.getCount());
}

TEST(CanObd2, handleGetDataRequest_SUPPORTED_PIDS_21_40)
{
    EngineTestHelper eth(engine_type_e::TEST_ENGINE);

    CANRxFrame frame;
    frame.data8[2] = PID_SUPPORTED_PIDS_REQUEST_21_40;

    handleGetDataRequest(frame, 0);
    CANTxFrame rxFrame = txCanBuffer.get();

    EXPECT_EQ(rxFrame.data8[0], 6);		// correct data size
    EXPECT_EQ(rxFrame.data8[1], 0x41);	// correct header
    EXPECT_EQ(rxFrame.data8[2], PID_SUPPORTED_PIDS_REQUEST_21_40); // correct PID

    // clear shared buffer
    txCanBuffer.clear();
    EXPECT_FALSE(txCanBuffer.getCount());
}

TEST(CanObd2, handleGetDataRequest_SUPPORTED_PIDS_41_60)
{
    EngineTestHelper eth(engine_type_e::TEST_ENGINE);

    CANRxFrame frame;
    frame.data8[2] = PID_SUPPORTED_PIDS_REQUEST_41_60;

    handleGetDataRequest(frame, 0);
    CANTxFrame rxFrame = txCanBuffer.get();

    EXPECT_EQ(rxFrame.data8[0], 6);		// correct data size
    EXPECT_EQ(rxFrame.data8[1], 0x41);	// correct header
    EXPECT_EQ(rxFrame.data8[2], PID_SUPPORTED_PIDS_REQUEST_41_60); // correct PID

    // clear shared buffer
    txCanBuffer.clear();
    EXPECT_FALSE(txCanBuffer.getCount());
}


TEST(CanObd2, handleGetDataRequest_SUPPORTED_PID_ENGINE_LOAD)
{
    EngineTestHelper eth(engine_type_e::TEST_ENGINE);

    CANRxFrame frame;
    frame.data8[2] = PID_ENGINE_LOAD;

    handleGetDataRequest(frame, 0);
    CANTxFrame rxFrame = txCanBuffer.get();

    EXPECT_EQ(rxFrame.data8[0], 3);		// correct data size
    EXPECT_EQ(rxFrame.data8[1], 0x41);	// correct header
    EXPECT_EQ(rxFrame.data8[2], PID_ENGINE_LOAD); // correct PID
    EXPECT_EQ(rxFrame.data8[3], getFuelingLoad() * ODB_TPS_BYTE_PERCENT); // correct value

    // clear shared buffer
    txCanBuffer.clear();
    EXPECT_FALSE(txCanBuffer.getCount());
}


TEST(CanObd2, handleGetDataRequest_PID_COOLANT_TEMP)
{
    EngineTestHelper eth(engine_type_e::TEST_ENGINE);

    CANRxFrame frame;
    frame.data8[2] = PID_COOLANT_TEMP;

    handleGetDataRequest(frame, 0);
    CANTxFrame rxFrame = txCanBuffer.get();

    EXPECT_EQ(rxFrame.data8[0], 3);		// correct data size
    EXPECT_EQ(rxFrame.data8[1], 0x41);	// correct header
    EXPECT_EQ(rxFrame.data8[2], PID_COOLANT_TEMP); // correct PID
    EXPECT_EQ(rxFrame.data8[3], Sensor::getOrZero(SensorType::Clt) + ODB_TEMP_EXTRA); // correct value

    // clear shared buffer
    txCanBuffer.clear();
    EXPECT_FALSE(txCanBuffer.getCount());
}

TEST(CanObd2, handleGetDataRequest_PID_STFT_BANK1_2)
{
    EngineTestHelper eth(engine_type_e::TEST_ENGINE);

    CANRxFrame frame;
    frame.data8[2] = PID_STFT_BANK1;

    handleGetDataRequest(frame, 0);
    CANTxFrame rxFrame = txCanBuffer.get();

    EXPECT_EQ(rxFrame.data8[0], 3);		// correct data size
    EXPECT_EQ(rxFrame.data8[1], 0x41);	// correct header
    EXPECT_EQ(rxFrame.data8[2], PID_STFT_BANK1); // correct PID
    EXPECT_EQ(rxFrame.data8[3], 128 * engine->engineState.stftCorrection[0]); // correct value

    frame.data8[2] = PID_STFT_BANK2;

    handleGetDataRequest(frame, 0);
    rxFrame = txCanBuffer.get();

    EXPECT_EQ(rxFrame.data8[0], 3);		// correct data size
    EXPECT_EQ(rxFrame.data8[1], 0x41);	// correct header
    EXPECT_EQ(rxFrame.data8[2], PID_STFT_BANK2); // correct PID
    EXPECT_EQ(rxFrame.data8[3], 128 * engine->engineState.stftCorrection[0]); // correct value

    // clear shared buffer
    txCanBuffer.clear();
    EXPECT_FALSE(txCanBuffer.getCount());
}

TEST(CanObd2, handleGetDataRequest_PID_INTAKE_MAP)
{
    EngineTestHelper eth(engine_type_e::TEST_ENGINE);

    CANRxFrame frame;
    frame.data8[2] = PID_INTAKE_MAP;

    handleGetDataRequest(frame, 0);
    CANTxFrame rxFrame = txCanBuffer.get();

    EXPECT_EQ(rxFrame.data8[0], 3);		// correct data size
    EXPECT_EQ(rxFrame.data8[1], 0x41);	// correct header
    EXPECT_EQ(rxFrame.data8[2], PID_INTAKE_MAP); // correct PID
    EXPECT_EQ(rxFrame.data8[3], Sensor::getOrZero(SensorType::Map)); // correct value

    // clear shared buffer
    txCanBuffer.clear();
    EXPECT_FALSE(txCanBuffer.getCount());
}

TEST(CanObd2, handleGetDataRequest_PID_RPM)
{
    EngineTestHelper eth(engine_type_e::TEST_ENGINE);

    CANRxFrame frame;
    frame.data8[2] = PID_RPM;

    handleGetDataRequest(frame, 0);
    CANTxFrame rxFrame = txCanBuffer.get();

    EXPECT_EQ(rxFrame.data8[0], 4);		// correct data size
    EXPECT_EQ(rxFrame.data8[1], 0x41);	// correct header
    EXPECT_EQ(rxFrame.data8[2], PID_RPM); // correct PID

    int rpm = Sensor::getOrZero(SensorType::Rpm) * ODB_RPM_MULT;

    EXPECT_EQ(rxFrame.data8[3], (rpm >> 0) & 0xff); // correct value
    EXPECT_EQ(rxFrame.data8[4], (rpm >> 8) & 0xff); // correct value

    // clear shared buffer
    txCanBuffer.clear();
    EXPECT_FALSE(txCanBuffer.getCount());
}

TEST(CanObd2, handleGetDataRequest_PID_SPEED)
{
    EngineTestHelper eth(engine_type_e::TEST_ENGINE);

    CANRxFrame frame;
    frame.data8[2] = PID_SPEED;

    handleGetDataRequest(frame, 0);
    CANTxFrame rxFrame = txCanBuffer.get();

    EXPECT_EQ(rxFrame.data8[0], 3);		// correct data size
    EXPECT_EQ(rxFrame.data8[1], 0x41);	// correct header
    EXPECT_EQ(rxFrame.data8[2], PID_SPEED); // correct PID
    EXPECT_EQ(rxFrame.data8[3], Sensor::getOrZero(SensorType::VehicleSpeed)); // correct value

    // clear shared buffer
    txCanBuffer.clear();
    EXPECT_FALSE(txCanBuffer.getCount());
}


TEST(CanObd2, handleGetDataRequest_PID_INTAKE_TEMP)
{
    EngineTestHelper eth(engine_type_e::TEST_ENGINE);

    CANRxFrame frame;
    frame.data8[2] = PID_INTAKE_TEMP;

    handleGetDataRequest(frame, 0);
    CANTxFrame rxFrame = txCanBuffer.get();

    EXPECT_EQ(rxFrame.data8[0], 3);		// correct data size
    EXPECT_EQ(rxFrame.data8[1], 0x41);	// correct header
    EXPECT_EQ(rxFrame.data8[2], PID_INTAKE_TEMP); // correct PID
    EXPECT_EQ(rxFrame.data8[3], Sensor::getOrZero(SensorType::Iat) + ODB_TEMP_EXTRA); // correct value

    // clear shared buffer
    txCanBuffer.clear();
    EXPECT_FALSE(txCanBuffer.getCount());
}

TEST(CanObd2, handleGetDataRequest_PID_INTAKE_MAF)
{
    EngineTestHelper eth(engine_type_e::TEST_ENGINE);

    CANRxFrame frame;
    frame.data8[2] = PID_INTAKE_MAF;

    handleGetDataRequest(frame, 0);
    CANTxFrame rxFrame = txCanBuffer.get();

    EXPECT_EQ(rxFrame.data8[0], 4);		// correct data size
    EXPECT_EQ(rxFrame.data8[1], 0x41);	// correct header
    EXPECT_EQ(rxFrame.data8[2], PID_INTAKE_MAF); // correct PID

    int maf = Sensor::getOrZero(SensorType::Maf) * 100.0f;

    EXPECT_EQ(rxFrame.data8[3], (maf >> 0) & 0xff); // correct value
    EXPECT_EQ(rxFrame.data8[4], (maf >> 8) & 0xff); // correct value

    // clear shared buffer
    txCanBuffer.clear();
    EXPECT_FALSE(txCanBuffer.getCount());
}

TEST(CanObd2, handleGetDataRequest_PID_THROTTLE)
{
    EngineTestHelper eth(engine_type_e::TEST_ENGINE);

    CANRxFrame frame;
    frame.data8[2] = PID_THROTTLE;

    handleGetDataRequest(frame, 0);
    CANTxFrame rxFrame = txCanBuffer.get();

    EXPECT_EQ(rxFrame.data8[0], 3);		// correct data size
    EXPECT_EQ(rxFrame.data8[1], 0x41);	// correct header
    EXPECT_EQ(rxFrame.data8[2], PID_THROTTLE); // correct PID
    EXPECT_EQ(rxFrame.data8[3], Sensor::getOrZero(SensorType::Tps1) * ODB_TPS_BYTE_PERCENT); // correct value

    // clear shared buffer
    txCanBuffer.clear();
    EXPECT_FALSE(txCanBuffer.getCount());
}


TEST(CanObd2, handleGetDataRequest_PID_FUEL_AIR_RATIO_1)
{
    EngineTestHelper eth(engine_type_e::TEST_ENGINE);

    CANRxFrame frame;
    frame.data8[2] = PID_FUEL_AIR_RATIO_1;

    handleGetDataRequest(frame, 0);
    CANTxFrame rxFrame = txCanBuffer.get();

    EXPECT_EQ(rxFrame.data8[0], 6);		// correct data size
    EXPECT_EQ(rxFrame.data8[1], 0x41);	// correct header
    EXPECT_EQ(rxFrame.data8[2], PID_FUEL_AIR_RATIO_1); // correct PID

    float lambda = clampF(0, Sensor::getOrZero(SensorType::Lambda1), 1.99f);
    uint16_t scaled = (lambda * 32768);

    EXPECT_EQ(rxFrame.data8[3], ((scaled << 16) >> 0) & 0xff); // correct value
    EXPECT_EQ(rxFrame.data8[4], ((scaled << 16) >> 8) & 0xff); // correct value


    // clear shared buffer
    txCanBuffer.clear();
    EXPECT_FALSE(txCanBuffer.getCount());
}
