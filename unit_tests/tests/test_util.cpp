/**
 * @file	test_util.c
 *
 * @date Dec 8, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include <string.h>

#include "cyclic_buffer.h"
#include "histogram.h"

#include "malfunction_central.h"
#include "cli_registry.h"

#include "nmea.h"
#include "mmc_card.h"
#include "lcd_menu_tree.h"
#include "crc.h"
#include "fl_stack.h"

TEST(util, negativeZero) {
	ASSERT_TRUE(IS_NEGATIVE_ZERO(-0.0));

	ASSERT_FALSE(IS_NEGATIVE_ZERO(-10.0));
	ASSERT_FALSE(IS_NEGATIVE_ZERO(10.0));
	ASSERT_FALSE(IS_NEGATIVE_ZERO(0.0));
}

TEST(util, crc) {
	ASSERT_EQ(4, efiRound(4.4, 1));
	ASSERT_FLOAT_EQ(1.2, efiRound(1.2345, 0.1));
	ASSERT_FLOAT_EQ(0.2, efiRound(0.2345, 0.1));

	const char * A = "A";

	ASSERT_EQ( 168,  calc_crc((const crc_t *) A, 1)) << "crc8";
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
	removeError(OBD_Engine_Coolant_Temperature_Circuit_Malfunction);
}

static void testMalfunctionCentralSameElementAgain() {
	clearWarnings();
	error_codes_set_s localCopy;

	addError(OBD_Engine_Coolant_Temperature_Circuit_Malfunction);
	addError(OBD_Engine_Coolant_Temperature_Circuit_Malfunction);
	getErrorCodes(&localCopy);
	ASSERT_EQ(1, localCopy.count);
}

static void testMalfunctionCentralRemoveFirstElement() {
	clearWarnings();
	error_codes_set_s localCopy;

	obd_code_e firstElement = OBD_Engine_Coolant_Temperature_Circuit_Malfunction;
	addError(firstElement);

	obd_code_e secondElement = OBD_Intake_Air_Temperature_Circuit_Malfunction;
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

	obd_code_e code = OBD_Engine_Coolant_Temperature_Circuit_Malfunction;
	// let's add one error and validate
	addError(code);

	getErrorCodes(&localCopy);
	ASSERT_EQ( 1,  localCopy.count) << "count #1";
	ASSERT_EQ(code, localCopy.error_codes[0]);

	// let's remove value which is not in the collection
	removeError((obd_code_e) 22);
	// element not present - nothing to removed
	ASSERT_EQ(1, localCopy.count);
	ASSERT_EQ(code, localCopy.error_codes[0]);

	code = OBD_Intake_Air_Temperature_Circuit_Malfunction;
	addError(code);
	getErrorCodes(&localCopy);
	// todo:	ASSERT_EQ(2, localCopy.count);

	for (int code = 0; code < 100; code++) {
		addError((obd_code_e) code);
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
	ASSERT_EQ( 2006,  GPSdata.GPStm.tm_year + 1900) << "3 GPS yy";
	ASSERT_EQ( 12,  GPSdata.GPStm.tm_mon) << "3 GPS mm";
	ASSERT_EQ( 26,  GPSdata.GPStm.tm_mday) << "3 GPS yy";
	ASSERT_EQ( 11,  GPSdata.GPStm.tm_hour) << "3 GPS hh";
	ASSERT_EQ( 16,  GPSdata.GPStm.tm_min) << "3 GPS mm";
	ASSERT_EQ( 9,  GPSdata.GPStm.tm_sec) << "3 GPS ss";

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

	char *ptr = validateSecureLine(UNKNOWN_COMMAND);
	ASSERT_EQ(0, strcmp(UNKNOWN_COMMAND, ptr));
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
		ASSERT_TRUE(cisnan(v)) << "NaN atoff";
	}
	{
		float v = atoff("N");
		ASSERT_TRUE(cisnan(v)) << "NaN atoff";
	}

//	ASSERT_EQ(true, strEqual("spa3", getPinName(SPARKOUT_3_OUTPUT)));
//	ASSERT_EQ(SPARKOUT_12_OUTPUT, getPinByName("spa12"));
}

TEST(misc, testMenuTree) {
	MenuItem ROOT(NULL, NULL);

	MenuTree tree(&ROOT);

	MenuItem miTopLevel1(tree.root, "top level 1");
	MenuItem miTopLevel2(tree.root, "top level 2");
	MenuItem miTopLevel3(tree.root, LL_RPM);
	MenuItem miTopLevel4(tree.root, "top level 4");
	MenuItem miTopLevel5(tree.root, "top level 5");

	MenuItem miSubMenu1_1(&miTopLevel1, "sub menu 1 1");
	MenuItem miSubMenu1_2(&miTopLevel1, "sub menu 1 2");

	MenuItem miSubMenu5_1(&miTopLevel5, "sub menu 5 1");
	MenuItem miSubMenu5_2(&miTopLevel5, "sub menu 5 2");

	ASSERT_EQ(0, miTopLevel1.index);
	ASSERT_EQ(1, miTopLevel2.index);
	ASSERT_EQ(4, miTopLevel5.index);

	tree.init(&miTopLevel1, 3);

	tree.nextItem();
	ASSERT_TRUE(tree.topVisible == &miTopLevel1);
	ASSERT_TRUE(tree.current == &miTopLevel2);

	tree.back();
	ASSERT_TRUE(tree.current == &miTopLevel2); // no 'back' since we are on the top level already

	tree.nextItem();
	ASSERT_TRUE(tree.topVisible == &miTopLevel1);
	ASSERT_TRUE(tree.current == &miTopLevel3);

	tree.nextItem();
	ASSERT_TRUE(tree.topVisible == &miTopLevel2);
	ASSERT_TRUE(tree.current == &miTopLevel4);

	tree.enterSubMenu();
	ASSERT_TRUE(tree.current == &miTopLevel4) << "still same"; // no children in this one

	tree.nextItem();
	ASSERT_TRUE(tree.topVisible == &miTopLevel3);
	ASSERT_TRUE(tree.current == &miTopLevel5) << "tl5";

	tree.nextItem();
	ASSERT_TRUE(tree.topVisible == &miTopLevel1) << "tl1 t";
	ASSERT_TRUE(tree.current == &miTopLevel1) << "tl1 c";

	tree.nextItem();
	tree.nextItem();
	tree.nextItem();
	tree.nextItem();

	tree.enterSubMenu();
	ASSERT_TRUE(tree.current == &miSubMenu5_1);

	tree.back();
	ASSERT_TRUE(tree.current == &miTopLevel1);
}

int getRusEfiVersion(void) {
	return 776655;
}

TEST(util, datalogging) {
	char LOGGING_BUFFER[1000];
	Logging logger("settings control", LOGGING_BUFFER, sizeof(LOGGING_BUFFER));

	printCurrentState(&logger, 239, "DEFAULT_FRANKENSO", "ID");
//	printf("Got [%s]\r\n", LOGGING_BUFFER);
//	ASSERT_STREQ("rusEfiVersion,776655@321ID DEFAULT_FRANKENSO 239,", LOGGING_BUFFER);
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
		srandom(seed);
		for (unsigned i = 0; i < 10000; i++) {
			int32_t delta = random();
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
