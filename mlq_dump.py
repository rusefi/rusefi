import struct
import sys

path = sys.argv[1]
data = open(path, 'rb').read()

data_begin = struct.unpack_from('>i', data, 16)[0]
rec_len = struct.unpack_from('>H', data, 20)[0]
num_fields = struct.unpack_from('>H', data, 22)[0]

SIZE = {0: 1, 1: 1, 2: 2, 3: 2, 4: 4, 7: 4}
fields = []
pos = 24
for i in range(num_fields):
    t = data[pos]; pos += 1
    name = data[pos:pos+34].split(b'\x00')[0].decode('latin1'); pos += 34
    unit = data[pos:pos+11].split(b'\x00')[0].decode('latin1'); pos += 11
    scale = struct.unpack_from('>f', data, pos)[0]; pos += 4
    pos += 5 + 34
    fields.append((t, name, unit, scale))

idx = {f[1]: i for i, f in enumerate(fields)}

records = []
pos = data_begin
n = len(data)
while pos + rec_len + 4 <= n:
    b0 = data[pos]
    tms = struct.unpack_from('>H', data, pos + 2)[0]
    p = pos + 4
    vals = []
    for (t, name, unit, scale) in fields:
        s = SIZE[t]
        if t == 7:
            v = struct.unpack_from('>f', data, p)[0]
        elif s == 1:
            v = data[p] if t == 0 else struct.unpack_from('>b', data, p)[0]
        elif s == 2:
            v = struct.unpack_from('>h', data, p)[0]
        else:
            v = struct.unpack_from('>i', data, p)[0]
        vals.append(v * scale)
        p += s
    if b0 != 0 or p != pos + 4 + rec_len:
        break
    records.append((tms, vals))
    pos += 4 + rec_len + 1

def get(name, r):
    return records[r][1][idx[name]]

names = [
    'seconds', 'rpmvalue', 'instantrpm', 'mapvalue', 'instantmapvalue', 'effectivemap',
    'fallbackmap', 'baropressure', 'rawmap', 'rawmapfast', 'vbatt', 'basedwell',
    'dwellvoltagecorrection', 'dwelldurationangle', 'sparkduration1',
    'coolant', 'intake', 'tpsvalue', 'throttlepedalposition',
    'trgsynchronizationcounter', 'totaltriggererrorcounter', 'lasterrorcode', 'warningcounter',
    'enginemode', 'actuallastinjection', 'injectordutycycle',
    'crankingfuel_fuel', 'crankingfuel_basefuel', 'running_basefuel',
    'ignitionadvancecyl1', 'correctedignitionadvance', 'revolutioncountersincestart',
    'iscranking', 'starterstate', 'sparkcutreason', 'fuelcutreason',
    'fuelflowrate', 'triggertoothangleerror', 'istriggererror',
    'triggerignoredtoothcount', 'phaseresynccounter', 'dwelloverchargecounter',
    'stopenginecode', 'throttleestimatedflow', 'lambdatimesincegood',
    'smoothedafrvalue', 'engineruntime', 'ignitionontime', 'instantrpmrange',
    'injectionoffset', 'targetrpmbyclt',
]
names = [x for x in names if x in idx]

print("records:", len(records))
print()
print('\t'.join(names))
for r in range(len(records)):
    row = []
    for nm in names:
        v = get(nm, r)
        if isinstance(v, float):
            row.append(f"{v:.2f}")
        else:
            row.append(f"{v:.0f}")
    print('\t'.join(row))
