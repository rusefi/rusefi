//
// Created by kifir on 4/1/25.
//

#pragma once

class EtbTargetAdjuster {
public:
  virtual float adjustEtbTarget(float originalTarget) = 0;

  virtual ~EtbTargetAdjuster() {}
};