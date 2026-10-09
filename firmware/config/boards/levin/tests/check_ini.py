"""Check the generated Levin INI against the physical connector mapping."""
from pathlib import Path
import re
import sys

OUTPUTS = {
    'B15': 'INJ 1', 'A8': 'INJ 2', 'B13': 'INJ 3', 'B14': 'INJ 4',
    'E13': 'INJ 5', 'B12': 'INJ 6', 'E7': 'INJ 7', 'E10': 'INJ 8',
    'C13': 'IGN 1', 'E6': 'IGN 2', 'E5': 'IGN 3', 'E4': 'IGN 4',
    'E3': 'IGN 5', 'E2': 'IGN 6', 'B9': 'IGN 7', 'D12': 'IGN 8',
    'E8': 'TACHO', 'E15': 'PNP PWM OUT', 'D10': 'IDLE 1', 'D9': 'IDLE 2',
    'E11': 'FUEL PUMP', 'E9': 'FAN', 'D15': 'PWM OUT 1', 'C6': 'PWM OUT 2',
    'D11': 'PWM OUT 3', 'D8': 'PWM OUT 4',
}
INPUTS = {
    'B6': 'VSS', 'D14': 'GAUX 2', 'D3': 'CRANK', 'D4': 'CAM',
    'D13': 'AUX PULSE 1', 'E14': 'GAUX 1', 'D7': 'AUX PULSE 4',
    'C7': 'AUX PULSE 3', 'B8': 'AUX PULSE 2',
}
ANALOG = {
    1: 'IAT', 2: 'CLT', 3: 'TPS', 4: 'MAP', 9: 'O2',
    11: 'ANALOG IN 2', 12: 'ANALOG IN 3', 13: 'ANALOG IN 1',
}


def check(path):
    # Inspect only ASCII syntax, regardless of the generating JVM's charset.
    text = path.read_bytes().decode('latin-1')

    def choices(name):
        match = re.search(r'^#define ' + name + r'=(.*)$', text, re.MULTILINE)
        assert match, name
        return re.findall(r'"([^"]*)"', match[1])

    def index(pin):
        return 2 + 16 * (ord(pin[0]) - ord('A')) + int(pin[1:])

    for name, mapping in (
        ('output_pin_e_list', OUTPUTS),
        ('brain_input_pin_e_list', INPUTS),
        ('switch_input_pin_e_list', INPUTS),
        ('gpio_list', {**OUTPUTS, **INPUTS}),
    ):
        values = choices(name)
        assert values[0] == 'NONE', name
        assert sum(value not in ('NONE', 'INVALID') for value in values) == len(mapping), name
        for pin, label in mapping.items():
            assert values[index(pin)] == label, (name, pin, label)
        for pin in ('A9', 'A10', 'A15', 'B10', 'B11', 'C10', 'C11', 'C12', 'D0', 'D1', 'D2'):
            assert values[index(pin)] == 'INVALID', (name, pin)

    analog = choices('adc_channel_e_list')
    assert sum(value not in ('NONE', 'INVALID') for value in analog) == len(ANALOG)
    for channel, label in ANALOG.items():
        assert analog[channel] == label, (channel, label)

    fields = set(re.findall(r'^\s*field\s*=\s*"[^"]*",\s*(\w+)', text, re.MULTILINE))
    for field in (
        'baroSensor_hwChannel', 'vbattAdcChannel', 'canRxPin', 'canTxPin',
        'sdCardCsPin', 'spi3sckPin', 'idle_stepperDirectionPin',
        'idle_stepperStepPin', 'stepperEnablePin',
    ):
        assert field not in fields, field
    for field in ('clt_bias_resistor', 'iat_bias_resistor', 'vbattDividerCoeff'):
        assert field in fields, field
    active_lines = (line for line in text.splitlines() if not line.lstrip().startswith(';'))
    assert all('@@if_' not in line for line in active_lines)
    print(f'PASS {path.name}: connector labels, enum IDs, internal pin visibility and calibration fields')


if __name__ == '__main__':
    check(Path(sys.argv[1]))
