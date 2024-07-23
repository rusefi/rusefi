
#include "pch.h"
// todo: should we unify code with deadband.h?

template <typename T>
class StickyValue {
public:
	T get(T currentValue, T deadbandThreshold) {
		// If we're within the deadband, be "sticky" with the previous value
		T absError = absF(currentValue - m_lastValue);

		if (std::is_same<T,float>::value) {
			if (std::isnan(currentValue))
				return currentValue;
			if (std::isnan(m_lastValue))
				m_lastValue = currentValue;
		}

		// use current value if there's enough error
		if (absError > deadbandThreshold) {
			m_lastValue = currentValue;
		}

		return m_lastValue;
	}

private:
	T m_lastValue = 0;
};


TEST(Sticky, PPS) {
	StickyValue<float> pps;

	float threshold = 0.5;

	EXPECT_EQ(1, pps.get(1, threshold));
	EXPECT_EQ(1, pps.get(1.2, threshold));
	EXPECT_EQ(1, pps.get(0.8, threshold));
	EXPECT_NEAR(2.2, pps.get(2.2, threshold), EPS4D);

	float expectedNaN = pps.get(NAN, threshold);
	EXPECT_TRUE(std::isnan(expectedNaN));

	EXPECT_NEAR(33, pps.get(33, threshold), EPS4D);
}
