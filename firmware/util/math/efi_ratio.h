#pragma once

namespace efi {

template <int TNum, int TDenom = 1>
class ratio {
private:
	static constexpr int num = TNum;
	static constexpr int den = TDenom;

public:
	static float asFloat() {
		return (float)num / den;
	}
};

} // namespace efi
