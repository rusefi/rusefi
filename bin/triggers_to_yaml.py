#!/usr/bin/env python3
"""Convert unit_tests/triggers.txt into YAML format."""

import re
import os
import math

def parse_triggers_txt(filepath):
    """Parse the triggers.txt file and return structured data."""

    with open(filepath, 'r') as f:
        lines = f.readlines()

    triggers = []
    current_trigger = None

    for line in lines:
        line = line.strip()

        if not line or line.startswith('#'):
            continue

        triggertype_match = re.match(r'TRIGGERTYPE\s+\d+\s+(\d+)\s+(\S+)\s+([\d.+-]+)', line)
        if triggertype_match:
            if current_trigger is not None:
                finish_trigger(current_trigger)
                triggers.append(current_trigger)

            length, name, offset = triggertype_match.groups()
            current_trigger = {
                'name': name,
                'length': int(length),
                'offset': float(offset),
                'metadata': {},
                'events': [],
                'wrappedFall': {},
                'gaps': [],
                'eventCount': {"0": 0, "1": 0}
            }
            continue

        if current_trigger is None:
            continue

        meta_match = re.match(r'^(\w+)=(.+)$', line)
        if meta_match and current_trigger is not None:
            key, value = meta_match.groups()

            if value.lower() in ('true', 'false'):
                current_trigger['metadata'][key] = value.lower() == 'true'
            elif value.isdigit():
                current_trigger['metadata'][key] = int(value)
            else:
                try:
                    current_trigger['metadata'][key] = float(value)
                except ValueError:
                    current_trigger['metadata'][key] = value
            continue

        gaps_match = re.match(r'^(\w+)\.([0-9]+)=(.+)$', line)
        if gaps_match and current_trigger is not None:
            key, idxs, value = gaps_match.groups()

            idx = int(idxs)

            targetKey = ""
            if key == "gapFrom":
                targetKey = "from"
            else:
                targetKey = "to"

            if idx >= len(current_trigger['gaps']):
                current_trigger['gaps'].insert(idx, {})
            current_trigger['gaps'][idx][targetKey] = float(value);

        event_match = re.match(r'event\s+\d+\s+(\d+)\s+(\d+)\s+([\d.+-]+)\s+(nan|[\d.+-]+)', line)
        if event_match:
            channel, state, angle, gap = event_match.groups()

            if (720 / get_crank_divider(current_trigger['metadata']['operationMode'], channel, current_trigger['metadata']['isSecondWheelCam'])) - float(angle) < 0.001:
                continue

            if state == "0":
                current_trigger['eventCount'][channel] += 1
                fallAngle = "0.0"
                if current_trigger['metadata']['syncEdge'] != "RiseOnly":
                    fallAngle = angle

                last = None
                last_index = None
                try:
                    last_index = nth_from_last(current_trigger['events'], 1, lambda e: e['channel'] == int(channel))
                    last = current_trigger['events'][last_index]
                except:
                    current_trigger['wrappedFall'][channel] = float(fallAngle)

                if last is None or last['count'] > 1:
                    continue

                current_trigger['events'][last_index]['width'] = float(angle) - last['angle']
            else:
                current_trigger['events'].append({
                    'channel': int(channel),
                    'state': int(state),
                    'angle': float(angle),
                    'gap': None if gap == 'nan' else float(gap),
                    'width': 0.0,
                    'count': 1,
                    'lastAngle': 0.0
                })
                current_trigger['eventCount'][channel] += 1

            if len(current_trigger['events']) < 2:
                continue
            try:
                ult = current_trigger['events'][nth_from_last(current_trigger['events'], 1, lambda e: e['channel'] == int(channel))]
                penult = current_trigger['events'][nth_from_last(current_trigger['events'], 2, lambda e: e['channel'] == int(channel))]
            except ValueError:
                continue;
            mergeEvents(penult, ult, current_trigger)
            continue

    if current_trigger is not None:
        finish_trigger(current_trigger)
        triggers.append(current_trigger)

    return triggers

def mergeEvents(penult, ult, current_trigger):
    if penult['count'] > 1:
        every = (penult['lastAngle'] - penult['angle']) / (penult['count'] - 1)
    if (math.isclose(ult['width'], penult['width'], rel_tol=1e-2, abs_tol=1e-2)
        and (penult['count'] == 1 or math.isclose(ult['angle'] - penult['lastAngle'], every, rel_tol=1e-2, abs_tol=1e-2))):
        penult['lastAngle'] = ult['angle']
        penult['count'] += 1
        current_trigger['events'] = current_trigger['events'][:-1]

def finish_trigger(current_trigger):
    for channel in current_trigger['wrappedFall'].keys():
        ult_index = nth_from_last(current_trigger['events'], 1, lambda e: e['channel'] == int(channel))
        ult = dict(current_trigger['events'][ult_index])
        current_trigger['events'][ult_index]['width'] = (720 + current_trigger['wrappedFall'][channel]) - ult['angle']
        try:
            penult = current_trigger['events'][nth_from_last(current_trigger['events'], 2, lambda e: e['channel'] == int(channel))]
        except ValueError as err:
            continue;
        mergeEvents(penult, ult, current_trigger)

