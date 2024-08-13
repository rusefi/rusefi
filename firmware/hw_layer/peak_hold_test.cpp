#include "pch.h"

static int phase;

static scheduling_s toggleScheduling;

static OutputPin pin;

static void pokePeak();

static void toggle(void *arg) {
	(void)arg;
  pin.toggle();
  phase++;
  if (phase < 31) {
    pokePeak();
  }
}

static void pokePeak() {
  int duration = US2NT(phase == 0 ? 200 : 30);
  efitick_t nowNt = getTimeNowNt();
  getExecutorInterface()->scheduleByTimestampNt("peakhold", &toggleScheduling, nowNt + duration, &toggle);
}

void initPeakHoldTest() {
  pin.initPin("peahold test", Gpio::B9);

  addConsoleAction("peakhold", [](){
    pin.setValue(1);
    phase = 0;
    pokePeak();
  });
}
