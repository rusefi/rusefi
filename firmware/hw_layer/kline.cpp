/**
 * this file is mostly about SEFMJ early 2000s specific honda K-line protocol
 * https://rusefi.com/forum/viewtopic.php?f=4&t=2514
 */

#include "pch.h"
#include "kline.h"
#include "hellen_meta.h"
#include "crc8hondak.h"

size_t readWhileGives(ByteSource source, uint8_t *buffer, size_t bufferSize) {
        size_t totalBytes = 0;
        while (totalBytes < bufferSize) {
           size_t readThisTime = source(&buffer[totalBytes], bufferSize - totalBytes);
           if (readThisTime == 0) {
                // looks like idle gap
                break;
           }
           totalBytes += readThisTime;
        }
    return totalBytes;
}

#ifdef EFI_KLINE

static uint8_t kvalues[8];

#define HONDA_K_BCM_STATUS_NIBBLE 0x0
#define HONDA_K_BCM_REQ_NIBBLE 0x1

bool kAcRequestState;

static void handleHonda(uint8_t *bufferIn) {

    // no headlights 0x40, with headlights 0x60
	uint8_t statusByte1 = bufferIn[1];
	// no cabin blower 0x06, with blower 0x86
	uint8_t statusByte2 = bufferIn[2];
    kAcRequestState = statusByte2 & 0x80;
    if (engineConfiguration->verboseKLine) {
        efiPrintf("honda status packet with 0x%02x 0x%02x state %d", statusByte1, statusByte2, kAcRequestState);
    }
}

static SerialDriver* const klDriver = &KLINE_SERIAL_DEVICE;
static THD_WORKING_AREA(klThreadStack, UTILITY_THREAD_STACK_SIZE);

static int totalBytes = 0;

void kLineThread(void*) {
    // due to single wire we read everything we've transmitted
    bool ignoreRecentTransmit = false;
    int sendCounter = 0;
    while (1) {

        /**
         * under the hood there is SERIAL_BUFFERS_SIZE which we hope to help us
         */

        uint8_t bufferIn[16];
        // a bit of a busy read open question if this would affect performance?
        // on 2003 Honda for instance the bus seems to be 70%-ish busy. 9600 baud is 1.04ms per byte, a bit below 1kHz
        ByteSource serialSource = [] (uint8_t * buffer, int maxSize) {
            return chnReadTimeout(klDriver,buffer, maxSize, TIME_US2I(engineConfiguration->kLinePeriodUs));
        };
        size_t len = readWhileGives(serialSource, bufferIn, sizeof(bufferIn));

                    if (engineConfiguration->verboseKLine) {
//                        efiPrintf("ignoreRecentTransmit %d", ignoreRecentTransmit);
                    }

        // to begin with just write byte to console
        if (len > 0) {
            if (engineConfiguration->verboseKLine) {
                efiPrintf("kline: got count 0x%02x", len);
            }
            for (size_t i =0;i<len;i++) {
                if (engineConfiguration->verboseKLine) {
                    efiPrintf("kline: got 0x%02x", bufferIn[i]);
                }
                totalBytes++;
            }
            if (len > 1) {
                int crc = crc_hondak_calc(bufferIn, len - 1);
                uint8_t low = bufferIn[0] & 0xF;
                if (crc == bufferIn[len - 1]) {
                    if (engineConfiguration->verboseKLine) {
                        efiPrintf("happy CRC 0x%02x", crc);
                    }
                    if (low == HONDA_K_BCM_STATUS_NIBBLE) {
                        handleHonda(bufferIn);
                    }

                    if (low == HONDA_K_BCM_REQ_NIBBLE) {
                        if (engineConfiguration->verboseKLine) {
                            efiPrintf("BCM request 0x%02x", bufferIn[0]);
                        }
                    }

                } else if (low == HONDA_K_BCM_STATUS_NIBBLE && bufferIn[4] == crc_hondak_calc(bufferIn, 4)) {
                    if (engineConfiguration->verboseKLine) {
                        efiPrintf("hack for now, happy CRC 0x%02x", crc);
                    }
                    handleHonda(bufferIn);
                }


                if (engineConfiguration->kLineDoHondaSend && !ignoreRecentTransmit) {
                    sendCounter++;
#define OUT_SIZE 6
//                	const uint8_t out2[] = {0x2, 0x0, 0x0, 0x50, 0x0, 0x0};
               	    //static_assert(sizeof(out2) == OUT_SIZE);
//                	const uint8_t outB[] = {0x42, 0x0, 0x0, 0x50, 0x0, 0x0};
//                	static_assert(sizeof(outB) == OUT_SIZE);
                	//const uint8_t *out = (sendCounter % 3 == 0) ? outB : out2;
//                	const uint8_t *out = out2;

                    if (sendCounter % 30 == 0) {
                        // no idea what this, maybe "i am running"?
                        kvalues[0] = 0x82;
                        kvalues[2] = 0x10;
                    } else {
                        kvalues[0] = 0x2;
                        kvalues[2] = 0;
                    }

                    if (engineConfiguration->verboseKLine) {
                        efiPrintf("kline doSend");
                    }
                    int positiveCltWithHighishValueInCaseOfSensorIssue = maxI(1,
#ifdef HW_HELLEN_HONDA
                        /* temporary while we are playing with calibration */
                        config->hondaKcltGaugeAdder
#else
  50
#endif
                         + Sensor::get(SensorType::Clt).value_or(140)
                    );
    // 125 about horizontal
    // 162 points at red mark, looks like gauge has hysteresis?
    // value 200 way above red mark
                    kvalues[3] = positiveCltWithHighishValueInCaseOfSensorIssue;

                    chnWrite(klDriver, (const uint8_t *)kvalues, OUT_SIZE);
                    crc = crc_hondak_calc(kvalues, OUT_SIZE);
                    chnWrite(klDriver, (const uint8_t *)&crc, 1);
                    ignoreRecentTransmit = true;
                } else {
                    ignoreRecentTransmit = false;
                }
            }
        }
    }
}
#endif // EFI_KLINE

