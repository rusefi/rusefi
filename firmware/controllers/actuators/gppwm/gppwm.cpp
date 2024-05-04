
#include "pch.h"

#include "gppwm_channel.h"

static GppwmChannel channels[GPPWM_CHANNELS];
static OutputPin pins[GPPWM_CHANNELS];
static SimplePwm outputs[GPPWM_CHANNELS];

using gppwm_Map3D_t = Map3D<GPPWM_RPM_COUNT, GPPWM_LOAD_COUNT, uint8_t, int16_t, int16_t>;

static gppwm_Map3D_t table1;
static gppwm_Map3D_t table2;
static gppwm_Map3D_t table3;
static gppwm_Map3D_t table4;

static gppwm_Map3D_t* const tables[] = {
	&table1,
	&table2,
	&table3,
	&table4,
};

static const char *channelNames[GPPWM_CHANNELS] = { "GPPWM#1",
"GPPWM#2",
"GPPWM#3",
"GPPWM#4",
 };


void initGpPwm() {
	for (size_t i = 0; i < efi::size(channels); i++) {
		auto& cfg = engineConfiguration->gppwm[i];

		// If no pin, don't enable this channel.
		if (!isBrainPinValid(cfg.pin)) {
			continue;
		}

		// Determine frequency and whether PWM is enabled
		float freq = cfg.pwmFrequency;
		bool usePwm = freq > 0;

		// Setup pin & pwm
		pins[i].initPin("gp pwm", cfg.pin);
		if (usePwm) {
			startSimplePwm(&outputs[i], channelNames[i], &engine->executor, &pins[i], freq, 0);
		}

		// Set up this channel's lookup table
		tables[i]->init(cfg.table, cfg.loadBins, cfg.rpmBins);

		// Finally configure the channel
		channels[i].init(usePwm, &outputs[i], &pins[i], tables[i], &cfg);
	}
}

void updateGppwm() {
	// There are only 8 debug float fields, this will overflow if more channels
	static_assert(efi::size(channels) <= 8);

	for (size_t i = 0; i < efi::size(channels); i++) {
		auto result = channels[i].update();

		engine->outputChannels.gppwmOutput[i] = result.Result;
		engine->outputChannels.gppwmXAxis[i] = result.X;
		engine->outputChannels.gppwmYAxis[i] = result.Y;
	}
}
