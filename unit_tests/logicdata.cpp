/*
 * @file logicdata.cpp
 *
 * Based on LogicdataStreamFile.java by andreika
 *
 *  Created on: Jul 19, 2020
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "logicdata.h"
#include <stdio.h>
#include <stdlib.h>
#include <cstdint>
#include <string.h>

#define frequency 1000000
#define frequencyDiv 10
#define magic 0x7f

#define BLOCK 0x15
#define CHANNEL_BLOCK 0x16
#define SUB 0x54

#define title "Data save2"

#define FLAG_NOTEMPTY 2
#define FLAG_NOTEMPTY_LONG 3
#define FLAG_EMPTY 5

#define LOGIC4 0x40FD
#define LOGIC8 0x673B

#define SIGN_FLAG 0x80000000L

// todo: numChannels 7 or numChannels 8 does not work? :(
#define numChannels 6
#define reservedDurationInSamples 10

#define MAX_STRING_SIZE 40

static char channelNames[][MAX_STRING_SIZE] = { "Primary", "Secondary", "TDC",
		"Sync", "Coil", "Injector", "Channel 6", "Channel 7" };

static int CHANNEL_FLAGS[] = { 0x13458b, 0x0000ff, 0x00a0f9, 0x00ffff, 0x00ff00,
		0xff0000, 0xf020a0, };

static FILE *ptr;

static int realDurationInSamples;
static int scaledDurationInSamples;

static void writeByte(uint8_t value) {
	fwrite(&value, 1, sizeof(value), ptr);
}

static void writeAs(int64_t value, int numBytes) {
	if (value == 0) {
		writeByte(0);
	} else {
		writeByte(numBytes);
		for (int i = 0; i < numBytes; i++) {
			writeByte((uint8_t) ((value >> (i * 8)) & 0xff));
		}
	}
}

// This is the main secret of this format! :)
static void write(int64_t value) {
	if (value < 0 || value > 0xFFFFFFFFL) {
		writeAs(value, 8);
	} else if (value == 0) {
		writeByte(0);
	} else if (value <= 0xff) {
		writeAs(value, 1);
	} else if (value <= 0xffff) {
		writeAs(value, 2);
	} else if (value <= 0xffffff) {
		writeAs(value, 3);
	} else {
		writeAs(value, 4);
	}
}

static void writeString(const char *value) {
	int len = strlen(value);
	write(len);
	for (int i = 0; i < len; i++) {
		writeByte(value[i]);
	}
}

// todo: some C++ magic would allow us to drop 'count' parameter
// todo: Look at efi::size in util
static void write(int values[], int count) {
	for (int i = 0; i < count; i++) {
		write(values[i]);
	}
}

static void write(int value, int num) {
	for (int i = 0; i < num; i++)
		write(value);
}

static void writeId(int i1, int i2) {
	write((numChannels == 4) ? LOGIC4 : LOGIC8);
	write(i1);
	write(i2);
}

static void writeHeader() {
	writeByte(magic);
	write(strlen(title));
	writeString(title);

	write(BLOCK);
	write(SUB);
	write(frequency);
	write(0);
	write(reservedDurationInSamples);
	write(frequency / frequencyDiv);
	write(0, 2);
	write(numChannels);

	write(BLOCK);
	write(0);

	write(BLOCK);
	for (int i = 0; i < numChannels; i++) {
		writeId(i, 1);
	}
	write(0);

	write(BLOCK);

	writeId(0, 0);
	write(0);
	write(0);

}

static void writeDouble(double value) {
	static_assert(sizeof(double) == 8);

	if (value == 0.0) {
		writeByte(0);
	} else {
		writeByte(8);
		char *ptr = (char*) (void*) &value;

		for (int i = 0; i < 8; i++) {
			writeByte(ptr[i]);
		}
	}
}

static void writeChannelHeader(int ch) {
	write(0xff);
	write(ch);
	writeString(channelNames[ch]);
	write(0, 2);
	writeDouble(1.0);
	write(0);
	writeDouble(0.0);
	write(1);	// or 2
	writeDouble(0.0);	// or 1.0

	// this part sounds like the 'next' pointer?
	if (ch == numChannels - 1) {
		write(0);
	} else {
		writeId(1 + ch, 1);
		for (int i = 0; i < 3; i++) {
			write((CHANNEL_FLAGS[ch] >> (i * 8)) & 0xff);
		}
	}
}

static void writeEdges(int64_t *chDeltas, bool useLongDeltas, int numEdges) {
	for (int i = 0; i < numEdges; i++) {
		uint64_t d = chDeltas[i];

		// set 16-bit 'sign' flag
		if (!useLongDeltas && (d & SIGN_FLAG) == SIGN_FLAG)
			d = (d & 0x7fff) | (SIGN_FLAG >> 16);
		writeByte((uint8_t) (d & 0xff));
		writeByte((uint8_t) ((d >> 8) & 0xff));
		if (useLongDeltas) {
			writeByte((uint8_t) ((d >> 16) & 0xff));
			writeByte((uint8_t) ((d >> 24) & 0xff));
		}
	}
	writeByte(0x00);
}

static void writeRaw(int value, int num) {
	for (int i = 0; i < num; i++) {
		writeByte(value);
	}
}

static void writeChannelData(int ch, int64_t *chDeltas, int chLastState,
		int lastRecord, bool useLongDeltas, int numEdges) {
	if (numEdges == 0)
		lastRecord = 0;
	write(CHANNEL_BLOCK);
	// channel#0 is somehow special...
	if (ch == 0) {
		write(SUB);
		write(BLOCK);
	}

	write(ch + 1);
	write(0);
	write(realDurationInSamples);
	write(1);
	write(lastRecord);

	int numSamplesLeft = realDurationInSamples - lastRecord;
	write(numSamplesLeft);

	write(chLastState);

	int chFlag =
			(numEdges == 0) ?
					FLAG_EMPTY :
					(useLongDeltas ? FLAG_NOTEMPTY_LONG : FLAG_NOTEMPTY);
	write(chFlag);

	if (ch == 0) {
		write(0);
		write(BLOCK);
		write(0, 11);
		if (useLongDeltas) {
			write(BLOCK);
			write(0, 6);
		}
		write(BLOCK);
	} else {
		write(0, 10);
		if (useLongDeltas) {
			write(0, 5);
		}
	}

	write(numEdges);
	write(0);
	write(numEdges);
	write(0);
	write(numEdges);

	writeEdges(chDeltas, useLongDeltas, numEdges);

	if (ch == 0) {
		write(BLOCK);
		write(0, 6);
		if (!useLongDeltas) {
			write(BLOCK);
			write(0, 6);
		}
		write(BLOCK);
	} else {
		write(0, 4);
		if (!useLongDeltas) {
			write(0, 5);
		}
	}

	if (numEdges == 0) {
		write(0, 5);
		return;
	}

	write(1);
	write(0);
	write(1);
	write(0);
	write(1);
	write(0, 16);

	writeRaw(0xFF, 8);
	writeRaw(chFlag, 1);
	writeRaw(0x00, 7);
}

static void writeChannelDataHeader() {
	write(BLOCK);
	write(scaledDurationInSamples);
	write(0, 5);
	write(numChannels);
	write(0, 3);
	writeId(0, 1);
	write(0);

	write(BLOCK);
	write(0, 3);

	for (int i = 0; i < numChannels; i++) {
		writeChannelHeader(i);
	}

	write(BLOCK);
	int SUB_ARRAY[] = { SUB, SUB, 0, SUB, 0, SUB };
	write(SUB_ARRAY, 6);
	write(0, 6);

	write(BLOCK);
	write(0, 2);
	write(realDurationInSamples);
	write(0);
	write(SUB);
	write(reservedDurationInSamples);
	write(frequency / frequencyDiv);
	write(0, 2);
	write(SUB);
	write(0, 2);
	write(1);
	write(0, 3);
	writeId(0, 0);

	write(BLOCK);
	int SAM_ARRAY[] = { realDurationInSamples, realDurationInSamples,
			realDurationInSamples };
	write(SAM_ARRAY, 3);
	write(0);
	write(SUB);
	write(0);

	write(BLOCK);
	write(0);

	write(BLOCK);
	write(SUB, 4);
	write(0);

	write(BLOCK);
	write(frequency);
	write(0, 3);
	write(1);
	write(0, 3);
	writeId(0, 0);
	int ARR_6[] = { 0, 1, 1, 0, 1, 0x13 };
	write(ARR_6, 6);
	write(SUB);

	write(BLOCK);
	write(0);
	write(realDurationInSamples);
	write(0, 2);
	write(numChannels);
	int ARR_3[] = { 1, 0, 1 };
	write(ARR_3, 3);
}

static void writeTimingMarker() {
	write(BLOCK);
	write(numChannels + 2);
	write(0, 4);
	writeString("Timing Marker Pair");
	writeString("A1");
	writeString("A2");
	write(0, 2);
	write(SUB);
	write(0, 9);
}

static void writeChannelDataFooter() {
	write(0, 3);
	write(1);
	write(1);
	write(0);
	write(numChannels);
}

static int getChannelState(int ch, CompositeEvent *event) {
	switch (ch) {
	case 0:
		return event->primaryTrigger;
	case 1:
		return event->secondaryTrigger;
	case 2:
		return event->isTDC;
	case 3:
		return event->sync;
	case 4:
		return event->coil;
	case 5:
		return event->injector;
	}
	return -1;
}

static void writeEvents(CompositeEvent *events, int count) {
	// we need at least 2 records
	if (count < 2)
		return;
	int firstRecordTs = events[1].timestamp;
	int lastRecordTs = events[count - 1].timestamp;
	// we don't know the total duration, so we create a margin after the last record which equals to the duration of the first event
	realDurationInSamples = lastRecordTs + firstRecordTs;
	scaledDurationInSamples = realDurationInSamples / 4;

	writeChannelDataHeader();

	int64_t *chDeltas = (int64_t*) malloc(sizeof(int64_t) * count);

	bool useLongDeltas = false;
	for (int ch = 0; ch < numChannels; ch++) {
		int chPrevState = -1;
		int prevTs = 0;
		int deltaCount = 0;

		for (int i = 0; i < count; i++) {
			CompositeEvent *event = &events[i];

			int chState = getChannelState(ch, event);
			int ts = event->timestamp;

			if (chPrevState == -1) {
				chPrevState = chState;
			}
			if (chState != chPrevState) {
				long delta = ts - prevTs;
				if (delta > 0x7fff) {
					useLongDeltas = true;
				}
				// encode state
				if (chState == 0)
					delta |= SIGN_FLAG;

				chDeltas[deltaCount++] = delta;

				prevTs = ts;
				chPrevState = chState;
			}
		}
		writeChannelData(ch, chDeltas, chPrevState, prevTs, useLongDeltas,
				deltaCount);
	}

	free(chDeltas);

	writeChannelDataFooter();
}

static void writeFooter() {
	write(BLOCK);
	for (int i = 0; i < numChannels; i++) {
		writeId(i, 1);
	}
	write(1);
	writeId(numChannels, 0x15);
	for (int i = 0; i < numChannels; i++) {
		writeId(i, 1);
	}
	write(1);
	write(0);
	write(frequency);
	write(0, 16);
	write(0x01);
	write(0x23); // ???
	write(SUB);

	write(BLOCK);
	write(numChannels + 1);
	write(0);
	write(0xFFFFFFFFFFFFFFFFL);
	write(0xFFFFFFFFL);
	write(1);
	write(0, 3);

	write(BLOCK);
	write(0);

	write(BLOCK);
	write(0);
	writeDouble(1.0);
	write(SUB);
	write(0, 6);
	write(1);
	write(0, 4);

	write(1);
	write(0x29);  // ???
	write(SUB);

	writeTimingMarker();
}

void writeFile(const char * fileName, CompositeEvent *events, int count) {

	ptr = fopen(fileName, "wb");

	writeHeader();
	writeEvents(events, count);
	writeFooter();

	fclose(ptr);

}
