#include "pch.h"
#include "biquad.h"

#pragma once

struct LuaBiQuad {
  Biquad delegate;

  float filter(float input) {
    if (isFirstValue) {
      delegate.cookSteadyState(input);
      isFirstValue = false;
    }
    return delegate.filter(input);
  }

  void configureLowpass(float samplingFrequency, float cutoffFrequency) {
    delegate.configureLowpass(samplingFrequency, cutoffFrequency);
  }

  bool isFirstValue = true;
};