void startKLine() {
#ifdef EFI_KLINE
    if (!engineConfiguration->enableKline) {
        return;
    }
#if EFI_PROD_CODE
	efiSetPadMode("K-Line UART RX", KLINE_SERIAL_DEVICE_RX, PAL_MODE_ALTERNATE(TS_SERIAL_AF));
	efiSetPadMode("K-Line UART TX", KLINE_SERIAL_DEVICE_TX, PAL_MODE_ALTERNATE(TS_SERIAL_AF));
#endif /* EFI_PROD_CODE */

	static SerialConfig cfg = {
		#if EFI_PROD_CODE
			.speed = 0,
			.cr1 = 0,
			.cr2 = USART_CR2_STOP1_BITS | USART_CR2_LINEN,
			.cr3 = 0
		#endif // EFI_PROD_CODE
	};

    if (engineConfiguration->kLineBaudRate < 100)
        engineConfiguration->kLineBaudRate = KLINE_BAUD_RATE;
    cfg.speed = engineConfiguration->kLineBaudRate;

	sdStart(klDriver, &cfg);
#endif // EFI_KLINE
}

void stopKLine() {
#ifdef EFI_KLINE
#if EFI_PROD_CODE
    if (activeConfiguration.enableKline) {
	    efiSetPadUnused(KLINE_SERIAL_DEVICE_RX);
	    efiSetPadUnused(KLINE_SERIAL_DEVICE_TX);

	    sdStop(klDriver);
	}

#endif /* EFI_PROD_CODE */
#endif // EFI_KLINE
}

void initKLine() {
    if (!engineConfiguration->enableKline) {
        return;
    }
#ifdef EFI_KLINE
	startKLine();

    memset(kvalues, 0, sizeof(kvalues));
    kvalues[0] = 0x2;

    chThdCreateStatic(klThreadStack, sizeof(klThreadStack), NORMALPRIO + 1, kLineThread, nullptr);
    addConsoleAction("kline", [](){
        efiPrintf("kline totalBytes %d", totalBytes);
    });
    addConsoleAction("klineyes", [](){
        engineConfiguration->kLineDoHondaSend = true;
        efiPrintf("kline send %d", engineConfiguration->kLineDoHondaSend);
    });
    addConsoleAction("klineno", [](){
        engineConfiguration->kLineDoHondaSend = false;
        efiPrintf("kline send %d", engineConfiguration->kLineDoHondaSend);
    });
    addConsoleActionII("temp_k", [](int index, int value) {
        kvalues[index] = value;
    });

#endif // EFI_KLINE
}
