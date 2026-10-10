#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#define TRUE 1
#define FALSE 0
#define STM32_DMA_ADVANCED 0
#define STM32_DMA_CR_CIRC (1U << 5)
#define STM32_DMA_CR_HTIE (1U << 2)
#define ADC_CR2_DDS (1U << 9)
#define ADC_CR2_DMA (1U << 8)
#define ADC_CR2_ADON 1U
#define ADC_CR2_CONT 2U
#define ADC_CR2_SWSTART (1U << 30)
#define ADC_CR1_OVRIE (1U << 26)
#define ADC_CR1_SCAN (1U << 8)
#define ADC_SR_OVR (1U << 5)
#define ADC_SR_AWD 1U
#define ADC_ACTIVE 3
#define ADC_ERR_OVERFLOW 2U
#define ADC_ERR_AWD 4U
typedef uint32_t adcerror_t;
#define ADC_SQR1_NUM_CH(n) (((n) - 1U) << 20)

typedef struct {
  uint32_t SR, SMPR1, SMPR2, HTR, LTR, SQR1, SQR2, SQR3, CR1, CR2;
} Registers;
typedef struct {
  bool circular;
  uint32_t num_channels, cr1, cr2, smpr1, smpr2, htr, ltr, sqr1, sqr2, sqr3;
} ADCConversionGroup;
typedef struct { void *memory; uint32_t count, mode; bool enabled; } Dma;
typedef struct {
  unsigned state;
  uint32_t dmamode;
  const ADCConversionGroup *grpp;
  Dma *dmastp;
  void *samples;
  size_t depth;
  Registers *adc;
} ADCDriver;
static void dmaStreamSetMemory0(Dma *d, void *p) { d->memory = p; }
static void dmaStreamSetTransactionSize(Dma *d, uint32_t n) { d->count = n; }
static void dmaStreamSetMode(Dma *d, uint32_t mode) { d->mode = mode; }
static void dmaStreamEnable(Dma *d) { d->enabled = true; }

/* PRODUCTION_START_CONVERSION */

static uint32_t errors;
#define _adc_isr_error_code(adcp, mask) ((void)(adcp), errors |= (mask))
static inline uint32_t dmaStreamGetTransactionSize(Dma *d) { return d->count; }
/* PRODUCTION_SERVE_INTERRUPT */

int main(void) {
  for (unsigned circular = 0; circular <= 1; circular++) {
    for (unsigned remaining = 0; remaining <= 1; remaining++) {
      Registers regs = {.CR1 = ADC_CR1_OVRIE};
      Dma dma = {.count = remaining};
      ADCConversionGroup group = {.circular = circular};
      ADCDriver driver = {.state = ADC_ACTIVE, .grpp = &group,
                          .dmastp = &dma, .adc = &regs};
      errors = 0;
      adc_lld_serve_interrupt(&driver, ADC_SR_OVR);
#if defined(AT32F435xx)
      if (!circular && remaining == 0) {
        assert(errors == 0 && !(regs.CR1 & ADC_CR1_OVRIE));
        assert(driver.grpp == &group && driver.state == ADC_ACTIVE);
      } else
#endif
      {
        assert(errors == ADC_ERR_OVERFLOW);
      }
    }
  }
  for (unsigned circular = 0; circular <= 1; circular++) {
    for (unsigned depth = 1; depth <= 4; depth *= 4) {
      Registers regs = {0};
      Dma dma = {0};
      uint16_t samples[12];
      ADCConversionGroup group = {.circular = circular, .num_channels = 3,
                                  .cr2 = ADC_CR2_SWSTART};
      ADCDriver driver = {.grpp = &group, .dmastp = &dma, .samples = samples,
                          .depth = depth, .adc = &regs};
      adc_lld_start_conversion(&driver);
      assert(dma.enabled && dma.memory == samples && dma.count == 3 * depth);
      assert(!!(dma.mode & STM32_DMA_CR_CIRC) == !!circular);
      assert(!!(dma.mode & STM32_DMA_CR_HTIE) == (circular && depth > 1));
      bool repeat = circular;
#ifdef AT32F435xx
      repeat |= depth > 1;
#endif
      assert(!!(regs.CR2 & ADC_CR2_DDS) == repeat);
      assert(regs.CR2 & ADC_CR2_SWSTART);
      assert(regs.CR2 & ADC_CR2_CONT);
      assert(regs.SQR1 == ADC_SQR1_NUM_CH(3));
    }
  }
}
