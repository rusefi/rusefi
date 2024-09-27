/**
 * @file	test_util.c
 *
 * @date Dec 8, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include <string.h>

#include "histogram.h"

#include "malfunction_central.h"
#include "cli_registry.h"

#include "nmea.h"
#include "mmc_card.h"
#include "fl_stack.h"

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

TEST(util, histogram) {
	initHistogramsModule();

	ASSERT_EQ(80, histogramGetIndex(239));
	ASSERT_EQ(223, histogramGetIndex(239239));
	ASSERT_EQ(364, histogramGetIndex(239239239));

	histogram_s h;

	initHistogram(&h, "test");

	int result[5];
	ASSERT_EQ(0, hsReport(&h, result));

	hsAdd(&h, 10);
	ASSERT_EQ(1, hsReport(&h, result));
	ASSERT_EQ(10, result[0]);

	// let's add same value one more time
	hsAdd(&h, 10);
	ASSERT_EQ(2, hsReport(&h, result));
	ASSERT_EQ(10, result[0]);
	ASSERT_EQ(10, result[1]);

	hsAdd(&h, 10);
	hsAdd(&h, 10);
	hsAdd(&h, 10);

	hsAdd(&h, 1000);
	hsAdd(&h, 100);

	ASSERT_EQ(5, hsReport(&h, result));

	ASSERT_EQ(5, result[0]);
	ASSERT_EQ(10, result[1]);
	ASSERT_EQ(10, result[2]);
	ASSERT_EQ(100, result[3]);
	// values are not expected to be exactly the same, it's the shape what matters
	ASSERT_EQ(1011, result[4]);
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

	for (int code = 0; code < 100; code++) {
		addError((ObdCode) code);
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

static loc_t GPSdata;

static char nmeaMessage[1000];

TEST(misc, testGpsParser) {
	strcpy(nmeaMessage, "");
	gps_location(&GPSdata, nmeaMessage);

	// we need to pass a mutable string, not a constant because the parser would be modifying the string
	strcpy(nmeaMessage, "$GPRMC,173843,A,3349.896,N,11808.521,W,000.0,360.0,230108,013.4,E*69");
	gps_location(&GPSdata, nmeaMessage);
	ASSERT_EQ( 4,  GPSdata.quality) << "1 valid";
	assertEqualsM("1 latitude", 3349.896, GPSdata.latitude);
	assertEqualsM("1 longitude", 11808.521, GPSdata.longitude);
	ASSERT_EQ( 0,  GPSdata.speed) << "1 speed";
// 	ASSERT_EQ( 0,  GPSdata.altitude) << "1 altitude";	// GPRMC not overwrite altitude
	ASSERT_EQ( 360,  GPSdata.course) << "1 course";

	strcpy(nmeaMessage, "$GPGGA,111609.14,5001.27,N,3613.06,E,3,08,0.0,10.2,M,0.0,M,0.0,0000*70");
	gps_location(&GPSdata, nmeaMessage);
	ASSERT_EQ( 3,  GPSdata.quality) << "2 valid";		// see field details
	assertEqualsM("2 latitude", 50.0212, GPSdata.latitude);
	assertEqualsM("2 longitude", 36.2177, GPSdata.longitude);
	ASSERT_EQ( 0,  GPSdata.speed) << "2 speed";
	assertEqualsM("2 altitude", 10.2, GPSdata.altitude);
//	ASSERT_EQ( 0,  GPSdata.course) << "2 course";  // GPGGA not overwrite course

	strcpy(nmeaMessage, "$GPRMC,111609.14,A,5001.27,N,3613.06,E,11.2,0.0,261206,0.0,E*50");
	gps_location(&GPSdata, nmeaMessage);
	ASSERT_EQ( 4,  GPSdata.quality) << "3 valid";
	assertEqualsM("3 latitude", 5001.27, GPSdata.latitude);
	assertEqualsM("3 longitude", 3613.06, GPSdata.longitude);
	assertEqualsM("3 speed", 11.2, GPSdata.speed);
//	ASSERT_EQ( 0,  GPSdata.altitude) << "3 altitude";  // GPRMC not overwrite altitude
	ASSERT_EQ( 0,  GPSdata.course) << "3 course";
	ASSERT_EQ( 2006,  GPSdata.time.year + 1900) << "3 GPS yy";
	ASSERT_EQ( 12,  GPSdata.time.month) << "3 GPS mm";
	ASSERT_EQ( 26,  GPSdata.time.day) << "3 GPS dd";
	ASSERT_EQ( 11,  GPSdata.time.hour) << "3 GPS hh";
	ASSERT_EQ( 16,  GPSdata.time.minute) << "3 GPS mm";
	ASSERT_EQ( 9,  GPSdata.time.second) << "3 GPS ss";

	// check again first one
	// we need to pass a mutable string, not a constant because the parser would be modifying the string
	strcpy(nmeaMessage, "$GPRMC,173843,A,3349.896,N,11808.521,W,000.0,360.0,230108,013.4,E*69");
	gps_location(&GPSdata, nmeaMessage);
	ASSERT_EQ( 4,  GPSdata.quality) << "4 valid";
	assertEqualsM("4 latitude", 3349.896, GPSdata.latitude);
	assertEqualsM("4 longitude", 11808.521, GPSdata.longitude);
	ASSERT_EQ( 0,  GPSdata.speed) << "4 speed";
	ASSERT_EQ( 360,  GPSdata.course) << "4 course";
}

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

	//addConsoleActionSSS("GPS", testGpsParser);
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
		ASSERT_TRUE(std::isnan(v)) << "NaN atoff";
	}
	{
		float v = atoff("N");
		ASSERT_TRUE(std::isnan(v)) << "NaN atoff";
	}

//	ASSERT_EQ(true, strEqual("spa3", getPinName(SPARKOUT_3_OUTPUT)));
//	ASSERT_EQ(SPARKOUT_12_OUTPUT, getPinByName("spa12"));
}

int getRusEfiVersion() {
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
