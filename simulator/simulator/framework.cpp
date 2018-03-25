/**
 * @file	framework.cpp
 *
 * @date Sep 25, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "main.h"
#include "framework.h"

efitick_t getTimeNowNt(void) {
	return getTimeNowUs() * US_TO_NT_MULTIPLIER;
}

efitimeus_t getTimeNowUs(void) {
	return chVTGetSystemTimeX() * (1000000 / CH_CFG_ST_FREQUENCY);
}

// todo; reduce code duplication with prod code?
efitimems_t currentTimeMillis(void) {
	return chVTGetSystemTimeX() / 1000;
}

// todo; reduce code duplication with prod code?
efitimesec_t getTimeNowSeconds(void) {
	return currentTimeMillis() / CH_CFG_ST_FREQUENCY;
}

static size_t wt_writes(void *ip, const uint8_t *bp, size_t n) {
	printToWin32Console((char*)bp);
	return CONSOLE_PORT->vmt->write(CONSOLE_PORT, bp, n);
}

static size_t wt_reads(void *ip, uint8_t *bp, size_t n) {
	return CONSOLE_PORT->vmt->read(CONSOLE_PORT, bp, n);
}

static msg_t wt_putt(void *instance, uint8_t b, systime_t time) {
	return CONSOLE_PORT->vmt->putt(CONSOLE_PORT, b, time);
}

static msg_t wt_gett(void *instance, systime_t time) {
	return CONSOLE_PORT->vmt->gett(CONSOLE_PORT, time);
}

static size_t wt_writet(void *instance, const uint8_t *bp,
                size_t n, systime_t time) {
	return CONSOLE_PORT->vmt->writet(CONSOLE_PORT, bp, n, time);
}

static size_t wt_readt(void *instance, uint8_t *bp, size_t n, systime_t time) {
	return CONSOLE_PORT->vmt->readt(CONSOLE_PORT, bp, n, time);
}

static char putMessageBuffer[2];

static msg_t wt_put(void *ip, uint8_t b) {
	putMessageBuffer[0] = b;
	putMessageBuffer[1] = 0;
	printToWin32Console((char*)putMessageBuffer);
//	cputs("wt_put");
	return CONSOLE_PORT->vmt->put(CONSOLE_PORT, b);
}

static msg_t wt_get(void *ip) {
//	cputs("wt_get");
	//return 0;
	return CONSOLE_PORT->vmt->get(CONSOLE_PORT);
}

static const struct Win32TestStreamVMT vmt = { wt_writes, wt_reads, wt_put, wt_get, wt_putt, wt_gett, wt_writet, wt_readt };

void initTestStream(TestStream *ts) {
	ts->vmt = &vmt;
}
