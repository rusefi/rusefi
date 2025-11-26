/**
 * @file	rusEfiFunctionalTest.cpp
 *
 * @date Mar 1, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "boards.h"
#include "rusEfiFunctionalTest.h"
#include "console_io.h"
#include "eficonsole.h"
#include "trigger_central.h"
#include "datalogging.h"
#include "engine_sniffer.h"
#include "status_loop.h"
#include "trigger_emulator_algo.h"
#include "main_trigger_callback.h"
#include "bench_test.h"
#include "tunerstudio.h"
#include "map_averaging.h"
#include "memstreams.h"
#include <chprintf.h>
#include "rusefi_lua.h"
#include "can_hw.h"
#include "flash_main.h"
#include "can_msg_tx.h"
#include "fifo_buffer.h"
#include "script_impl.h"
#include <vector>

extern fifo_buffer<CANTxFrame, 1024> txCanBuffer;

#define DEFAULT_SIM_RPM 1200
#define DEFAULT_SNIFFER_THR 2500

#if EFI_ENGINE_SNIFFER
extern WaveChart waveChart;
#endif

int getRemainingStack(thread_t*) {
	return 99999;
}

static void assertString(const char*actual, const char *expected) {
	if (strcmp(actual, expected) != 0) {
		printf("assertString FAILED\n");
		criticalError("chprintf test: got %s while %s", actual, expected);
	}
}

static void runChprintfTest() {
	static MemoryStream ts;
	static char testBuffer[200];
	msObjectInit(&ts, (uint8_t *) testBuffer, sizeof(testBuffer), 0);


	ts.eos = 0; // reset
	chprintf((BaseSequentialStream*)&ts, "%.2f - %.2f", NAN, NAN);
	ts.buffer[ts.eos] = 0;
	assertString(testBuffer, "NaN - NaN");

// it's a very, very long and mostly forgotten story how this became our %.2f precision format
	ts.eos = 0; // reset
	chprintf((BaseSequentialStream*)&ts, "%.2f/%.4f/%.4f", 0.239f, 239.932, 0.1234);
	ts.buffer[ts.eos] = 0;

	assertString(testBuffer, "0.23/239.9320/0.1234");


	{
		LoggingWithStorage testLogging("test");
		testLogging.appendFloat(1.23, 5);
		testLogging.appendFloat(1.234, 2);
		assertString(testLogging.buffer, "1.230001.23");

	}

	{
		LoggingWithStorage testLogging("test");
		testLogging.appendFloat(-1.23, 5);
		assertString(testLogging.buffer, "-1.23000");
	}
	{
		LoggingWithStorage testLogging("test");
		testLogging.appendPrintf( "a%.2fb%fc", -1.2, -3.4);
		// different compilers produce different 8th digit
		testLogging.buffer[strlen(testLogging.buffer) - 2] = 'X';
		assertString(testLogging.buffer, "a-1.20b-3.40000009Xc");
	}

}

static void runCanGpioTest() {
}

// todo: reuse intFlashWrite method?
static void writeSimulatorTune(const char *fileName) {
	FILE *ptr = fopen(fileName, "wb");
	if (ptr == nullptr) {
		printf("ERROR creating file: [%s]\n", SIMULATOR_TUNE_BIN_FILE_NAME);
		printf("Please check folder exists and is writeable.");
		return;
	}
	fwrite(&persistentState.persistentConfiguration, 1, sizeof(persistentState.persistentConfiguration), ptr);
	fclose(ptr);
}

static void runToothLoggerTest() {
#if EFI_TOOTH_LOGGER
	EnableToothLoggerIfNotEnabled();

	{
		// no data yet
		CompositeBuffer * toothBuffer = GetToothLoggerBufferNonblocking();
		if (toothBuffer != nullptr) {
			criticalError("nullptr buffer expected");
		}
	}

	getTriggerCentral()->isEngineSnifferEnabled = true;

	for (int i = 0;i < 400;i++) {
		efitick_t nowNt = getTimeNowNt();
		LogTriggerTooth(SHAFT_SECONDARY_RISING, nowNt);
	}

	{
		CompositeBuffer * toothBuffer = GetToothLoggerBufferNonblocking();
		criticalAssertVoid(toothBuffer != nullptr, "filled buffer expected");

		size_t size = toothBuffer->nextIdx * sizeof(composite_logger_s);
		criticalAssertVoid(size != 0, "Positive payload size expected");

		const uint8_t* ptr = reinterpret_cast<const uint8_t*>(toothBuffer->buffer);
		criticalAssertVoid(ptr != nullptr, "Payload reference expected");
	}
#endif // EFI_TOOTH_LOGGER
}

static void assertNear(float actual, float expected) {
    float delta = absF(actual - expected);
    if (delta > 0.01)
        throw std::runtime_error("assertNear actual=" + std::to_string(actual) + " expected=" + std::to_string(expected));
}

static void	runNotSquareTest() {
    // [tag:runNotSquareTest]
    assertNear(getscriptTable(3)->getValue(0, 20), 140);
    assertNear(getscriptTable(3)->getValue(0, 30), 240);

    assertNear(getscriptTable(3)->getValue(3000, 20), 144.384);
}

static void writeEngineTypeDefaultConfig(engine_type_e type) {
	engineConfiguration->engineType = type;
	resetConfigurationExt(engineConfiguration->engineType);
	char fileName[3000];
	sprintf(fileName, "%s_%d%s",
	    SIMULATOR_TUNE_BIN_FILE_NAME_PREFIX,
	    (int)engineConfiguration->engineType,
	    SIMULATOR_TUNE_BIN_FILE_NAME_SUFFIX
	);
	writeSimulatorTune(fileName);
}

void rusEfiFunctionalTest() {
  printf("Running rusEFI simulator version: [%d]", (int)getRusEfiVersion());
  // todo: do we still need 'printToConsole' for any reason?!
	printToConsole("Running rusEFI simulator version:");
	static char versionBuffer[20];
	itoa10(versionBuffer, (int)getRusEfiVersion());
	printToConsole(versionBuffer);

	engine->setConfig();

	startLoggingProcessor();

	initDataStructures();
	initializeConsole();

	// todo: reduce code duplication with initRealHardwareEngineController

	initFlash();

  printf("[CannedTunes]: %d \n", (int)getLastEngineType());
  // [CannedTunes] let's export all just for simplicity. See also WriteSimulatorConfiguration.java
	for (size_t typeIndex = 0;typeIndex<(size_t)getLastEngineType();typeIndex++) {
    engine_type_e type = (engine_type_e)typeIndex;
		printf("[CannedTunes]: testing engineConfig: %d \n", (int)type);
		writeEngineTypeDefaultConfig(type);
	}

	// this here is really 'reset to default configuration'
	loadConfiguration();

	commonInitEngineController();

    commonEarlyInit();

#if EFI_EMULATE_POSITION_SENSORS
	enableTriggerStimulator(false);
#endif

	writeSimulatorTune(SIMULATOR_TUNE_BIN_FILE_NAME);

    /**
     * !!!! TESTS !
     */
	runChprintfTest();
	runToothLoggerTest();
	runCanGpioTest();
	runNotSquareTest();
    /**
     * end of TESTS !
     */
