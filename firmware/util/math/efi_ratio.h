#pragma once

namespace efi {

template <int TNum, int TDenom = 1>
class ratio {
private:
	static constexpr int num = TNum;
	static constexpr int den = TDenom;

public:
	// A ratio type representing the reciprocal of this type.
	using recip = ratio<den, num>;

	static float asFloat() {
		return (float)num / den;
	}
};

} // namespace efi
