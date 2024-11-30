#pragma once

struct LimpState {
  int ecuRestartCounter = 0;
  void save() {
    ecuRestartCounter++;
  }
  void load();
};

