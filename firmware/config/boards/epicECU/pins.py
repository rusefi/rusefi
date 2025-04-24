#!/usr/bin/python3

#
# we're generating pins.yaml file for this ECU from that ther google table
#
#


import pandas as pd


def add_comma_if_needed(x):
    if isinstance(x, (int, float)):
        return "{:,}".format(x)
    return x



sheet_id = "1TT5FeYZpRmhrBHAyqW8Gol4aBldZXQF4vRYXDIsdQhw"
sheet_name = "data_for_connectors_yaml"
url = f"https://docs.google.com/spreadsheets/d/{sheet_id}/gviz/tq?tqx=out:csv&sheet={sheet_name}"
df = pd.read_csv(url)

file = open("connectors/epicECU.yaml", "wb");


file.write("""\
meta: config/boards/epicECU/epicECU_meta.h

pins:
""".encode())

print ("aaaa")

print (df)

for v in df.sort_values(by='ts_name').itertuples():
    if (v.skip_pins>0):
        continue



    file.write(f"""\
  - pin: {v.pin}
""".encode())

    if (v.no_meta != 1):
        file.write(f"""\
    meta: EPICECU_{v.meta}
""".encode())

    if (type(v.pinclass) == str):
        file.write(f"""\
    class: {v.pinclass}
""".encode())

    if (type(v.type) == str):
        file.write(f"""\
    type: {v.type}
""".encode())

    file.write(f"""\
    ts_name: {v.ts_name}
    function: {v.function}
    
""".encode())






file2 = open("board_configuration_generated.h", "wb")

file2.write("static Gpio OUTPUTS[] = {\n".encode())

outputs = df[df['pinclass'] == 'outputs'].sort_values(by=['type', 'ordernum'], ascending = [False, True])
n_rows = len(outputs)
i=0
for v in outputs.itertuples():

    comma = ""

    file2.write(f"    Gpio::EPICECU_{v.meta},  // {v.function}\n".encode())


file2.write("};\n".encode())


file2.close();

"""

static Gpio OUTPUTS[] = {
	Gpio::MM100_INJ6, // B1 injector output 6
	Gpio::MM100_INJ5, // B2 injector output 5
	Gpio::MM100_INJ4, // B3 injector output 4
	Gpio::MM100_INJ3, // B4 injector output 3
	Gpio::MM100_INJ2, // B5 injector output 2
	Gpio::MM100_INJ1, // B6 injector output 1
	Gpio::MM100_INJ7, // B7 Low Side output 1
	Gpio::MM100_IGN8, // B8 Fan Relay Weak Low Side output 2
	Gpio::MM100_IGN7, // B9 Main Relay Weak Low Side output 1
	Gpio::MM100_OUT_PWM2, // B16 Low Side output 4 / Fuel Pump
	Gpio::MM100_OUT_PWM1, // B17 Low Side output 3
	Gpio::MM100_INJ8, // B18 Low Side output 2
	// high sides
	Gpio::MM100_IGN6, // B10 Coil 6
	Gpio::MM100_IGN4, // B11 Coil 4
	Gpio::MM100_IGN3, // B12 Coil 3
	Gpio::MM100_IGN5, // B13 Coil 5
	Gpio::MM100_IGN2, // B14 Coil 2
	Gpio::MM100_IGN1, // B15 Coil 1
};


bool isBoardAllowingLackOfPps() {
	return engineConfiguration->allowIdenticalPps;
}
"""









#     class: event_inputs
#     #if df is not None:
#    print(df)

"""
Name: 126, dtype: object
pin            83
ts_name      VR2+
function      NaN
color        blue
port          PI4
meta        RPM10


  - pin: 19a
    meta: MM100_UART8_TX
    class: event_inputs
    ts_name: VR_9924+
    function: VR_9924 positive input
    type: vr

  - pin: 34a
    function: CAM Sensor Exhaust
    meta: MM100_IN_D3
    class: event_inputs
    ts_name: ___ HALL3
    type: hall
    
    
  - pin: 44a
    class: outputs
    meta: MM100_MEGA_UAEFI_INJ1
    ts_name: ___ INJ_1
    function: Injector 1
    color: green
    type: inj
    
    
  - pin: 14a
    meta: MM100_INJ7
    ts_name: ___ LS1
    class: outputs
    function: Low-Side 1 output
    type: ls
    

"""
