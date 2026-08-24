| Image | Entry | Name | Retained | Proxy | Scenario |
|---|---|---|---:|---:|---|
| firmware | main | main/process | 704 | 1196 | startup fast callback |
| firmware | exception/ISR | exception/ISR | - | - | - |
| firmware | __idle_thread | idle | - | - | - |
| firmware | MainLoop::PeriodicTask | main loop | 208 | 620 | normal ADC processing |
| firmware | LuaThread::ThreadTask | Lua | - | - | - |
| firmware | CanRead::ThreadTask | CAN RX | 496 | 1340 | CAN serial receive |
| firmware | BenchController::ThreadTask | bench test | 204 | 2028 | idle production worker |
| firmware | StepperMotor::ThreadTask | stepper | 280 | 652 | startup redundant pedal check |
| firmware | TunerstudioThread::ThreadTask | TunerStudio | 744 | 1316 | Ethernet TS with ARP |
| firmware | LoggingBufferFlusher::ThreadTask | log flush | 132 | 644 | mailbox wait/flush |
| firmware | MicrosecondTimerWatchdogController::PeriodicTask | timer watchdog | 112 | 528 | normal watchdog sleep |
| firmware | CanWrite::PeriodicTask | CAN TX | 1136 | 1024 | Honda-K saturated output |
| firmware | DcThread::PeriodicTask | electronic throttle | 312 | 532 | normal ETB update |
| firmware | storageManagerThread(void*) | storage manager | 292 | 644 | internal-flash tune burn |
| firmware | MMCmonThread(void*) | SD/MMC | 1436 | 2036 | exFAT log creation |
| firmware | MassStorageController::ThreadTask | USB mass storage | 300 | 732 | SD READ/WRITE(10) |
| firmware | lwip_thread | lwIP driver | 192 | 812 | normal Ethernet delivery |
| firmware | tcpip_thread | lwIP TCP/IP | 584 | 660 | TCP write with ARP |
| bootloader | main | main/process | - | - | - |
| bootloader | exception/ISR | exception/ISR | - | - | - |
| bootloader | __idle_thread | idle | - | - | - |
| bootloader | BlinkyThread::main | LED | - | - | - |
