

#include "pch.h"
#include "trigger_central.h"

static int getZigZag(int index) {
	index = index % 1000;

	if (index < 400) {
		// going up from 0 to 400
		return index;
	}
	if (index < 500) {
		// going down from 400 to 300
		return 800 - index;
	}
	if (index < 600) {
		// going up from 300 to 400
		return index - 200;
	}
	// going down from 400 to 0
	return 1000 - index;
}

TEST(trigger, map_cam) {

	MapState state;

	int i = 0;
	for (;i<404;i++) {
		state.add(getZigZag(i));

		if (state.mapBuffer.getCount() > MAP_CAM_BUFFER) {
			ASSERT_FALSE(state.isPeak()) << "At " << i;
		}
	}

	state.add(getZigZag(i));
	ASSERT_TRUE(state.isPeak()) << "At " << i;

	for (;i<604;i++) {
		state.add(getZigZag(i));
		ASSERT_FALSE(state.isPeak()) << "At " << i;
	}

	state.add(getZigZag(i));
	ASSERT_TRUE(state.isPeak()) << "At " << i;
}
