#include "pch.h"

#include "odometer.h"


//static odometer_state odometerCopies[2];

void setCrc(odometer_state *state) {
	state->crc = crc32(state, 4);
}

void validate(odometer_state *state) {
	int crc = crc32(state, 4);
	if (state->crc != crc)
		state->totalMeters = 0;

}


void initOdometer() {



}
