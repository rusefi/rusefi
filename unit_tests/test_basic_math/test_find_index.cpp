/*
 * test_find_index.cpp
 *
 *  Created on: Oct 30, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"
#include "interpolation.h"

static void testIndex(const int expected, const float array[], int size, float value) {
	ASSERT_EQ(expected, findIndex(array, size, value));
	ASSERT_EQ(expected, findIndex2(array, size, value));
}

TEST(misc, testFindIndex) {
	printf("*************************************************** testFindIndex\r\n");
/*
	{
		// in real life we only use powers of 2 for sizes
		float array5[] = { 1, 2, 3, 4, 5 };
		const int size5 = 5;
		printf("To the right5\r\n");
		testIndex(4, array5, size5, 10.0);

		printf("Middle2\r\n");
		testIndex(3, array5, size5, 4);

		printf("Middle2\r\n");
		testIndex(2, array5, size5, 3.1);

	}
*/
	{
		float array4[] = { 1, 2, 3, 4 };
		const int size4 = 4;

		printf("To the left\r\n");
		testIndex(-1, array4, size4, -1.0);

		printf("To the right4\r\n");
		testIndex(3, array4, size4, 10.0);

		printf("On the edge\r\n");
		testIndex(3, array4, size4, 4.0);

		printf("Another1\r\n");
		testIndex(2, array4, size4, 3.9);

		printf("Another2\r\n");
		testIndex(3, array4, size4, 4.1);

		printf("Another3\r\n");
		testIndex(1, array4, size4, 2);

		printf("Left edge1\r\n");
		testIndex(0, array4, size4, 1);

		printf("Left edge2\r\n");
		testIndex(0, array4, size4, 1.1);

		printf("Middle\r\n");
		testIndex(2, array4, size4, 3);
	}
}

TEST(misc, testInterpolate2d) {
	printf("*************************************************** testInterpolate2d\r\n");

	float bins4[] = { 1, 2, 3, 4 };
	float values4[] = { 1, 20, 30, 400 };

	int result;

	printf("Left size\r\n");
	result = interpolate2d(0, bins4, values4);
	ASSERT_EQ(1, result);

	printf("Right size\r\n");
	result = interpolate2d(10, bins4, values4);
	ASSERT_EQ(400, result);

	printf("Middle1\r\n");
	result = interpolate2d(3, bins4, values4);
	ASSERT_EQ(30, result);

	printf("Middle1\r\n");
	result = interpolate2d(3.5, bins4, values4);
	ASSERT_EQ(215, result);
}

TEST(misc, testSetTableValue) {
	printf("*************************************************** testSetTableValue\r\n");

	persistent_config_s config;

	for (int i = 0; i < CLT_CURVE_SIZE; i++) {
		config.cltFuelCorrBins[i] = -40 + i * 10;
		config.cltFuelCorr[i] = 1;
	}

	ASSERT_EQ(1, config.cltFuelCorr[0]);

	setCurveValue(config.cltFuelCorrBins, config.cltFuelCorr, CLT_CURVE_SIZE, -40, 1.5);
	ASSERT_FLOAT_EQ(1.5, config.cltFuelCorr[0]);

	setCurveValue(config.cltFuelCorrBins, config.cltFuelCorr, CLT_CURVE_SIZE, -50, 1.4);
	ASSERT_FLOAT_EQ(1.4, config.cltFuelCorr[0]);

}

class TestTable2dSmall : public ::testing::Test
{
protected:
	float bins[2];
	float values[2];

	void SetUp() override
	{
		// This test maps [20,30] -> [100,200]
		copyArray(bins, { 20.0f, 30.0f });
		copyArray(values, { 100.0f, 200.0f });
	}
};

TEST_F(TestTable2dSmall, OffScaleLow)
{
    EXPECT_FLOAT_EQ(interpolate2d(10, bins, values), 100);
}

TEST_F(TestTable2dSmall, OffScaleHigh)
{
    EXPECT_FLOAT_EQ(interpolate2d(40, bins, values), 200);
}