#if EFI_EMULATE_POSITION_SENSORS
	setTriggerEmulatorRPM(DEFAULT_SIM_RPM);
#endif
	engineConfiguration->engineSnifferRpmThreshold = DEFAULT_SNIFFER_THR;

	startSerialChannels();

	engineConfiguration->enableVerboseCanTx = true;

	initPeriodicEvents(); //TODO: replace to initMainLoop();
	initMainLoop();
	rememberCurrentConfiguration();

	extern bool main_loop_started;
	main_loop_started = true;
}

void printPendingMessages() {
	updateDevConsoleState();
#if EFI_ENGINE_SNIFFER
	waveChart.publishIfFull();
#endif
}

int isSerialOverTcpReady;

bool isCommandLineConsoleReady() {
	return isSerialOverTcpReady;
}

void applyNewConfiguration() {
}

void onFatalError(const char *msg, const char * file, int line) {
	printf("onFatalError %s %s%d", msg, file, line);
	exit(-1);
}

void logMsg(const char * /*format*/, ...) {
//	FILE * fp;
//	fp = fopen ("simulator.log", "a");
//
//	va_list(args);
//	va_start(args, format);
//	vfprintf(fp, format, args);
//
//	fclose(fp);
}

#if HAL_USE_CAN

CANDriver* getCanDevice(size_t index)
{
	switch (index) {
	case 0:
		return &CAND1;
	case 1:
		return &CAND2;
	case 2:
		return &CAND3;
	}

	return nullptr;
}

