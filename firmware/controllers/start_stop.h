// file start_stop.h

#pragma once

struct StartStopState {
  ButtonDebounce startStopButtonDebounce{"start_button"};
  Timer timeSinceIgnitionPower;
 	Timer startStopStateLastPush;

  bool isFirstTime = true;

};

void initStartStopButton();
