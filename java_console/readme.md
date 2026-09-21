# rusEFI console

rusEFI frontend applications is historically known as rusEFI console.

* firmware update
* wizard for one-time initial setup
* on-going monitoring and calibration

Here we have source code for rusEFI console: while [TunerStudio](http://www.tunerstudio.com/index.php/products/tuner-studio) is the primary calibration frontend application we need secondary app for firmware update and some advanced troubleshooting. One day we shall combine but not yet :(

## Linux SocketCAN

The console automatically probes a Linux SocketCAN interface for a rusEFI ECU.
It uses `can0` by default. The interface bitrate must match the ECU configuration;
the standard rusEFI default is 500 kbit/s.

Configure and enable `can0`:

```bash
sudo ip link set can0 down
sudo ip link set can0 type can bitrate 500000 restart-ms 100
sudo ip link set can0 up
```

Verify the interface state and bitrate:

```bash
ip -details -statistics link show can0
```

The output should report `state UP` and `bitrate 500000`. To monitor received
traffic with the `can-utils` package:

```bash
candump can0
```

To use another SocketCAN interface, set the Java system property before `-jar`:

```bash
java -DCAN_DEVICE_NAME=can1 -jar rusefi_console.jar
```

Troubleshooting:

- `Cannot find device "can0"`: the CAN adapter driver has not created the
  interface. Native SocketCAN adapters usually create it automatically; serial
  SLCAN adapters require adapter-specific `slcand` setup first.
- `Network is down`: the interface exists but is not `UP`; run the configuration
  commands above.
- `CAN connected, but no ECU replied`: the interface is active, but no valid
  rusEFI response was received. Check that the bitrate matches the ECU, the ECU
  is powered, CAN high/low are not swapped, and the bus is correctly terminated.

See also http://rusefi.com/build_server/ for pre-compiled full bundle

🔴 not main folder for java code - it's [../java_tools](../java_tools) where primary build.gradle is located 🔴
