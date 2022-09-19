#include "pch.h"
#include "can_filter.h"

TEST(CanFilterTest, acceptAny) {
	CanFilter filterAny;

	filterAny.Id = 0;
	filterAny.Mask = 0;

	ASSERT_TRUE(filterAny.accept(1));
	ASSERT_TRUE(filterAny.accept(239));
}

TEST(CanFilterTest, acceptOnlyExact) {
	CanFilter filterSpecific;

	filterSpecific.Id = 239;
	filterSpecific.Mask = FILTER_SPECIFIC;

	ASSERT_FALSE(filterSpecific.accept(1));
	ASSERT_TRUE(filterSpecific.accept(239));
}

#define CALLBACK_ALL 3
#define CALLBACK_239 5

TEST(CanFilterTest, orderOfBusinessOne) {
	resetLuaCanRx();

	// accept everything as first filter
	addLuaCanRxFilter(/*eid*/0, 0, ANY_BUS, CALLBACK_ALL);

	addLuaCanRxFilter(/*eid*/239, FILTER_SPECIFIC, ANY_BUS, CALLBACK_239);

	// filters are applied in the order in which those were added
	ASSERT_EQ(CALLBACK_ALL, getFilterForId(/*bus*/0, /*id*/ 0)->Callback);
	ASSERT_EQ(CALLBACK_ALL, getFilterForId(/*bus*/0, /*id*/ 239)->Callback);
}

TEST(CanFilterTest, orderOfBusinessTwo) {
	resetLuaCanRx();

	addLuaCanRxFilter(/*eid*/239, FILTER_SPECIFIC, ANY_BUS, CALLBACK_239);
	// accept everything as last filter
	addLuaCanRxFilter(/*eid*/0, 0, ANY_BUS, CALLBACK_ALL);

	// filters are applied in the order in which those were added
	ASSERT_EQ(CALLBACK_ALL, getFilterForId(/*bus*/0, /*id*/ 0)->Callback);
	ASSERT_EQ(CALLBACK_239, getFilterForId(/*bus*/0, /*id*/ 239)->Callback);
}
