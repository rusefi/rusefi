/**
 * @file    alternator_controller.h
 * @brief   alternator controller
 *
 * @date Apr 6, 2014
 * @author Dmitry Sidin
 * @author Andrey Belomutskiy, (c) 2012-2020
 *
 */

#pragma once

void initAlternatorCtrl();

class AlternatorController : public EngineModule {
public:
	void onFastCallback() override;
	void onConfigurationChange(engine_configuration_s const* previousConfiguration) override;
};
