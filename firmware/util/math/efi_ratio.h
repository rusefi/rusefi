#pragma once

namespace efi {

template <int TNum, int TDenom = 1>
struct ratio {
	static constexpr int num = TNum;
	static constexpr int den = TDenom;

	// A ratio type representing the reciprocal of this type.
	using recip = ratio<den, num>;

	static constexpr float asFloat() {
		return (float)num / den;
	}
};

} // namespace efi
