| Image | Entry | Name | Retained | Proxy | Scenario |
|---|---|---|---:|---:|---|
| firmware | main | main/process | - | - | - |
| firmware | exception/ISR | exception/ISR | - | - | - |
| firmware | __idle_thread | idle | - | - | - |
| firmware | MainLoop::PeriodicTask | main loop | - | - | - |
| firmware | LuaThread::ThreadTask | Lua | - | - | - |
| firmware | CanRead::ThreadTask | CAN RX | 440 | 1244 | CAN serial receive |
| firmware | BenchController::ThreadTask | bench test | 148 | 1932 | idle production worker |
| firmware | StepperMotor::ThreadTask | stepper | 280 | 676 | dual H-bridge stepping |
| firmware | TunerstudioThread::ThreadTask | TunerStudio | 824 | 1324 | normal communication |
| firmware | LoggingBufferFlusher::ThreadTask | log flush | 124 | 660 | mailbox wait/flush |
| firmware | MicrosecondTimerWatchdogController::PeriodicTask | timer watchdog | 104 | 552 | normal watchdog sleep |
| firmware | CanWrite::PeriodicTask | CAN TX | 600 | 860 | Honda-K dashboard output |
| firmware | DcThread::PeriodicTask | electronic throttle | 304 | 556 | normal ETB update |
| firmware | storageManagerThread(void*) | storage manager | 1032 | 660 | SD extra-page burn |
| firmware | MMCmonThread(void*) | SD/MMC | 1432 | 2052 | exFAT log creation |
| firmware | MassStorageController::ThreadTask | USB mass storage | 240 | 756 | SD block read |
| firmware | Max3185xRead::ThreadTask | MAX3185x | 192 | 756 | normal sensor read |
| firmware | KnockThread::ThreadTask | software knock | 144 | 628 | normal RMS processing |
| firmware | AccelController::PeriodicTask | accelerometer | 184 | 24 | normal SPI sensor read |
| bootloader | main | main/process | - | - | - |
| bootloader | exception/ISR | exception/ISR | - | - | - |
| bootloader | __idle_thread | idle | - | - | - |
| bootloader | BlinkyThread::main | LED | - | - | - |
