"""Compile the actual HAL/device headers against the AT32 register contract."""
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
CHIBIOS = ROOT / "firmware/ChibiOS"
CC = shutil.which("arm-none-eabi-gcc")


@unittest.skipUnless(CC, "ARM compiler required")
class At32AdcDmaContractTest(unittest.TestCase):
    def compile(self, source):
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "contract.c"
            path.write_text(source)
            result = subprocess.run([
                CC, "-std=c11", "-mcpu=cortex-m4", "-mthumb", "-fsyntax-only",
                "-I" + str(CHIBIOS / "os/common/ext/ARM/CMSIS/Core/Include"),
                "-I" + str(CHIBIOS / "os/common/ext/Artery/AT32F4xx"),
                "-I" + str(CHIBIOS / "os/hal/ports/STM32/LLD/ADCv2"),
                str(path),
            ], capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)

    def test_dma_register_offsets(self):
        self.compile('''
#include <stddef.h>
#include "at32f435xx.h"
_Static_assert(offsetof(DMA_TypeDef, MUXSEL) == 0x100, "DMA mux selection");
_Static_assert(offsetof(DMA_TypeDef, MUXC) == 0x104, "DMA mux channels");
_Static_assert(offsetof(DMA_TypeDef, MUXG) == 0x120, "DMA request generators");
_Static_assert(offsetof(DMA_Channel_TypeDef, CMAR) == 0x0c, "DMA channel memory");
_Static_assert(ADC_CCR_ADCPRE == 0x000f0000, "four-bit AT32 ADC divider");
''')

    def test_adc_clock_encodings(self):
        for at32 in (False, True):
            for divisor in (2, 4, 6, 8):
                with self.subTest(at32=at32, divisor=divisor):
                    encoding = divisor - 2 if at32 else divisor // 2 - 1
                    clock = (48000000 if at32 else 24000000) // divisor
                    self.compile(("#define AT32F435xx\n" if at32 else "") + f'''
#define TRUE 1
#define FALSE 0
#define HAL_USE_ADC TRUE
#define STM32_ADC_USE_ADC1 TRUE
#define STM32_HAS_ADC1 TRUE
#define STM32_DMA_SUPPORTS_DMAMUX TRUE
#define STM32_ADC_ADCPRE ADC_CCR_ADCPRE_DIV{divisor}
#define STM32_HCLK 48000000
#define STM32_PCLK2 24000000
#include <stdint.h>
typedef struct ADCDriver ADCDriver;
#include "hal_adc_lld.h"
_Static_assert(STM32_ADC_ADCPRE == {encoding}, "ADC divider encoding");
_Static_assert(STM32_ADCCLK == {clock}, "ADC clock source and divisor");
''')


    def test_linear_and_circular_dma_sequences(self):
        # Compile the production conversion-start routine with a recording DMA
        # boundary. This exercises its register writes without emulating an MCU.
        driver = (CHIBIOS / "os/hal/ports/STM32/LLD/ADCv2/hal_adc_lld.c").read_text()
        start = driver.index("void adc_lld_start_conversion(ADCDriver *adcp) {")
        routine = driver[start:driver.index("\n}\n", start) + 3]
        irq_start = driver.index("static void adc_lld_serve_interrupt(ADCDriver *adcp, uint32_t sr) {")
        irq = driver[irq_start:driver.index("\n}\n", irq_start) + 3]
        fixture = (ROOT / "unit_tests/at32_adc/adc_dma_contract.c").read_text().replace(
            "/* PRODUCTION_SERVE_INTERRUPT */", irq)
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "contract.c"
            exe = Path(tmp) / "contract"
            for at32 in (False, True):
                with self.subTest(at32=at32):
                    path.write_text(("#define AT32F435xx\n" if at32 else "") +
                                    fixture.replace("/* PRODUCTION_START_CONVERSION */", routine))
                    result = subprocess.run(["cc", "-std=c11", "-Wall", "-Wextra", "-Werror",
                                             str(path), "-o", str(exe)], capture_output=True, text=True)
                    self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
                    subprocess.run([str(exe)], check=True, capture_output=True)



    def test_empty_fast_adc_does_not_start_timer_or_conversion(self):
        source = (ROOT / "firmware/hw_layer/adc/adc_onchip_fast.cpp").read_text()
        fixture = (ROOT / "unit_tests/at32_adc/fast_adc_empty_contract.cpp").read_text()
        for marker, declaration in (("INIT", "void AdcDevice::init(void) {"),
                                    ("START", "void AdcDevice::startConversionI()\n{")):
            start = source.index(declaration)
            routine = source[start:source.index("\n}\n", start) + 3]
            fixture = fixture.replace("/* PRODUCTION_FAST_" + marker + " */", routine)
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "contract.cpp"
            exe = Path(tmp) / "contract"
            path.write_text(fixture)
            result = subprocess.run(["c++", "-std=c++17", "-Wall", "-Wextra", "-Werror",
                                     str(path), "-o", str(exe)], capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
            subprocess.run([str(exe)], check=True)

if __name__ == "__main__":
    unittest.main()
