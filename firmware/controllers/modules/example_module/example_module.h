#pragma once
// this file is important here, is the live_data struct!
#include "live_data_example_generated.h"

class ExampleModule : public EngineModule, public live_data_example_s {
public:
    void onSlowCallback() override;
};
