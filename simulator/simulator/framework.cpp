/**
 * @file	framework.cpp
 *
 * @date Sep 25, 2014
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "global.h"
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
	printToConsole((char*)bp);
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
	printToConsole((char*)putMessageBuffer);
//	cputs("wt_put");
	return CONSOLE_PORT->vmt->put(CONSOLE_PORT, b);
}

static msg_t wt_get(void *ip) {
//	cputs("wt_get");
	//return 0;
	return CONSOLE_PORT->vmt->get(CONSOLE_PORT);
}

static msg_t _ctl(void *ip, unsigned int operation, void *arg) {

  (void)ip;
  (void)operation;
  (void)arg;

  return MSG_OK;
}

/**
 * These implementation print same content on console screen and send data over the underlying CONSOLE_PORT
 * this is useful to see what's going on.
 * See #wt_get() as a typical implementation
 */
static const struct BaseChannelVMT vmt = {
		  (size_t)0,
		wt_writes, wt_reads, wt_put, wt_get,
		wt_putt, wt_gett, wt_writet, wt_readt,
		  _ctl
};

void initTestStream(BaseChannel *ts) {
	ts->vmt = &vmt;
}
