/* libPCBUSB version A/B test: init, send the console hello on 0x710, watch
 * for the ECU's response on 0x720 and the TX counter. Usage:
 *   pcan_ver_test <path-to-libPCBUSB.dylib>
 * Enables the MacCAN trace (PCAN_TRACE_*) to a temp file so the wire-level
 * DLC of TX frames can be verified afterwards. */
#include <stdio.h>
#include <string.h>
#include <dlfcn.h>
#include <unistd.h>
#include <time.h>

typedef unsigned short WORD;
typedef unsigned int   DWORD; /* must be 32-bit: 64-bit DWORD shifts LEN -> DLC=0 on the wire */
typedef unsigned char  BYTE;

typedef struct { DWORD ID; BYTE MSGTYPE; BYTE LEN; BYTE DATA[8]; } TPCANMsg;

typedef DWORD (*Init)(WORD, DWORD, BYTE, DWORD, WORD);
typedef DWORD (*Read)(WORD, TPCANMsg*, void*);
typedef DWORD (*Write)(WORD, TPCANMsg*);
typedef DWORD (*GetValue)(WORD, BYTE, void*, DWORD);
typedef DWORD (*SetValue)(WORD, BYTE, void*, DWORD);
typedef DWORD (*Uninit)(WORD);

#define PCAN_EXT_TX_COUNTER 0x81U
#define PCAN_TRACE_LOCATION 0x11U
#define PCAN_TRACE_STATUS   0x12U
#define PCAN_TRACE_CONFIGURE 0x14U

static long nowMs(void) {
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}

int main(int argc, char** argv) {
	const char* path = argc > 1 ? argv[1] : "/usr/local/lib/libPCBUSB.dylib";
	const char* traceDir = argc > 2 ? argv[2] : "/tmp"; /* PCAN_TRACE_LOCATION is a directory */
	void* h = dlopen(path, RTLD_NOW);
	if (!h) { printf("dlopen failed: %s\n", dlerror()); return 1; }
	Init CAN_Initialize = (Init)dlsym(h, "CAN_Initialize");
	Read CAN_Read = (Read)dlsym(h, "CAN_Read");
	Write CAN_Write = (Write)dlsym(h, "CAN_Write");
	GetValue CAN_GetValue = (GetValue)dlsym(h, "CAN_GetValue");
	SetValue CAN_SetValue = (SetValue)dlsym(h, "CAN_SetValue");
	Uninit CAN_Uninitialize = (Uninit)dlsym(h, "CAN_Uninitialize");

	printf("lib: %s\n", path);
	printf("Initialize: %u\n", CAN_Initialize(0x51, 0x1C, 0, 0, 0));

	/* enable wire trace */
	DWORD one = 1, conf = 0x00000002;
	printf("SetValue TRACE_LOCATION: %u\n", CAN_SetValue(0x51, PCAN_TRACE_LOCATION, (void*)traceDir, (DWORD)strlen(traceDir) + 1));
	printf("SetValue TRACE_CONFIGURE: %u\n", CAN_SetValue(0x51, PCAN_TRACE_CONFIGURE, &conf, sizeof(conf)));
	printf("SetValue TRACE_STATUS: %u\n", CAN_SetValue(0x51, PCAN_TRACE_STATUS, &one, sizeof(one)));

	DWORD tx0 = 0, tx1 = 0;
	CAN_GetValue(0x51, PCAN_EXT_TX_COUNTER, &tx0, sizeof(tx0));

	TPCANMsg hello;
	hello.ID = 0x710;
	hello.MSGTYPE = 0x00;
	hello.LEN = 8;
	hello.DATA[0] = 0x07; hello.DATA[1] = 0x00; hello.DATA[2] = 0x01; hello.DATA[3] = 0x53;
	hello.DATA[4] = 0x20; hello.DATA[5] = 0x60; hello.DATA[6] = 0xEF; hello.DATA[7] = 0xC3;
	printf("CAN_Write(hello, LEN=8): %u\n", CAN_Write(0x51, &hello));

	TPCANMsg small;
	small.ID = 0x710;
	small.MSGTYPE = 0x00;
	small.LEN = 1;
	small.DATA[0] = 0x30; /* flow control, DLC=1 */
	printf("CAN_Write(small, LEN=1): %u\n", CAN_Write(0x51, &small));

	CAN_GetValue(0x51, PCAN_EXT_TX_COUNTER, &tx1, sizeof(tx1));
	printf("TX_COUNTER: %u -> %u\n", tx0, tx1);

	/* watch for the ECU response on 720 for 3 s */
	TPCANMsg m;
	long t0 = nowMs();
	int rx = 0, resp = 0;
	while (nowMs() - t0 < 3000) {
		DWORD st = CAN_Read(0x51, &m, NULL);
		if (st != 0) { usleep(1000); continue; }
		rx++;
		if (m.ID == 0x720) {
			resp++;
			printf("GOT 720: LEN=%u DATA:", m.LEN);
			for (int i = 0; i < m.LEN && i < 8; i++) printf(" %02X", m.DATA[i]);
			printf("\n");
		}
	}
	printf("rx=%d 720_responses=%d\n", rx, resp);

	one = 0;
	CAN_SetValue(0x51, PCAN_TRACE_STATUS, &one, sizeof(one));
	CAN_Uninitialize(0x51);
	return resp > 0 ? 0 : 2;
}