#endif // HAL_USE_CAN

static uint8_t wrapOutBuffer[BLOCKING_FACTOR + 2];

static uint8_t readInt8FromCan(char * & data, int & incomingPacketSize) {
	incomingPacketSize--;
	return (uint8_t)(*data++);
}

static int32_t readInt32FromCan(char * & data, int & incomingPacketSize) {
	int32_t value = 0;
	value |= ((*data++) & 0xff) << 24;
	value |= ((*data++) & 0xff) << 16;
	value |= ((*data++) & 0xff) << 8;
	value |= (*data++) & 0xff;

	incomingPacketSize -= 4;
	return value;
}

void handleWrapCan(TsChannelBase* tsChannel, char *data, int incomingPacketSize) {
	std::vector<int> responseEids;
	// process incoming CAN packets (at least 2 bytes are expected to store the number of EIDs and packets)
	if (incomingPacketSize >= 2) {
		responseEids.resize(readInt8FromCan(data, incomingPacketSize));
		for (int& eid : responseEids) {
			eid = readInt32FromCan(data, incomingPacketSize);
		}
		int numPackets = readInt8FromCan(data, incomingPacketSize);

		// if we received a packet, we treat it as a query, and then
		// we want to see some packets as response to the query,
		// so we need to clear the CAN buffer because it may be full already
		if (numPackets > 0) {
			txCanBuffer.clear();
		}

		for (int i = 0; i < numPackets && incomingPacketSize >= 16; i++) {
			CANRxFrame rxFrame;
			rxFrame.FMI = data[0];
			rxFrame.TIME = (data[1] << 8) | data[2];
			rxFrame.DLC = data[3] & 0xf;
			rxFrame.RTR = (data[3] >> 4) & 1;
			rxFrame.IDE = (data[3] >> 5) & 1;
			rxFrame.EID = (data[7] << 24) | (data[6] << 16) | (data[5] << 8) | (data[4]);
			memcpy(rxFrame.data8, data + 8, sizeof(rxFrame.data8));

			processCanRxMessage(0, rxFrame, getTimeNowNt());

			data += sizeof(rxFrame);
			incomingPacketSize -= sizeof(rxFrame);
		}
	}

    uint16_t numPackets = 0;
    int outputSize = 2;

    while (txCanBuffer.getCount() > 0 && (outputSize + sizeof(CANTxFrame)) <= BLOCKING_FACTOR) {
    	CANTxFrame f = txCanBuffer.get();
    	// filter out CAN packets
		for (int eid : responseEids) {
    		if (f.EID == eid) {
		        void *frame = (void *)&f;
		        memcpy((void*)(wrapOutBuffer + outputSize), frame, sizeof(CANTxFrame));
		        outputSize += sizeof(CANTxFrame);
		        numPackets++;
		        break;
		    }
	    }
	}

    memcpy(wrapOutBuffer, &numPackets, 2);
    tsChannel->sendResponse(TS_CRC, wrapOutBuffer, outputSize, true);
}
