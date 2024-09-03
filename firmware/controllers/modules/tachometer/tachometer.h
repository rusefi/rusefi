/*
 * @file tachometer.h
 *
 * @date Aug 18, 2015
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

void initTachometer();

struct TachometerModule : public EngineModule {

	// TODO: can/should this be slow callback instead?
	void onFastCallback() override;
};
