#include <cassert>
#include <cstdint>
#define EFI_INTERNAL_FAST_ADC_GPT (&timer)
#define GPT_PERIOD_FAST 100
#define ADC_READY 2
#define ADC_ERROR 5
#define ADC_FAST_DEVICE device
using adcsample_t = uint16_t;
struct Driver { int state = ADC_READY; } device;
struct Config { unsigned num_channels = 0; };
static unsigned timerStarts, conversions, lockDepth;
static int timer, fast_adc_config;
struct { struct { unsigned fastAdcErrorCount = 0; } outputChannels; } instance;
static auto* engine = &instance;
static void gptStart(int*, int*) { timerStarts++; }
static void gptStartContinuous(int*, int) { timerStarts++; }
static void chSysLockFromISR() { lockDepth++; }
static void chSysUnlockFromISR() { lockDepth--; }
static void adcStartConversionI(Driver*, Config* c, adcsample_t*, unsigned depth) {
    assert(lockDepth && c->num_channels > 0 && depth == 4);
    conversions++;
}
class AdcDevice {
public:
    Config config;
    Config* hwConfig = &config;
    Driver* adcp = &device;
    adcsample_t samples[16]{};
    unsigned depth = 4, channels;
    explicit AdcDevice(unsigned n) : channels(n) {}
    unsigned size() { return channels; }
    void init();
    void startConversionI();
};
/* PRODUCTION_FAST_INIT */
/* PRODUCTION_FAST_START */
int main() {
    AdcDevice empty(0);
    empty.init();
    empty.startConversionI();
    assert(timerStarts == 0 && conversions == 0 && lockDepth == 0);
    AdcDevice populated(2);
    populated.init();
    populated.startConversionI();
    assert(timerStarts == 2 && conversions == 1 && lockDepth == 0);
    device.state = 3; // Conversion still active: retain existing error handling.
    populated.startConversionI();
    assert(conversions == 1 && engine->outputChannels.fastAdcErrorCount == 1);
}
