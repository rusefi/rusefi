"""Characterize the existing transient UI against a generated INI."""

import argparse
import ast
import csv
import itertools
from pathlib import Path
import re
import unittest


def uncomment(line):
    quoted = False
    for index, char in enumerate(line):
        if char == '"':
            quoted = not quoted
        elif char == ';' and not quoted:
            return line[:index].strip()
    return line.strip()


def split_values(value):
    # Expressions such as bitStringValue(...) contain commas inside braces.
    parts = []
    quoted = False
    depth = 0
    start = 0
    for index, char in enumerate(value):
        if char == '"':
            quoted = not quoted
        elif not quoted:
            if char == '{':
                depth += 1
            elif char == '}':
                depth -= 1
            elif char == ',' and depth == 0:
                parts.append(value[start:index].strip())
                start = index + 1
    parts.append(value[start:].strip())
    return [next(csv.reader([part]))[0] if part.startswith('"') else part for part in parts]


class Ini:
    def __init__(self, path):
        self.sections = {}
        self.dialogs = {}
        self.gauges = {}
        self.layouts = {}
        self.readout_columns = {}
        self.curves = {}
        self.menu = []
        section = ""
        current = None
        data = Path(path).read_bytes()
        try:
            content = data.decode('utf-8-sig')
        except UnicodeDecodeError:
            # Windows Java generators use the platform's legacy INI encoding.
            content = data.decode('cp1252')
        for raw in content.splitlines():
            line = uncomment(raw)
            if not line:
                continue
            if line.startswith('[') and line.endswith(']'):
                section = line
                current = None
                continue
            self.sections.setdefault(section, []).append(line)
            if '=' not in line:
                continue
            key, value = map(str.strip, line.split('=', 1))
            values = split_values(value)
            if section == '[GaugeConfigurations]':
                self.gauges[key] = values
            if section == '[Menu]' and key in ('groupChildMenu', 'subMenu'):
                self.menu.append(values)
            if section == '[CurveEditor]':
                if key == 'curve':
                    current = values[0]
                    self.curves[current] = []
                elif current:
                    self.curves[current].append((key, values))
            if section not in ('[UserDefined]', '[UiDialogs]'):
                continue
            if key in ('dialog', 'indicatorPanel', 'readoutPanel'):
                current = values[0]
                self.dialogs[current] = []
                if key == 'dialog':
                    self.layouts[current] = values[2] if len(values) > 2 else 'yAxis'
                if key == 'readoutPanel':
                    self.readout_columns[current] = int(values[1]) if len(values) > 1 else 1
            elif key == 'help':
                current = None
            elif current:
                self.dialogs[current].append((key, values))

    def visit(self, name, config, seen=None):
        seen = set() if seen is None else seen
        if name in seen:
            return []
        seen = seen | {name}
        result = [('panel', [name])]
        for kind, values in self.dialogs.get(name, []):
            if kind == 'panel':
                expressions = [v for v in values[1:] if v.startswith('{')]
                visible = expressions[1] if len(expressions) > 1 else '{1}'
                if evaluate(visible, config):
                    result.extend(self.visit(values[0], config, seen))
            elif kind == 'field':
                visible = values[3] if len(values) > 3 else '{1}'
                if evaluate(visible, config):
                    result.append((kind, values))
            else:
                result.append((kind, values))
        return result


def evaluate(expression, config):
    expression = expression.strip('{} ').replace('&&', ' and ').replace('||', ' or ')
    expression = re.sub(r'!(?!=)', ' not ', expression).strip()
    tree = ast.parse(expression, mode='eval')
    allowed = (ast.Expression, ast.BoolOp, ast.UnaryOp, ast.Compare, ast.Name,
               ast.Load, ast.Constant, ast.And, ast.Or, ast.Not, ast.Eq,
               ast.NotEq, ast.Gt, ast.GtE, ast.Lt, ast.LtE)
    if any(not isinstance(node, allowed) for node in ast.walk(tree)):
        raise ValueError('Unsupported visibility expression: ' + expression)
    return bool(eval(compile(tree, '<INI visibility>', 'eval'), {'__builtins__': {}}, config))


def configuration(mode=0, pin=2, comp=1, wall=0, algorithm=0, ui=0, **extra):
    return dict(accelEnrichmentMode=mode, flexSensorPin=pin,
                flexFuelTransientComp=comp, complexWallModel=wall,
                fuelAlgorithm=algorithm, uiMode=ui, isInjectionEnabled=1,
                map_sensor_hwChannel=2, tpsAccelFractionPeriod=1,
                tpsAccelFractionDivisor=1, **extra)


class TransientUiTest(unittest.TestCase):
    def test_ui_contract_regressions(self):
        selectors = [v for entries in INI.dialogs.values() for k, v in entries
                     if k == 'field' and len(v) > 1 and v[1] == 'accelEnrichmentMode']
        self.assertEqual(2, len(selectors))
        flex = next(v for v in INI.menu if v[0] == 'flexAeMultDialog')
        self.assertTrue(evaluate(flex[3], configuration(pin=0)))
        table = next(v for k, v in INI.dialogs['tpsTpsDialog'] if k == 'panel' and v[0] == 'tpsTpsAccelTbl')
        self.assertEqual(3, len(table))
        self.assertEqual('wallFuelCorrection', INI.gauges['wallFuelCorrectionGauge'][0])
        self.assertEqual('mg', INI.gauges['wallFuelCorrectionGauge'][2])


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('ini', type=Path)
    ARGS = parser.parse_args()
    INI = Ini(ARGS.ini)
    unittest.main(argv=['test_transient_ui'], verbosity=2)
