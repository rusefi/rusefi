/**
 * @file	knock_spectrogram.cpp
 *
 * @date Feb 20, 2024
 * @author Alexey Ershov, (c) 2023-2024
 */

#include "pch.h"

#if KNOCK_SPECTROGRAM

#include "knock_spectrogram.h"
#include <climits>

#if EFI_TEXT_LOGGING
static char PROTOCOL_KNOCK_SPECTROGRAMM_BUFFER[128] CCM_OPTIONAL;
static Logging scLogging("knock_spectrogram", PROTOCOL_KNOCK_SPECTROGRAMM_BUFFER, sizeof(PROTOCOL_KNOCK_SPECTROGRAMM_BUFFER));

static char compressToByte(const float& v, const float& min, const float& max) {
	float vn = (v-min) / (max-min);
	int iv =int((float)256 * vn);

	char compressed = (char)(iv-128);
	return compressed;
}

void base64(Logging& l, const float* data, size_t size, const float& min, const float& max) {
	static constexpr char encTable[] = {
		'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
		'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
		'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
		'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};

	size_t in_len = size;
	size_t out_len = 4 * ((in_len + 2) / 3);
	size_t i;

	l.appendChar((char)out_len);

	for (i = 0; in_len > 2 && i < in_len - 2; i += 3) {
		l.appendChar(encTable[(compressToByte(data[i], min, max) >> 2) & 0x3F]);
		l.appendChar(encTable[((compressToByte(data[i], min, max) & 0x3) << 4) |
							((int)(compressToByte(data[i + 1], min, max) & 0xF0) >> 4)]);
		l.appendChar(encTable[((compressToByte(data[i + 1], min, max) & 0xF) << 2) |
							((int)(compressToByte(data[i + 2], min, max) & 0xC0) >> 6)]);
		l.appendChar(encTable[compressToByte(data[i + 2], min, max) & 0x3F]);
	}
	if (i < in_len) {
		l.appendChar(encTable[(compressToByte(data[i], min, max) >> 2) & 0x3F]);

		if (i == (in_len - 1)) {
			l.appendChar(encTable[((compressToByte(data[i], min, max) & 0x3) << 4)]);
			l.appendChar('=');
		} else {
			l.appendChar(encTable[((compressToByte(data[i], min, max) & 0x3) << 4) |
								((int)(compressToByte(data[i + 1], min, max) & 0xF0) >> 4)]);
			l.appendChar(encTable[((compressToByte(data[i + 1], min, max) & 0xF) << 2)]);
		}

		l.appendChar('=');
	}
}
#endif /* EFI_TEXT_LOGGING */

void knockSpectorgramAddLine(float main_freq, float* data, size_t size) {
#if EFI_TEXT_LOGGING
	if (scLogging.remainingSize() > size) {

		float min = 99999999999;
		float max = -99999999999;
		for(size_t i = 0; i < size; ++i) {
			float v = data[i];
			if(v < min) {
				min = v;
			}

			if(v > max) {
				max = v;
			}
		}

		scLogging.reset();
		scLogging.appendPrintf(PROTOCOL_KNOCK_SPECTROGRAMM LOG_DELIMITER);

		scLogging.appendFloat(main_freq, 2);
		scLogging.appendPrintf("*");

		scLogging.appendChar((char)size);

		base64(scLogging, data, size, min, max);

		scLogging.append(LOG_DELIMITER);

		scheduleLogging(&scLogging);
	}

#endif /* EFI_TEXT_LOGGING */
}

#endif /* KNOCK_SPECTROGRAM */