def nth_from_last(lst, n, condition):
    """
    Return the nth item from the end of lst that satisfies condition.

    lst: list of items
    n: 1-based index from last match
    condition: function that takes an item and returns True/False
    """
    count = 0
    for i in range(len(lst) - 1, -1, -1):
        if condition(lst[i]):
            count += 1
            if count == n:
                return i
    raise ValueError("Less than n matching items in the list")

def format_value(val):
    """Format a Python value for YAML output."""
    if isinstance(val, bool):
        return 'true' if val else 'false'
    elif isinstance(val, str) and val.lower() in ('true', 'false'):
        return val
    elif val is None:
        return ''
    elif isinstance(val, float):
        return f'{val:.2f}'.rstrip('0').rstrip('.')
    else:
        return str(val)

def get_crank_divider(mode, channel, secondWheelCam):
    match mode:
        case "FOUR_STROKE_CRANK_SENSOR":
            return 2
        case "FOUR_STROKE_SYMMETRICAL_CRANK_SENSOR":
            return 4
        case "FOUR_STROKE_THREE_TIMES_CRANK_SENSOR":
            return 6
        case "FOUR_STROKE_SIX_TIMES_CRANK_SENSOR":
            return 12
        case "FOUR_STROKE_TWELVE_TIMES_CRANK_SENSOR":
            return 24
        case "FOUR_STROKE_CAM_SENSOR":
            if channel == "1" and not secondWheelCam:
                return 2
            return 1
        case "OM_NONE" | "TWO_STROKE":
            return 1

def is_crank_based(mode):
    return mode in ["FOUR_STROKE_CRANK_SENSOR", "FOUR_STROKE_THREE_TIMES_CRANK_SENSOR", "FOUR_STROKE_SYMMETRICAL_CRANK_SENSOR", "FOUR_STROKE_TWELVE_TIMES_CRANK_SENSOR", "TWO_STROKE"]

def process_cam_trigger(f, secondWheelCam):
    p = []
    s = []
    for e in f:
        if e['channel'] == 0:
            e['angle'] = e['angle'] / 2
            e['width'] = e['width'] / 2
            if e['count'] > 1: e['lastAngle'] = e['lastAngle'] / 2
            p.append(e)
        else:
            if secondWheelCam:
                e['angle'] = e['angle'] / 2
                e['width'] = e['width'] / 2
                if e['count'] > 1: e['lastAngle'] = e['lastAngle'] / 2
            s.append(e)
    if not secondWheelCam:
        s = list(filter(lambda e: e["angle"] <= 360, s))
    j = p + s
    j.sort(key=lambda x: x["angle"])
    return j

def get_events(events, mode, secondWheelCam):
    events = sorted(events, key=lambda x: x["angle"])
    if not is_crank_based(mode):
        events = process_cam_trigger(events, secondWheelCam)

    return events

def write_yaml(triggers, output_path):
    """Write triggers to YAML format."""

    lines = []

    default_values = {
        'isSynchronizationNeeded': True,
        'isSecondWheelCam': False,
        'useOnlyPrimaryForSync': False,
        'shapeWithoutTdc': False,
        'knownOperationMode': True,
    }

    for trigger in triggers:
        lines.append(f'- name: {trigger["name"]}')
        if trigger["offset"] != 0: lines.append(f'  offset: {format_value(trigger["offset"])}')

        if trigger['metadata']:
            for key, value in trigger['metadata'].items():
                if key == "cycleDuration" or key == "hasSecondChannel" or key == "gapsCount" or key == "eventCount" or key == "crankBased":
                    continue
                if key in default_values and value == default_values[key]:
                    continue
                lines.append(f'  {key}: {format_value(value)}')

        if trigger['gaps']:
            lines.append('  syncGaps:')
            for idx, gap in enumerate(trigger['gaps']):
                lines.append(f'    - from: {format_value(gap["from"])}')
                lines.append(f'      to: {format_value(gap["to"])}')

        if trigger['events']:
            lines.append('  teeth:')
            mode = trigger['metadata']['operationMode']
            events = get_events(trigger['events'], mode, trigger['metadata']['isSecondWheelCam'])
            for idx, event in enumerate(events):
                lines.append(f'    - channel: {event["channel"]}')
                lines.append(f'      angle: {format_value(event["angle"])}')
                if event["width"] > 0: lines.append(f'      width: {format_value(event["width"])}')
                if event["count"] > 1: lines.append(f'      count: {event["count"]}')
                if event["count"] > 1: lines.append(f'      lastAngle: {format_value(event["lastAngle"])}')
                gapsIdx = len(events) - idx
                if gapsIdx == len(events):
                    gapsIdx = 0

    with open(output_path, 'w') as f:
        f.write('\n'.join(lines) + '\n')

def main():
    input_file = os.path.dirname(__file__) + '/../unit_tests/triggers.txt'
    output_file = os.path.dirname(__file__) + '/../firmware/config/triggers.yaml'

    triggers = parse_triggers_txt(input_file)
    write_yaml(triggers, output_file)
    print(f'Generated {output_file} with {len(triggers)} trigger definitions')


if __name__ == '__main__':
    main()
