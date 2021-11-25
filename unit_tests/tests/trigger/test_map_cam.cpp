

#include "pch.h"

#define MAP_CAM_BUFFER 8

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

struct MapState {
	float current, previous, prevPrevious;
	cyclic_buffer<float, MAP_CAM_BUFFER> mapBuffer;

	void add(float value) {
		// rotate state
		prevPrevious = previous;
		previous = current;

		// add new value
		mapBuffer.add(value);
		current = mapBuffer.sum(MAP_CAM_BUFFER);
	}

	bool isPeak() {
		return previous > prevPrevious && previous >= current;
	}
};

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
