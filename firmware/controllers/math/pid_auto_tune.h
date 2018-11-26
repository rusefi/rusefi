/*
 * pid_auto_tune.h
 *
 * http://brettbeauregard.com/blog/2012/01/arduino-pid-autotune-library/
 * https://www.ripublication.com/ijeer17/ijeerv9n6_02.pdf
 *
 *
 *  Created on: Sep 13, 2017
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef CONTROLLERS_MATH_PID_AUTO_TUNE_H_
#define CONTROLLERS_MATH_PID_AUTO_TUNE_H_

#include "global.h"
#include "rusefi_types.h"

#define byte uint8_t

  // irrational constants
#define CONST_PI           3.14159265358979323846
#define CONST_SQRT2_DIV_2  0.70710678118654752440



// verbose debug option
#undef AUTOTUNE_DEBUG


// defining this option implements relay bias
// this is useful to adjust the relay output values
// during the auto tuning to recover symmetric
// oscillations
// this can compensate for load disturbance
// and equivalent signals arising from nonlinear
// or non-stationary processes
// any improvement in the tunings seems quite modest
// but sometimes unbalanced oscillations can be
// persuaded to converge where they might not
// otherwise have done so
#undef AUTOTUNE_RELAY_BIAS

// average amplitude of successive peaks must differ by no more than this proportion
// relative to half the difference between maximum and minimum of last 2 cycles
#define AUTOTUNE_PEAK_AMPLITUDE_TOLERANCE 0.05

// ratio of up/down relay step duration should differ by no more than this tolerance
// biasing the relay con give more accurate estimates of the tuning parameters but
// setting the tolerance too low will prolong the autotune procedure unnecessarily
// this parameter also sets the minimum bias in the relay as a proportion of its amplitude
#define AUTOTUNE_STEP_ASYMMETRY_TOLERANCE 0.20

// auto tune terminates if waiting too long between peaks or relay steps
// set larger value for processes with long delays or time constants
#define AUTOTUNE_MAX_WAIT_MINUTES 5

// Ziegler-Nichols type auto tune rules
// in tabular form
struct Tuning
{
  byte _divisor[3];

  bool PI_controller()
  {
    return _divisor[2] == 0;
  }

  double divisor(byte index)
  {
    return (double)(_divisor[index] * 0.05);
  }
};

#define STEPCOUNT 5

class PID_AutoTune
{

public:

  // constants ***********************************************************************************

  // auto tune method
  enum
  {
    ZIEGLER_NICHOLS_PI = 0,
    ZIEGLER_NICHOLS_PID = 1,
    TYREUS_LUYBEN_PI,
    TYREUS_LUYBEN_PID,
    CIANCONE_MARLIN_PI,
    CIANCONE_MARLIN_PID,
    AMIGOF_PI,
    PESSEN_INTEGRAL_PID,
    SOME_OVERSHOOT_PID,
    NO_OVERSHOOT_PID
  };

  // tuning rule divisor
  enum
  {
    KP_DIVISOR = 0,
    TI_DIVISOR = 1,
    TD_DIVISOR = 2
  };


  // commonly used methods ***********************************************************************
  PID_AutoTune();          // * Constructor.  links the Autotune to a given PID
  bool Runtime(Logging *logging);       // * Similar to the PID Compute function,
                                        //   returns true when done, otherwise returns false
  void Cancel();                        // * Stops the AutoTune

  void reset();

  void SetOutputStep(double);           // * how far above and below the starting value will
                                        //   the output step?
  double GetOutputStep();               //

  void SetControlType(byte);            // * Determines tuning algorithm
  byte GetControlType();                // * Returns tuning algorithm

  void SetLookbackSec(int);             // * how far back are we looking to identify peaks
  int GetLookbackSec();                 //

  void SetNoiseBand(double);            // * the autotune will ignore signal chatter smaller
                                        //   than this value
  double GetNoiseBand();                //   this should be accurately set

  float GetKp();                       // * once autotune is complete, these functions contain the
  float GetKi();                       //   computed tuning parameters.
  float GetKd();                       //

  Logging *logger;
  byte peakCount;
  float input;
  // suggested P coefficient while auto-tuning
  float output;

  void setOutput(float output);

#if EFI_UNIT_TEST
  double absMax;
  double absMin;
#endif /* EFI_UNIT_TEST */
  double outputStart;

  unsigned long sampleTime;
  byte nLookBack;

  void setState(PidAutoTune_AutoTunerState state);
  void setPeakType(PidAutoTune_Peak peakType);
  PidAutoTune_AutoTunerState state;            // * state of autotuner finite state machine

private:
  double oStep;

  double processValueOffset(double,     // * returns an estimate of the process value offset
      double);                          //   as a proportion of the amplitude

  double setpoint;

  bool running; // todo: remove this

  double noiseBand;
  byte controlType;                     // * selects autotune algorithm

  unsigned long lastTime;
  PidAutoTune_Peak peakType;
  unsigned long lastPeakTime[STEPCOUNT];        // * peak time, most recent in array element 0
  float lastPeaks[STEPCOUNT];                  // * peak value, most recent in array element 0
  float lastInputs[101];               // * process values, most recent in array element 0
  byte inputCount;
  float workingNoiseBand;
  float workingOutputstep;
  float inducedAmplitude;
  float Kp, Ti, Td;

  // used by AMIGOf tuning rule
  double calculatePhaseLag(double);     // * calculate phase lag from noiseBand and inducedAmplitude
  double fastArcTan(double);
  double newWorkingNoiseBand;
  double K_process;

#if defined AUTOTUNE_RELAY_BIAS
  double relayBias;
  unsigned long lastStepTime[5];        // * step time, most recent in array element 0
  double sumInputSinceLastStep[5];      // * integrated process values, most recent in array element 0
  byte stepCount;
#endif

};



#endif /* CONTROLLERS_MATH_PID_AUTO_TUNE_H_ */
