/**
 * @file	tunerstudio.h
 *
 * @date Aug 26, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef TUNERSTUDIO_H_
#define TUNERSTUDIO_H_

#include "tunerstudio_configuration.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void startTunerStudioConnectivity(void);
void syncTunerStudioCopy(void);
void updateTunerStudioState(TunerStudioOutputChannels *tsOutputChannels);
void tunerStudioWriteCrcPacket(const uint8_t command, const void *buf, const uint16_t size);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#if defined __GNUC__
// GCC
#define pre_packed
#define post_packed __attribute__((packed))
#else
// IAR
#define pre_packed __packed
#define post_packed
#endif

typedef pre_packed struct
	post_packed {
		short int offset;
		short int count;
	} TunerStudioWriteChunkRequest;

	typedef pre_packed struct
		post_packed {
			short int page;
			short int offset;
			short int count;
		} TunerStudioReadRequest;

		typedef pre_packed struct
			post_packed {
				short int offset;
				unsigned char value;
			} TunerStudioWriteValueRequest;

#endif /* TUNERSTUDIO_H_ */
