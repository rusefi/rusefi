"""Exercise the actual generated INI, including its mode/sensor visibility.

Run --baseline before changing the templates to characterize the old UI.
Afterwards, run without --baseline and pass --against the baseline INI to
verify that all stored calibration definitions and ECU output channels match.
Only Python's standard library is required.
"""

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
    def setUp(self):
        if ARGS.layout_baseline and self._testMethodName != 'test_compact_layout_and_separate_map_screen':
            self.skipTest('Layout reproduction checkpoint')
        if ARGS.wall_baseline and self._testMethodName != 'test_wall_advanced_has_four_curves_without_an_analog_map_pin':
            self.skipTest('Wall MAP-pin reproduction checkpoint')

    def test_wall_advanced_has_four_curves_without_an_analog_map_pin(self):
        if ARGS.baseline:
            self.skipTest('Final UI expectations')
        curves = {'wwTauCltCurve', 'wwTauMapCurve', 'wwBetaCltCurve', 'wwBetaMapCurve'}
        for analog_pin in (0, 2):
            for wall in (0, 1):
                config = configuration(wall=wall)
                config['map_sensor_hwChannel'] = analog_pin
                entries = INI.visit('wallWettingDialog', config)
                visible = {v[0] for k, v in entries if k == 'panel'} & curves
                if ARGS.wall_baseline and wall and not analog_pin:
                    # The ECU checks Sensor::hasSensor(Map), not the ADC pin.
                    self.assertEqual({'wwTauCltCurve', 'wwBetaCltCurve'}, visible)
                else:
                    self.assertEqual(curves if wall else set(), visible)

    def test_compact_layout_and_separate_map_screen(self):
        if ARGS.baseline:
            self.skipTest('Final UI expectations')
        settings = INI.visit('AccelEnrich', configuration())
        if ARGS.layout_baseline:
            # Passing reproduction before changing the layout: five stacked
            # circular gauges force the height of both East/West columns.
            self.assertEqual(5, sum(k == 'gauge' for k, v in settings))
            self.assertIn(('panel', ['aeSettingsMonitor', 'East']), INI.dialogs['AccelEnrich'])
            calibration = INI.visit('tpsTpsDialog', configuration())
            self.assertIn(('panel', ['aeMapFallbackCalibration']), calibration)
            self.assertNotIn('mapPredictionDialog', [v[0] for v in INI.menu])
            return
        # [tag:transient_compact_layout] North keeps field/readout rows at
        # their preferred height even when the user enlarges the dialog.
        wrappers = dict(AccelEnrich='aeSettingsContent',
                        tpsTpsDialog='aeTpsCalibrationContent',
                        mapPredictionDialog='aeMapCalibrationContent',
                        flexTransientDialog='aeFlexContent',
                        wallWettingDialog='aeWallHeader')
        for root, header in wrappers.items():
            self.assertEqual('border', INI.layouts[root])
            self.assertTrue(any(k == 'panel' and v[:2] == [header, 'North']
                                for k, v in INI.dialogs[root]), root)
        for mode, wall in itertools.product(range(4), (0, 1)):
            config = configuration(mode=mode, wall=wall)
            for root in wrappers:
                entries = INI.visit(root, config)
                self.assertFalse(any(k == 'gauge' for k, v in entries), root)
                for kind, values in entries:
                    if kind == 'readout':
                        self.assertIn(values[0], INI.gauges)
            tps = INI.visit('tpsTpsDialog', config)
            self.assertFalse(any(k == 'panel' and v[0] in (
                'mapEstimateTableTbl', 'predictiveMapBlendDuration',
                'aeMapFallbackCalibration') for k, v in tps))
        self.assertEqual(3, INI.readout_columns['aeInputReadouts'])
        # One six-value readout panel per calibration mode keeps the two
        # rows aligned, including units that differ between MS and Percent.
        for mode, panel in ((0, 'aeMsCalibrationReadouts'),
                            (1, 'aeFractionCalibrationReadouts'), (3, 'aeLuaReadouts')):
            self.assertEqual(3, INI.readout_columns[panel])
            entries = INI.visit('tpsTpsDialog', configuration(mode=mode))
            readouts = [v[0] for k, v in entries if k == 'readout']
            self.assertEqual(6, len(readouts))
            self.assertEqual(6, len(set(readouts)))
        self.assertEqual(3, INI.readout_columns['aeTpsEventReadouts'])
        self.assertEqual(2, INI.readout_columns['aeWallResultReadouts'])
        self.assertEqual(2, INI.readout_columns['aeFlexFilmMonitor'])
        for curve in ('tpsTspRpmCorrection', 'predictiveMapBlendDuration',
                      'wwTauCltCurve', 'wwTauMapCurve', 'wwBetaCltCurve', 'wwBetaMapCurve'):
            self.assertFalse(any(k == 'gauge' for k, v in INI.curves[curve]), curve)
            self.assertIn(('size', ['480', '220']), INI.curves[curve])
        for label in ('Pause STFT learning after acceleration', 'TPS history window'):
            self.assertTrue(any(k == 'field' and v[0] == label for k, v in settings))

    def test_ui_contract_regressions(self):
        selectors = [v for entries in INI.dialogs.values() for k, v in entries
                     if k == 'field' and len(v) > 1 and v[1] == 'accelEnrichmentMode']
        if ARGS.baseline:
            self.assertEqual(2, len(selectors))
            flex = next(v for v in INI.menu if v[0] == 'flexAeMultDialog')
            self.assertTrue(evaluate(flex[3], configuration(pin=0)))
            table = next(v for k, v in INI.dialogs['tpsTpsDialog'] if k == 'panel' and v[0] == 'tpsTpsAccelTbl')
            self.assertEqual(3, len(table))  # Only enable, no visibility condition.
            self.assertEqual('wallFuelCorrection', INI.gauges['wallFuelCorrectionGauge'][0])
        else:
            self.assertEqual(1, len(selectors))
            flex = next(v for v in INI.menu if v[0] == 'flexTransientDialog')
            self.assertFalse(evaluate(flex[3], configuration(pin=0)))
            for mode in range(4):
                tables = [v[0] for k, v in INI.visit('tpsTpsDialog', configuration(mode=mode))
                          if k == 'panel' and v[0] == 'tpsTpsAccelTbl']
                self.assertEqual(['tpsTpsAccelTbl'] if mode < 2 else [], tables)
            self.assertEqual('wallFuelCorrectionValue', INI.gauges['wallFuelCorrectionGauge'][0])
        self.assertEqual('mg', INI.gauges['wallFuelCorrectionGauge'][2])

    def test_single_selectors_and_five_menu_entries(self):
        if ARGS.baseline:
            self.skipTest('Final UI expectations')
        for field in ('accelEnrichmentMode', 'flexFuelTransientComp'):
            selectors = [v for entries in INI.dialogs.values() for k, v in entries
                         if k == 'field' and len(v) > 1 and v[1] == field]
            self.assertEqual(1, len(selectors), field)
        keys = [v[0] for v in INI.menu]
        for key in ('AccelEnrich', 'tpsTpsDialog', 'mapPredictionDialog', 'wallWettingDialog', 'flexTransientDialog'):
            self.assertEqual(1, keys.count(key), key)
        for key in ('mapEstimateTableTbl', 'predictiveMapBlendDuration', 'tpsTspRpmCorrection', 'flexAeMultDialog'):
            self.assertNotIn(key, keys)

    def test_mode_sensor_and_wall_matrix(self):
        if ARGS.baseline:
            self.skipTest('Final UI expectations')
        for mode, pin, comp, wall in itertools.product(range(4), (0, 2), (0, 1), (0, 1)):
            with self.subTest(mode=mode, pin=pin, comp=comp, wall=wall):
                config = configuration(mode, pin, comp, wall)
                settings = INI.visit('AccelEnrich', config)
                calibration = INI.visit('tpsTpsDialog', config)
                prediction = INI.visit('mapPredictionDialog', config)
                film = INI.visit('wallWettingDialog', config)
                flex = INI.visit('flexTransientDialog', config)
                panels = {v[0] for k, v in calibration if k == 'panel'}
                fields = {v[1] for k, v in settings if k == 'field' and len(v) > 1}
                flex_panels = {v[0] for k, v in flex if k == 'panel'}
                film_panels = {v[0] for k, v in film if k == 'panel'}
                self.assertEqual(mode < 2, 'tpsTpsAccelTbl' in panels)
                self.assertEqual(mode < 2, 'tpsTspRpmCorrection' in panels)
                self.assertEqual(mode != 3, 'tpsAccelEnrichmentThreshold' in fields)
                self.assertEqual(mode < 2, 'tpsAccelLookback' in fields)
                self.assertEqual(mode < 2, 'tpsAccelFractionPeriod' in fields)
                self.assertNotIn('aePredictiveHistoryInfo',
                                 {v[0] for k, v in settings if k == 'panel'})
                prediction_panels = {v[0] for k, v in prediction if k == 'panel'}
                self.assertEqual(mode == 2, 'aePredictiveHistoryInfo' in prediction_panels)
                self.assertNotIn('predictiveMapBlendDuration', panels)
                self.assertNotIn('mapEstimateTableTbl', panels)
                self.assertEqual(mode == 2, 'predictiveMapBlendDuration' in prediction_panels)
                self.assertEqual(1, sum(k == 'panel' and v[0] == 'mapEstimateTableTbl' for k, v in prediction))
                self.assertEqual(bool(pin and comp and mode < 2), 'flexAeMultTbl' in flex_panels)
                self.assertEqual(bool(pin and comp), 'flexWwTauMultTbl' in flex_panels)
                self.assertEqual(bool(pin and comp), 'flexWwBetaMultTbl' in flex_panels)
                self.assertEqual(bool(wall), 'wwTauCltCurve' in film_panels)
                self.assertEqual(bool(wall), 'wwBetaCltCurve' in film_panels)
                gauges = {v[0] for k, v in calibration + prediction if k in ('gauge', 'readout')}
                self.assertEqual(mode == 0, 'transientAeMsGauge' in gauges)
                self.assertEqual(mode == 1, 'transientAeFractionGauge' in gauges)
                self.assertEqual(mode == 2, 'transientEffectiveMapGauge' in gauges)
                self.assertEqual(mode == 3, 'transientLuaMassGauge' in gauges)
                forbidden = ('stft', 'lambda', 'afr')
                for kind, values in settings + calibration + prediction + flex:
                    if kind in ('gauge', 'readout', 'graphLine'):
                        channel = INI.gauges[values[0]][0] if kind in ('gauge', 'readout') else values[0]
                        self.assertFalse(any(s in (values[0] + channel).lower() for s in forbidden), values[0])
                mixture = [v[0] for k, v in film if k == 'readout'
                           and INI.gauges[v[0]][0] == 'selectedAfrLambda1']
                self.assertEqual(['transientMeasuredMixtureGauge'], mixture)
                for kind, values in film:
                    if kind in ('gauge', 'readout', 'graphLine'):
                        channel = INI.gauges[values[0]][0] if kind in ('gauge', 'readout') else values[0]
                        self.assertFalse(any(s in channel.lower() for s in ('stft', 'targetlambda', 'targetafr')))
                for screen in (settings, film, calibration if mode != 2 else prediction):
                    self.assertTrue(any(k == 'readout' and v[0] == 'transientRpmAccelerationGauge'
                                        for k, v in screen))
                for screen in (film, calibration if mode != 2 else prediction):
                    self.assertTrue(any(k == 'graphLine' and v[0] == 'rpmAcceleration'
                                        for k, v in screen))
                self.assertFalse(any('stftLearningState' in ' '.join(v) for k, v in settings if k == 'indicator'))

    def test_flex_menu_offline_and_fault_visibility(self):
        if ARGS.baseline:
            self.skipTest('Final UI expectations')
        menu = next(v for v in INI.menu if v[0] == 'flexTransientDialog')
        for pin in (0, 1, 2):
            for percent, error in ((0, 0), (0, 1), (85, 0)):
                config = configuration(pin=pin, comp=0, flexPercent=percent, isFlexError=error)
                self.assertEqual(pin > 1, evaluate(menu[3], config))
                fields = {v[1] for k, v in INI.visit('flexTransientDialog', config) if k == 'field' and len(v) > 1}
                self.assertEqual(pin > 1, 'flexFuelTransientComp' in fields)
        for key in ('AccelEnrich', 'tpsTpsDialog', 'mapPredictionDialog', 'wallWettingDialog', 'flexTransientDialog'):
            item = next(v for v in INI.menu if v[0] == key)
            config = configuration()
            config['isInjectionEnabled'] = 0
            self.assertFalse(evaluate(item[3], config), key)

    def test_map_fallback_and_incompatible_algorithm(self):
        if ARGS.baseline:
            self.skipTest('Final UI expectations')
        for mode, algorithm in itertools.product(range(4), (0, 1, 2)):
            config = configuration(mode=mode, algorithm=algorithm)
            panels = {v[0] for k, v in INI.visit('mapPredictionDialog', config) if k == 'panel'}
            self.assertEqual(algorithm == 0, 'mapEstimateTableTbl' in panels)
            self.assertEqual(algorithm == 0 and mode == 2, 'predictiveMapBlendDuration' in panels)
            self.assertEqual(algorithm == 0 and mode == 2, 'aePredictiveHistoryInfo' in panels)
            self.assertEqual(algorithm != 0 and mode == 2, 'aeMapIncompatibleInfo' in panels)
            menu = next(v for v in INI.menu if v[0] == 'mapPredictionDialog')
            self.assertEqual(mode == 2 or algorithm == 0, evaluate(menu[3], config))
            menu = next(v for v in INI.menu if v[0] == 'tpsTpsDialog')
            self.assertEqual(mode != 2, evaluate(menu[3], config))
        for ui in (0, 1, 2):
            config = configuration(ui=ui)
            panels = {v[0] for k, v in INI.visit('mapPredictionDialog', config) if k == 'panel'}
            self.assertEqual(ui in (0, 1), 'mapEstimateTableTbl' in panels)
            self.assertEqual(ui in (0, 1), evaluate(next(v for v in INI.menu if v[0] == 'mapPredictionDialog')[3], config))

    def test_units_and_calibration_compatibility(self):
        if ARGS.baseline:
            self.skipTest('Final UI expectations')
        self.assertEqual('wallFuelCorrectionValue', INI.gauges['wallFuelCorrectionGauge'][0])
        self.assertEqual('mg', INI.gauges['wallFuelCorrectionGauge'][2])
        self.assertEqual('coolant', INI.gauges['transientCltGauge'][0])
        rpm_rate = INI.gauges['transientRpmAccelerationGauge']
        self.assertEqual('rpmAcceleration', rpm_rate[0])
        self.assertEqual('RPM/s', rpm_rate[2])
        self.assertLess(float(rpm_rate[3]), 0)
        self.assertGreater(float(rpm_rate[4]), 0)
        mixture = INI.gauges['transientMeasuredMixtureGauge']
        self.assertEqual('selectedAfrLambda1', mixture[0])
        self.assertIn('useLambdaOnInterface', mixture[2])
        self.assertIn('selectedAfrLambda1 = { useLambdaOnInterface ? lambdaValue : AFRValue }',
                      INI.sections['[OutputChannels]'])
        for gauge in ('transientMeasuredMapGauge', 'transientEstimatedMapGauge', 'transientEffectiveMapGauge'):
            self.assertEqual('kPa', INI.gauges[gauge][2])
        if ARGS.against:
            baseline = Ini(ARGS.against)
            for section in ('[Constants]',):
                before = [line for line in baseline.sections[section] if '=' in line]
                after = [line for line in INI.sections[section] if '=' in line]
                self.assertEqual(before, after, 'Stored calibration definition changed')
            scalars = lambda model: {line.split('=', 1)[0].strip(): line for line in model.sections['[OutputChannels]']
                                     if re.search(r'=\s*(scalar|bits),', line)}
            self.assertEqual(scalars(baseline), scalars(INI), 'ECU binary telemetry changed')

    def test_fractional_monitor_only_in_applicable_advanced_modes(self):
        if ARGS.baseline:
            self.skipTest('Final UI expectations')
        for mode, ui in itertools.product(range(4), (0, 1, 2)):
            config = configuration(mode=mode, ui=ui)
            config['tpsAccelFractionPeriod'] = 3
            entries = INI.visit('tpsTpsDialog', config)
            panels = {v[0] for k, v in entries if k == 'panel'}
            self.assertEqual(mode < 2 and ui in (0, 1), 'aePumpMonitor' in panels)
            if 'aePumpMonitor' in panels:
                self.assertIn('aeFractionalIndicator', panels)
                self.assertTrue(any(k == 'runtimeValue' and v[1] == 'cycleCnt' for k, v in entries))


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('ini', type=Path)
    parser.add_argument('--baseline', action='store_true')
    parser.add_argument('--layout-baseline', action='store_true')
    parser.add_argument('--wall-baseline', action='store_true')
    parser.add_argument('--against', type=Path)
    ARGS = parser.parse_args()
    INI = Ini(ARGS.ini)
    unittest.main(argv=['test_transient_ui'], verbosity=2)
