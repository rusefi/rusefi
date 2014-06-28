/**
 * @file	tunerstudio.h
 *
 * @date Aug 26, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef TUNERSTUDIO_H_
#define TUNERSTUDIO_H_

#include "tunerstudio_configuration.h"

#if defined __GNUC__
typedef struct
	__attribute__((packed)) {
#else
		typedef __packed struct {
#endif
			short int offset;
			short int count;

		} TunerStudioWriteChunkRequest;

#if defined __GNUC__
typedef struct
	__attribute__((packed)) {
#else
		typedef __packed struct {
#endif

			short int offset;
			unsigned char value;
		} TunerStudioWriteValueRequest;

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

#endif /* TUNERSTUDIO_H_ */
