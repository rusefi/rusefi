#pragma once

#include <cstddef>
#include <cstdint>

namespace slcan {

// Prefix + extended ID frame + optional timestamp + CR + NUL.
constexpr size_t FrameBufferSize = 1 + 1 + 8 + 1 + 16 + 4 + 1 + 1;

// Elmue CANable channel convention: CAN1 has no prefix, CAN2 '&', CAN3 '$'.
// Keeping this formatter independent of the USB driver lets host tests exercise
// the exact bytes emitted by firmware, including RTR and maximum-length frames.
inline size_t formatFrame(char (&buffer)[FrameBufferSize], size_t busIndex, bool includeBus,
	uint32_t id, bool extended, bool rtr, uint8_t dlc, const uint8_t* data,
	bool timestampEnabled, uint16_t timestamp) {
	buffer[0] = '\0';
	if (dlc > 8 || (includeBus && busIndex > 2)) {
		return 0;
	}

	char* out = buffer;
	auto hex = [&](uint32_t value, int digits) {
		for (int shift = (digits - 1) * 4; shift >= 0; shift -= 4) {
			*out++ = "0123456789ABCDEF"[(value >> shift) & 0xf];
		}
	};
	if (includeBus && busIndex != 0) {
		*out++ = busIndex == 1 ? '&' : '$';
	}
	*out++ = extended ? (rtr ? 'R' : 'T') : (rtr ? 'r' : 't');
	hex(id, extended ? 8 : 3);
	hex(dlc, 1);
	if (!rtr) {
		for (uint8_t i = 0; i < dlc; i++) {
			hex(data[i], 2);
		}
	}
	if (timestampEnabled) {
		hex(timestamp, 4);
	}
	*out++ = '\r';
	*out = '\0';
	return out - buffer;
}

} // namespace slcan