TEST_F(TestTable2dSmall, EdgeLeft)
{
    EXPECT_FLOAT_EQ(interpolate2d(20, bins, values), 100);
}

TEST_F(TestTable2dSmall, EdgeRight)
{
    EXPECT_FLOAT_EQ(interpolate2d(30, bins, values), 200);
}

TEST_F(TestTable2dSmall, Middle)
{
    EXPECT_FLOAT_EQ(interpolate2d(25, bins, values), 150);
}

TEST_F(TestTable2dSmall, NanInput)
{
    EXPECT_FLOAT_EQ(interpolate2d(NAN, bins, values), 100);
}

class Test2dTableMassive : public ::testing::Test
{
	static constexpr int Count = 2500;

protected:
	float bins[Count];
	float values[Count];

	void SetUp() override
	{
		float x = 0;

		for (size_t i = 0; i < std::size(bins); i++)
		{
			x += 0.1f;
			bins[i] = x;
			values[i] = x * x;
		}
	}
};

TEST_F(Test2dTableMassive, t)
{
    float x = 0;
    float maxErr = -1;

    for (size_t i = 0; i < 25000; i++)
    {
        x += 0.01f;

        float actual = x * x;
        float lookup = interpolate2d(x, bins, values);

        float err = std::abs(actual - lookup);

        if (err > maxErr)
        {
            maxErr = err;
        }
    }

    EXPECT_LT(maxErr, 0.01);
}

// Helper for BinResult type
#define EXPECT_BINRESULT(actual, expectedIdx, expectedFrac) \
	{ \
		auto ___temp___ = actual; \
		EXPECT_EQ(___temp___.Idx, expectedIdx); \
		EXPECT_NEAR(___temp___.Frac, expectedFrac, expectedFrac / 1e4); \
	}

// Test with small bins: only two values
static const float smallBins[] = { 10, 20 };

TEST(TableBinsSmall, OffScaleLeft)
{
    EXPECT_BINRESULT(priv::getBin(5, smallBins), 0, 0);
}

TEST(TableBinsSmall, OffScaleRight)
{
    EXPECT_BINRESULT(priv::getBin(25, smallBins), 0, 1);
}

TEST(TableBinsSmall, EdgeLeft)
{
    EXPECT_BINRESULT(priv::getBin(10, smallBins), 0, 0);
}

TEST(TableBinsSmall, EdgeRight)
{
    EXPECT_BINRESULT(priv::getBin(10, smallBins), 0, 0);
}

TEST(TableBinsSmall, Middle)
{
    EXPECT_BINRESULT(priv::getBin(15, smallBins), 0, 0.5f);
}

TEST(TableBinsSmall, NanInput)
{
	EXPECT_BINRESULT(priv::getBin(NAN, smallBins), 0, 0);
}

// Test with medium bins, 3 items
static const float bigBins[] = { 10, 20, 30 };

TEST(TableBinsBig, OffScaleLow)
{
    EXPECT_BINRESULT(priv::getBin(5, bigBins), 0, 0);
}

TEST(TableBinsBig, OffScaleHigh)
{
    EXPECT_BINRESULT(priv::getBin(35, bigBins), 1, 1.0f);
}


TEST(TableBinsBig, NearMiddleLow)
{
    EXPECT_BINRESULT(priv::getBin(19.99f, bigBins), 0, 0.999f);
}

TEST(TableBinsBig, NearMiddleExact)
{
    EXPECT_BINRESULT(priv::getBin(20.0f, bigBins), 1, 0);
}

TEST(TableBinsBig, NearMiddleHigh)
{
    EXPECT_BINRESULT(priv::getBin(20.01f, bigBins), 1, 0.001f);
}

TEST(TableBinsBig, LeftMiddle)
{
    EXPECT_BINRESULT(priv::getBin(15.0f, bigBins), 0, 0.5f);
}

TEST(TableBinsBig, RightMiddle)
{
    EXPECT_BINRESULT(priv::getBin(25.0f, bigBins), 1, 0.5f);
}
