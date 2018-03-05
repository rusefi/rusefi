/**
 * @file	test_util.c
 *
 * @date Dec 8, 2013
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include <string.h>

#include "test_util.h"
#include "cyclic_buffer.h"
#include "main.h"
#include "histogram.h"

#include "malfunction_central.h"
#include "cli_registry.h"
#include "unit_test_framework.h"

#include "nmea.h"
#include "efilib2.h"
#include "lcd_menu_tree.h"
#include "crc.h"
#include "fl_stack.h"
#include "io_pins.h"
#include "efiGpio.h"

void testCrc(void) {
	assertEquals(4, efiRound(4.4, 1));
	assertEquals(1.2, efiRound(1.2345, 0.1));

	print("*************************************** testCrc\r\n");

	const char * A = "A";

	assertEqualsM("crc8", 168, calc_crc((const crc_t *) A, 1));
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

static cyclic_buffer<int> sb;

void testOverflow64Counter(void) {
	print("*************************************** testOverflow64Counter\r\n");

	Overflow64Counter o;
	assertEquals(0, o.update(0));
	assertEquals(10, o.update(10));

	assertEquals(20, o.update(20));

	// overflow
	assertEquals(4294967296, o.update(0));
}

void testCyclicBuffer(void) {
	print("*************************************** testCyclicBuffer\r\n");

	{
		sb.add(10);

		assertEquals(10, sb.sum(3));

		sb.add(2);
		assertEquals(12, sb.sum(2));
	}
	{
		sb.clear();

		sb.add(1);
		sb.add(2);
		sb.add(3);
		sb.add(4);

		assertEquals(4, sb.maxValue(3));
		assertEquals(4, sb.maxValue(113));
		assertEqualsM("minValue(3)", 2, sb.minValue(3));
		assertEquals(1, sb.minValue(113));
	}

}

void testHistogram(void) {
	print("******************************************* testHistogram\r\n");

	initHistogramsModule();

	assertEquals(80, histogramGetIndex(239));
	assertEquals(223, histogramGetIndex(239239));
	assertEquals(364, histogramGetIndex(239239239));

	histogram_s h;

	initHistogram(&h, "test");

	int result[5];
	assertEquals(0, hsReport(&h, result));

	hsAdd(&h, 10);
	assertEquals(1, hsReport(&h, result));
	assertEquals(10, result[0]);

	// let's add same value one more time
	hsAdd(&h, 10);
	assertEquals(2, hsReport(&h, result));
	assertEquals(10, result[0]);
	assertEquals(10, result[1]);

	hsAdd(&h, 10);
	hsAdd(&h, 10);
	hsAdd(&h, 10);

	hsAdd(&h, 1000);
	hsAdd(&h, 100);

	assertEquals(5, hsReport(&h, result));

	assertEquals(5, result[0]);
	assertEquals(10, result[1]);
	assertEquals(10, result[2]);
	assertEquals(100, result[3]);
	// values are not expected to be exactly the same, it's the shape what matters
	assertEquals(1011, result[4]);
}

static void testMalfunctionCentralRemoveNonExistent() {
	print("******************************************* testMalfunctionCentralRemoveNonExistent\r\n");
	initMalfunctionCentral();

	// this should not crash
	removeError(OBD_Engine_Coolant_Temperature_Circuit_Malfunction);
}

static void testMalfunctionCentralSameElementAgain() {
	initMalfunctionCentral();
	print("******************************************* testMalfunctionCentralSameElementAgain\r\n");
	error_codes_set_s localCopy;

	addError(OBD_Engine_Coolant_Temperature_Circuit_Malfunction);
	addError(OBD_Engine_Coolant_Temperature_Circuit_Malfunction);
	getErrorCodes(&localCopy);
	assertEquals(1, localCopy.count);
}

static void testMalfunctionCentralRemoveFirstElement() {
	initMalfunctionCentral();
	print("******************************************* testMalfunctionCentralRemoveFirstElement\r\n");
	error_codes_set_s localCopy;

	obd_code_e firstElement = OBD_Engine_Coolant_Temperature_Circuit_Malfunction;
	addError(firstElement);

	obd_code_e secondElement = OBD_Intake_Air_Temperature_Circuit_Malfunction;
	addError(secondElement);
	getErrorCodes(&localCopy);
	assertEquals(2, localCopy.count);

	// let's remove first element - code
	removeError(firstElement);

	getErrorCodes(&localCopy);
	assertEquals(1, localCopy.count);
	assertEquals(secondElement, localCopy.error_codes[0]);
}

void testMalfunctionCentral(void) {
	testMalfunctionCentralRemoveNonExistent();
	testMalfunctionCentralSameElementAgain();
	testMalfunctionCentralRemoveFirstElement();

	print("******************************************* testMalfunctionCentral\r\n");
	initMalfunctionCentral();

	error_codes_set_s localCopy;

	// on start-up error storage should be empty
	getErrorCodes(&localCopy);
	assertEquals(0, localCopy.count);

	obd_code_e code = OBD_Engine_Coolant_Temperature_Circuit_Malfunction;
	// let's add one error and validate
	addError(code);

	getErrorCodes(&localCopy);
	assertEqualsM("count #1", 1, localCopy.count);
	assertEquals(code, localCopy.error_codes[0]);

	// let's remove value which is not in the collection
	removeError((obd_code_e) 22);
	// element not present - nothing to removed
	assertEquals(1, localCopy.count);
	assertEquals(code, localCopy.error_codes[0]);

	code = OBD_Intake_Air_Temperature_Circuit_Malfunction;
	addError(code);
	getErrorCodes(&localCopy);
	// todo:	assertEquals(2, localCopy.count);

	for (int code = 0; code < 100; code++) {
		addError((obd_code_e) code);
	}
	getErrorCodes(&localCopy);
	assertEquals(MAX_ERROR_CODES_COUNT, localCopy.count);

	// now we have full array and code below present
	removeError(code);
	getErrorCodes(&localCopy);
	assertEquals(MAX_ERROR_CODES_COUNT - 1, localCopy.count);
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

void testGpsParser(void) {
	print("******************************************* testGpsParser\r\n");

	strcpy(nmeaMessage, "");
	gps_location(&GPSdata, nmeaMessage);

	// we need to pass a mutable string, not a constant because the parser would be modifying the string
	strcpy(nmeaMessage, "$GPRMC,173843,A,3349.896,N,11808.521,W,000.0,360.0,230108,013.4,E*69");
	gps_location(&GPSdata, nmeaMessage);
	assertEqualsM("1 valid", 4, GPSdata.quality);
	assertEqualsM("1 latitude", 3349.896, GPSdata.latitude);
	assertEqualsM("1 longitude", 11808.521, GPSdata.longitude);
	assertEqualsM("1 speed", 0, GPSdata.speed);
// 	assertEqualsM("1 altitude", 0, GPSdata.altitude);	// GPRMC not overwrite altitude
	assertEqualsM("1 course", 360, GPSdata.course);

	strcpy(nmeaMessage, "$GPGGA,111609.14,5001.27,N,3613.06,E,3,08,0.0,10.2,M,0.0,M,0.0,0000*70");
	gps_location(&GPSdata, nmeaMessage);
	assertEqualsM("2 valid", 3, GPSdata.quality);		// see field details
	assertEqualsM("2 latitude", 50.0212, GPSdata.latitude);
	assertEqualsM("2 longitude", 36.2177, GPSdata.longitude);
	assertEqualsM("2 speed", 0, GPSdata.speed);
	assertEqualsM("2 altitude", 10.2, GPSdata.altitude);
//	assertEqualsM("2 course", 0, GPSdata.course);  // GPGGA not overwrite course

	strcpy(nmeaMessage, "$GPRMC,111609.14,A,5001.27,N,3613.06,E,11.2,0.0,261206,0.0,E*50");
	gps_location(&GPSdata, nmeaMessage);
	assertEqualsM("3 valid", 4, GPSdata.quality);
	assertEqualsM("3 latitude", 5001.27, GPSdata.latitude);
	assertEqualsM("3 longitude", 3613.06, GPSdata.longitude);
	assertEqualsM("3 speed", 11.2, GPSdata.speed);
//	assertEqualsM("3 altitude", 0, GPSdata.altitude);  // GPRMC not overwrite altitude
	assertEqualsM("3 course", 0, GPSdata.course);
	assertEqualsM("3 GPS yy", 2006, GPSdata.GPStm.tm_year + 1900);
	assertEqualsM("3 GPS mm", 12, GPSdata.GPStm.tm_mon);
	assertEqualsM("3 GPS yy", 26, GPSdata.GPStm.tm_mday);
	assertEqualsM("3 GPS hh", 11, GPSdata.GPStm.tm_hour);
	assertEqualsM("3 GPS mm", 16, GPSdata.GPStm.tm_min);
	assertEqualsM("3 GPS ss", 9, GPSdata.GPStm.tm_sec);

	// check again first one
	// we need to pass a mutable string, not a constant because the parser would be modifying the string
	strcpy(nmeaMessage, "$GPRMC,173843,A,3349.896,N,11808.521,W,000.0,360.0,230108,013.4,E*69");
	gps_location(&GPSdata, nmeaMessage);
	assertEqualsM("4 valid", 4, GPSdata.quality);
	assertEqualsM("4 latitude", 3349.896, GPSdata.latitude);
	assertEqualsM("4 longitude", 11808.521, GPSdata.longitude);
	assertEqualsM("4 speed", 0, GPSdata.speed);
	assertEqualsM("4 course", 360, GPSdata.course);
}

// this buffer is needed because on Unix you would not be able to change static char constants
static char buffer[300];

void testConsoleLogic(void) {
	print("******************************************* testConsoleLogic\r\n");
	resetConsoleActions();

	helpCommand();

	char * cmd = "he ha";
	assertEquals(2, findEndOfToken(cmd));

	cmd = "\"hee\" ha";
	assertEquals(5, findEndOfToken(cmd));

	cmd = "\"h e\" ha";
	assertEquals(5, findEndOfToken(cmd));

	strcpy(buffer, "echo");
	assertTrue(strEqual("echo", unquote(buffer)));

	strcpy(buffer, "\"echo\"");
	assertTrueM("unquote quoted", strEqual("echo", unquote(buffer)));

	char *ptr = validateSecureLine(UNKNOWN_COMMAND);
	assertEquals(0, strcmp(UNKNOWN_COMMAND, ptr));
	assertEquals(10, tokenLength(UNKNOWN_COMMAND));

	// handling invalid token should work
	strcpy(buffer, "sdasdafasd asd");
	handleConsoleLine(buffer);

	print("\r\naddConsoleActionI\r\n");
	addConsoleActionI("echoi", testEchoI);
	strcpy(buffer, "echoi 239");
	handleConsoleLine(buffer);
	assertEquals(239, lastInteger);

	print("\r\naddConsoleActionI 240 with two spaces\r\n");
	addConsoleActionI("echoi", testEchoI);
	strcpy(buffer, "echoi  240");
	handleConsoleLine(buffer);
	assertEquals(240, lastInteger);


	print("\r\naddConsoleActionII\r\n");
	addConsoleActionII("echoii", testEchoII);
	strcpy(buffer, "echoii 22 239");
	handleConsoleLine(buffer);
	assertEquals(22, lastInteger);
	assertEquals(239, lastInteger2);

	print("\r\naddConsoleActionII three spaces\r\n");
	addConsoleActionII("echoii", testEchoII);
	strcpy(buffer, "echoii   21   220");
	handleConsoleLine(buffer);
	assertEquals(21, lastInteger);
	assertEquals(220, lastInteger2);

	print("\r\addConsoleActionSSS\r\n");
	addConsoleActionSSS("echosss", testEchoSSS);
	strcpy(buffer, "echosss 111 222 333");
	handleConsoleLine(buffer);
	assertEquals(111, atoi(lastFirst));
	assertEquals(333, atoi(lastThird));

	strcpy(buffer, "echosss \" 1\" 222 333");
	handleConsoleLine(buffer);
	assertTrue(strEqual("\" 1\"", lastFirst));

	//addConsoleActionSSS("GPS", testGpsParser);
}

void testFLStack(void) {
	print("******************************************* testFLStack\r\n");

	FLStack<int, 4> stack;
	assertEquals(0, stack.size());

	stack.push(123);
	stack.push(234);
	assertEquals(2, stack.size());

	int v = stack.pop();
	assertEquals(234, v);
	assertEquals(1, stack.size());
	assertEquals(123, stack.get(0));

	v = stack.pop();
	assertEquals(123, v);
	assertEquals(0, stack.size());

	stack.push(123);
	stack.push(234);
	stack.push(345);
	stack.push(456);
	assertEquals(4, stack.size());

	stack.remove(123);
	assertEquals(456, stack.get(0));
	assertEquals(3, stack.size());
}

static char buff[32];

void testMisc(void) {
	print("******************************************* testMisc\r\n");
	strcpy(buff, "  ab  ");
	// we need a mutable array here
	assertTrue(strEqual("ab", efiTrim(buff)));

//	assertEquals(true, strEqual("spa3", getPinName(SPARKOUT_3_OUTPUT)));
//	assertEquals(SPARKOUT_12_OUTPUT, getPinByName("spa12"));
}

void testMenuTree(void) {
	print("******************************************* testMenuTree\r\n");

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

	assertEquals(0, miTopLevel1.index);
	assertEquals(1, miTopLevel2.index);
	assertEquals(4, miTopLevel5.index);

	tree.init(&miTopLevel1, 3);

	tree.nextItem();
	assertTrue(tree.topVisible == &miTopLevel1);
	assertTrue(tree.current == &miTopLevel2);

	tree.back();
	assertTrue(tree.current == &miTopLevel2); // no 'back' since we are on the top level already

	tree.nextItem();
	assertTrue(tree.topVisible == &miTopLevel1);
	assertTrue(tree.current == &miTopLevel3);

	tree.nextItem();
	assertTrue(tree.topVisible == &miTopLevel2);
	assertTrue(tree.current == &miTopLevel4);

	tree.enterSubMenu();
	assertTrueM("still same", tree.current == &miTopLevel4); // no children in this one

	tree.nextItem();
	assertTrue(tree.topVisible == &miTopLevel3);
	assertTrueM("tl5", tree.current == &miTopLevel5);

	tree.nextItem();
	assertTrueM("tl1 t", tree.topVisible == &miTopLevel1);
	assertTrueM("tl1 c", tree.current == &miTopLevel1);

	tree.nextItem();
	tree.nextItem();
	tree.nextItem();
	tree.nextItem();

	tree.enterSubMenu();
	assertTrue(tree.current == &miSubMenu5_1);

	tree.back();
	assertTrue(tree.current == &miTopLevel1);

}
