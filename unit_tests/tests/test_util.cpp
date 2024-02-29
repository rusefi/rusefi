/**
 * @file	test_util.c
 *
 * @date Dec 8, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include <string.h>

#include "cyclic_buffer.h"

#include "malfunction_central.h"
#include "cli_registry.h"

#include "mmc_card.h"
#include "fl_stack.h"

#include "big_buffer.h"

TEST(util, testitoa) {
	char buffer[12];
	itoa10(buffer, 239);
	ASSERT_TRUE(strEqual(buffer, "239"));
}

TEST(util, negativeZero) {
	ASSERT_TRUE(IS_NEGATIVE_ZERO(-0.0));

	ASSERT_FALSE(IS_NEGATIVE_ZERO(-10.0));
	ASSERT_FALSE(IS_NEGATIVE_ZERO(10.0));
	ASSERT_FALSE(IS_NEGATIVE_ZERO(0.0));
}

TEST(util, crc8) {
	const uint8_t crc8_tab[] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38};

	ASSERT_EQ(0xB, crc8(crc8_tab, 8));
}

TEST(util, crc) {
	ASSERT_EQ(4, efiRound(4.4, 1));
	ASSERT_FLOAT_EQ(1.2, efiRound(1.2345, 0.1));
	ASSERT_FLOAT_EQ(0.2, efiRound(0.2345, 0.1));

	const char * A = "A";

	uint32_t c = crc32(A, 1);
	printf("crc32(A)=%x\r\n", c);
	assertEqualsM("crc32 1", 0xd3d99e8b, c);

	const char * line = "AbcDEFGF";
	c = crc32(line, 8);
	printf("crc32(line)=%x\r\n", c);
	assertEqualsM("crc32 line", 0x4775a7b1, c);

	c = crc32(line, 1);
	c = crc32inc(line + 1, c, 8 - 1);
	assertEqualsM("crc32 line inc", 0x4775a7b1, c);
}

TEST(util, cyclicBufferContains) {
	cyclic_buffer<int> sb;
	sb.add(10);
	ASSERT_EQ(TRUE, sb.contains(10));
	ASSERT_EQ(FALSE, sb.contains(11));
}

TEST(util, cyclicBuffer) {
	cyclic_buffer<int> sb;

	{
		sb.add(10);

		ASSERT_EQ(10, sb.sum(3));

		sb.add(2);
		ASSERT_EQ(12, sb.sum(2));
	}
	{
		sb.clear();

		sb.add(1);
		sb.add(2);
		sb.add(3);
		sb.add(4);

		ASSERT_EQ(4, sb.maxValue(3));
		ASSERT_EQ(4, sb.maxValue(113));
		ASSERT_EQ( 2,  sb.minValue(3)) << "minValue(3)";
		ASSERT_EQ(1, sb.minValue(113));
	}

}

static void testMalfunctionCentralRemoveNonExistent() {
	clearWarnings();

	// this should not crash
	removeError(ObdCode::OBD_TPS1_Correlation);
}

static void testMalfunctionCentralSameElementAgain() {
	clearWarnings();
	error_codes_set_s localCopy;

	addError(ObdCode::OBD_TPS1_Correlation);
	addError(ObdCode::OBD_TPS1_Correlation);
	getErrorCodes(&localCopy);
	ASSERT_EQ(1, localCopy.count);
}

static void testMalfunctionCentralRemoveFirstElement() {
	clearWarnings();
	error_codes_set_s localCopy;

	ObdCode firstElement = ObdCode::OBD_TPS1_Correlation;
	addError(firstElement);

	ObdCode secondElement = ObdCode::OBD_TPS2_Correlation;
	addError(secondElement);
	getErrorCodes(&localCopy);
	ASSERT_EQ(2, localCopy.count);

	// let's remove first element - code
	removeError(firstElement);

	getErrorCodes(&localCopy);
	ASSERT_EQ(1, localCopy.count);
	ASSERT_EQ(secondElement, localCopy.error_codes[0]);
}

TEST(misc, testMalfunctionCentral) {
	testMalfunctionCentralRemoveNonExistent();
	testMalfunctionCentralSameElementAgain();
	testMalfunctionCentralRemoveFirstElement();

	clearWarnings();

	error_codes_set_s localCopy;

	// on start-up error storage should be empty
	getErrorCodes(&localCopy);
	ASSERT_EQ(0, localCopy.count);

	ObdCode code = ObdCode::OBD_TPS1_Correlation;
	// let's add one error and validate
	addError(code);

	getErrorCodes(&localCopy);
	ASSERT_EQ( 1,  localCopy.count) << "count #1";
	ASSERT_EQ(code, localCopy.error_codes[0]);

	// let's remove value which is not in the collection
	removeError((ObdCode) 22);
	// element not present - nothing to removed
	ASSERT_EQ(1, localCopy.count);
	ASSERT_EQ(code, localCopy.error_codes[0]);

	code = ObdCode::OBD_TPS2_Correlation;
	addError(code);
	getErrorCodes(&localCopy);
	// todo:	ASSERT_EQ(2, localCopy.count);

	for (int c = 0; c < 100; c++) {
		addError((ObdCode) c);
	}
	getErrorCodes(&localCopy);
	ASSERT_EQ(MAX_ERROR_CODES_COUNT, localCopy.count);

	// now we have full array and code below present
	removeError(code);
	getErrorCodes(&localCopy);
	ASSERT_EQ(MAX_ERROR_CODES_COUNT - 1, localCopy.count);
}

static int lastInteger = -1;
static int lastInteger2 = -1;

static void testEchoI(int param) {
	lastInteger = param;
}

static void testEchoII(int param, int param2) {
	lastInteger = param;
	lastInteger2 = param2;
}

static const char *lastFirst = NULL;
static const char *lastThird = NULL;

static void testEchoSSS(const char *first, const char *second, const char *third) {
	lastFirst = first;
	lastThird = third;
}

static float fFirst;
static float fSecond;
static float fThird;

static void testEchoFFF(float first, float second, float third) {
	fFirst = first;
	fSecond = second;
	fThird = third;
}

#define UNKNOWN_COMMAND "dfadasdasd"

// this buffer is needed because on Unix you would not be able to change static char constants
static char buffer[300];

TEST(misc, testConsoleLogic) {
	resetConsoleActions();

	helpCommand();

	char * cmd = "he ha";
	ASSERT_EQ(2, findEndOfToken(cmd));

	cmd = "\"hee\" ha";
	ASSERT_EQ(5, findEndOfToken(cmd));

	cmd = "\"h e\" ha";
	ASSERT_EQ(5, findEndOfToken(cmd));

	strcpy(buffer, "echo");
	ASSERT_TRUE(strEqual("echo", unquote(buffer)));

	strcpy(buffer, "\"echo\"");
	ASSERT_TRUE(strEqual("echo", unquote(buffer))) << "unquote quoted";

	ASSERT_EQ(10, tokenLength(UNKNOWN_COMMAND));

	// handling invalid token should work
	strcpy(buffer, "sdasdafasd asd");
	handleConsoleLine(buffer);

	printf("\r\naddConsoleActionI\r\n");
	addConsoleActionI("echoi", testEchoI);
	strcpy(buffer, "echoi 239");
	handleConsoleLine(buffer);
	ASSERT_EQ(239, lastInteger);

	printf("\r\naddConsoleActionI 240 with two spaces\r\n");
	strcpy(buffer, "echoi  240");
	handleConsoleLine(buffer);
	ASSERT_EQ(240, lastInteger);


	printf("\r\naddConsoleActionII\r\n");
	addConsoleActionII("echoii", testEchoII);
	strcpy(buffer, "echoii 22 239");
	handleConsoleLine(buffer);
	ASSERT_EQ(22, lastInteger);
	ASSERT_EQ(239, lastInteger2);

	printf("\r\naddConsoleActionII three spaces\r\n");
	strcpy(buffer, "echoii   21   220");
	handleConsoleLine(buffer);
	ASSERT_EQ(21, lastInteger);
	ASSERT_EQ(220, lastInteger2);

	printf("\r\addConsoleActionSSS\r\n");
	addConsoleActionSSS("echosss", testEchoSSS);
	strcpy(buffer, "echosss 111 222 333");
	handleConsoleLine(buffer);
	ASSERT_EQ(111, atoi(lastFirst));
	ASSERT_EQ(333, atoi(lastThird));

	strcpy(buffer, "echosss \" 1\" 222 333");
	handleConsoleLine(buffer);
	ASSERT_TRUE(strEqual("\" 1\"", lastFirst));

	printf("\r\addConsoleActionFFF\r\n");
	addConsoleActionFFF("echofff", testEchoFFF);
	strcpy(buffer, "echofff 1.0 2 00003.0");
	handleConsoleLine(buffer);

	ASSERT_EQ(1.0, fFirst);
	ASSERT_EQ(2.0, fSecond);
	ASSERT_EQ(3.0, fThird);
}

TEST(misc, testFLStack) {
	FLStack<int, 4> stack;
	ASSERT_EQ(0, stack.size());

	stack.push(123);
	stack.push(234);
	ASSERT_EQ(2, stack.size());

	int v = stack.pop();
	ASSERT_EQ(234, v);
	ASSERT_EQ(1, stack.size());
	ASSERT_EQ(123, stack.get(0));

	v = stack.pop();
	ASSERT_EQ(123, v);
	ASSERT_EQ(0, stack.size());

	stack.push(123);
	stack.push(234);
	stack.push(345);
	stack.push(456);
	ASSERT_EQ(4, stack.size());

	stack.remove(123);
	ASSERT_EQ(456, stack.get(0));
	ASSERT_EQ(3, stack.size());
}

static char buff[32];

TEST(misc, testMisc) {
	strcpy(buff, "  ab  ");
	// we need a mutable array here
	ASSERT_TRUE(strEqual("ab", efiTrim(buff)));


	{
		float v = atoff("1.0");
		assertEqualsM("atoff", 1.0, v);
	}
	{
		float v = atoff("nan");
		ASSERT_TRUE(cisnan(v)) << "NaN atoff";
	}
	{
		float v = atoff("N");
		ASSERT_TRUE(cisnan(v)) << "NaN atoff";
	}

//	ASSERT_EQ(true, strEqual("spa3", getPinName(SPARKOUT_3_OUTPUT)));
//	ASSERT_EQ(SPARKOUT_12_OUTPUT, getPinByName("spa12"));
}

int getRusEfiVersion(void) {
	return TS_FILE_VERSION;
}

TEST(util, PeakDetect) {
	constexpr int startTime = 50;
	constexpr int timeout = 100;
	PeakDetect<int, timeout> dut;

	// Set a peak
	EXPECT_EQ(dut.detect(1000, startTime), 1000);

	// Smaller value at the same time is ignored
	EXPECT_EQ(dut.detect(500, startTime), 1000);

	// Larger value at the same time raises the peak
	EXPECT_EQ(dut.detect(1500, startTime), 1500);

	// Small value at almost the timeout is ignored
	EXPECT_EQ(dut.detect(500, startTime + timeout - 1), 1500);

	// Small value past the timeout is used
	EXPECT_EQ(dut.detect(500, startTime + timeout + 1), 500);
}

TEST(util, WrapAround62) {
	// Random test
	{
		WrapAround62 t;
		uint32_t source = 0;
		uint64_t actual = 0;

		// Test random progression, positive and negative.
		uint32_t seed = time(NULL);
		printf("Testing with seed 0x%08x\n", seed);
		srand(seed);
		for (unsigned i = 0; i < 10000; i++) {
			int32_t delta = rand();
			if (delta < 0) {
				delta = ~delta;
			}
			delta -= RAND_MAX >> 1;

			// Cap negative test
			if (delta < 0 && -delta > actual) {
				delta = -actual;
			}

			source += delta;
			actual += delta;

			uint64_t next = t.update(source);
			EXPECT_EQ(actual, next);
		}
	}

	// More pointed test for expected edge conditions
	{
		WrapAround62 t;

		EXPECT_EQ(t.update(0x03453455), 0x003453455LL);
		EXPECT_EQ(t.update(0x42342323), 0x042342323LL);
		EXPECT_EQ(t.update(0x84356345), 0x084356345LL);
		EXPECT_EQ(t.update(0x42342323), 0x042342323LL);
		EXPECT_EQ(t.update(0x84356345), 0x084356345LL);
		EXPECT_EQ(t.update(0xC5656565), 0x0C5656565LL);
		EXPECT_EQ(t.update(0x01122112), 0x101122112LL); // Wrap around!
		EXPECT_EQ(t.update(0xC5656565), 0x0C5656565LL);
		EXPECT_EQ(t.update(0x84356345), 0x084356345LL);
		EXPECT_EQ(t.update(0xC5656565), 0x0C5656565LL);
		EXPECT_EQ(t.update(0x01122112), 0x101122112LL); // Wrap around!
		EXPECT_EQ(t.update(0x42342323), 0x142342323LL);
		EXPECT_EQ(t.update(0x84356345), 0x184356345LL);
		EXPECT_EQ(t.update(0x42342323), 0x142342323LL);
		EXPECT_EQ(t.update(0x84356345), 0x184356345LL);
		EXPECT_EQ(t.update(0xC5656565), 0x1C5656565LL);
		EXPECT_EQ(t.update(0x01122112), 0x201122112LL); // Wrap around!
		EXPECT_EQ(t.update(0xC5656565), 0x1C5656565LL);
		EXPECT_EQ(t.update(0x84356345), 0x184356345LL);
		EXPECT_EQ(t.update(0xC5656565), 0x1C5656565LL);
		EXPECT_EQ(t.update(0x01122112), 0x201122112LL); // Wrap around!
		EXPECT_EQ(t.update(0xC5656565), 0x1C5656565LL);
		EXPECT_EQ(t.update(0x84356345), 0x184356345LL);
		EXPECT_EQ(t.update(0x42342323), 0x142342323LL);
		EXPECT_EQ(t.update(0x01122112), 0x101122112LL);
		EXPECT_EQ(t.update(0x84356345), 0x084356345LL);
		EXPECT_EQ(t.update(0x42342323), 0x042342323LL);
		EXPECT_EQ(t.update(0x03453455), 0x003453455LL);
	}
}

TEST(util, isInRange) {
	EXPECT_FALSE(isInRange(5, 4, 10));
	EXPECT_TRUE(isInRange(5, 5, 10));
	EXPECT_TRUE(isInRange(5, 7, 10));
	EXPECT_TRUE(isInRange(5, 10, 10));
	EXPECT_FALSE(isInRange(5, 11, 10));
}

TEST(util, wrapAngle) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	// Test within range
	EXPECT_EQ(0, wrapAngleMethod(0));
	EXPECT_EQ(20, wrapAngleMethod(20));
	EXPECT_EQ(710, wrapAngleMethod(710));

	// Test off the top of the range
	EXPECT_EQ(1, wrapAngleMethod(721));
	EXPECT_EQ(20, wrapAngleMethod(740));
	EXPECT_EQ(719, wrapAngleMethod(720 + 719));
	EXPECT_EQ(0, wrapAngleMethod(720 + 720));
	EXPECT_EQ(5, wrapAngleMethod(10 * 720 + 5));

	// Test off the bottom of the range
	EXPECT_EQ(719, wrapAngleMethod(-1));
	EXPECT_EQ(360, wrapAngleMethod(-360));
	EXPECT_EQ(1, wrapAngleMethod(-719));
}

BigBufferUser getBigBufferCurrentUser();

TEST(BigBuffer, CppMagic) {
	BigBufferHandle h = getBigBuffer(BigBufferUser::ToothLogger);
	ASSERT_EQ(getBigBufferCurrentUser(), BigBufferUser::ToothLogger);
	h = {};
	ASSERT_EQ(getBigBufferCurrentUser(), BigBufferUser::None);
}
