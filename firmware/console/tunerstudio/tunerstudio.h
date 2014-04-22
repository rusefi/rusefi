/**
 * @file	tunerstudio.h
 *
 * @date Aug 26, 2013
 * @author Andrey Belomutskiy, (c) 2012-2013
 */

#ifndef TUNERSTUDIO_H_
#define TUNERSTUDIO_H_

#if EFI_TUNER_STUDIO_OVER_USB
#define TS_SERIAL_DEVICE (&SDU1)
#else
#define TS_SERIAL_DEVICE &SD3
#define TS_SERIAL_SPEED 115200

#endif /* EFI_TUNER_STUDIO_OVER_USB */

#if defined __GNUC__
typedef struct
	__attribute__((packed)) {
#else
		typedef __packed struct {
#endif

			short int offset;
			unsigned char value;
		} TunerStudioWriteRequest;

		void startTunerStudioConnectivity(void);
		void syncTunerStudioCopy(void);
		void updateTunerStudioState(void);

#endif /* TUNERSTUDIO_H_ */
