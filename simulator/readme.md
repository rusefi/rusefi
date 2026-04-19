Win32 or posix version of firmware allows to explore rusEFI on a PC without any embedded hardware!

Simulator runs a subset of ECU on your pc, easier to debug some things, tighter dev loop.

Simulator is listening on local TCP ports 29001 and 29002, you would need licensed version of TS to connect to it.

* self-stimulation for trigger event capture
* mocked analog sensors
* mocked outputs
* SocketCAN integration on Linux

One of ways to mock analog sensors
```
// see SensorType.java for numeric ordinals
set_sensor_mock 4 90
```
