import pandas as pd
sheet_id = "1TT5FeYZpRmhrBHAyqW8Gol4aBldZXQF4vRYXDIsdQhw"
sheet_name = "data_for_connectors_yaml"
url = f"https://docs.google.com/spreadsheets/d/{sheet_id}/gviz/tq?tqx=out:csv&sheet={sheet_name}"
df = pd.read_csv(url)


file = open("epicECU_meta.h", "wb");



file.write("#pragma once\n".encode())


for v in df.sort_values(by='ts_name').itertuples():
    if (v.skip_meta>0):
        continue


    if (v.meta == 'nan'):
        continue

    if (v.function):
        file.write(f"// {v.function} {v.meta}\n".encode())

    if (v.use_meta_not_port== '1'):
        port = v.meta
    else:
        port = v.port

    if (v.pinclass == 'analog_inputs' and type(v.analog_port) == str):
        port = v.analog_port


    file.write(f"#define EPICECU_{v.meta} {port}\n\n".encode())

file.close()
