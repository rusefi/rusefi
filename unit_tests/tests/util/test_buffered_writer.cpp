#include "pch.h"

#include "buffered_writer.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

template <int TBufferSize>
struct MockBufferedWriter : public BufferedWriter<TBufferSize>
{
	MOCK_METHOD(size_t, writeInternal, (const char*, size_t), (override));
};

static const char* testBuffer = "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

TEST(BufferedWriter, WriteSmall) {
	// No calls to dut expected
	StrictMock<MockBufferedWriter<10>> dut;

	EXPECT_EQ(0, dut.write(testBuffer, 5));
}

TEST(BufferedWriter, WriteSmallFlush) {
	StrictMock<MockBufferedWriter<10>> dut;
	EXPECT_CALL(dut, writeInternal(_, 5)).WillOnce(Return(5));

	ASSERT_EQ(0, dut.write(testBuffer, 5));

	EXPECT_EQ(dut.flush(), 5);
}

TEST(BufferedWriter, WriteMultipleSmall) {
	StrictMock<MockBufferedWriter<10>> dut;
	{
		EXPECT_CALL(dut, writeInternal(_, 10)).WillOnce(Return(10));
		EXPECT_CALL(dut, writeInternal(_, 2)).WillOnce(Return(2));
	}

	EXPECT_EQ(0, dut.write(testBuffer, 3));
	EXPECT_EQ(0, dut.write(testBuffer, 3));
	EXPECT_EQ(0, dut.write(testBuffer, 3));
	EXPECT_EQ(10, dut.write(testBuffer, 3));	// <- this one should trigger a flush

	// Flush the remainder
	EXPECT_EQ(dut.flush(), 2);
}

TEST(BufferedWriter, WriteSingleFullBufferSize) {
	StrictMock<MockBufferedWriter<50>> dut;

	EXPECT_CALL(dut, writeInternal(_, 50)).WillOnce(Return(50));

	EXPECT_EQ(50, dut.write(testBuffer, 50));

	// Nothing left to flush!
	EXPECT_EQ(0, dut.flush());
}

TEST(BufferedWriter, WriteLarge) {
	StrictMock<MockBufferedWriter<10>> dut;

	{
		EXPECT_CALL(dut, writeInternal(_, 45)).WillOnce(Return(45));
	}

	EXPECT_EQ(45, dut.write(testBuffer, 45));

	EXPECT_EQ(0, dut.flush());
}

TEST(BufferedWriter, WriteLargeAfterSmall) {
	StrictMock<MockBufferedWriter<10>> dut;

	{
		EXPECT_CALL(dut, writeInternal(_, 10)).WillOnce(Return(10));
		EXPECT_CALL(dut, writeInternal(_, 36)).WillOnce(Return(36));
	}

	EXPECT_EQ(0, dut.write(testBuffer, 1));

	EXPECT_EQ(46, dut.write(testBuffer, 45));

	EXPECT_EQ(0, dut.flush());
}
