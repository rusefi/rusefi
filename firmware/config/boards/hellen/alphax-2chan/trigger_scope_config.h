#include "hellen_meta.h"

#define TRIGGER_SCOPE_ADC ADCD3

#define TRIGGER_SCOPE_SAMPLE_TIME ADC_SAMPLE_144

#define TRIGGER_SCOPE_PIN_CH1 H144_IN_RES3
#define TRIGGER_SCOPE_ADC_CH1 H144_ADC_IN_RES3

#define TRIGGER_SCOPE_HAS_CH2 false
// todo: fix https://github.com/rusefi/rusefi/issues/4909
#define TRIGGER_SCOPE_ADC_CH2 H144_ADC_IN_RES2
