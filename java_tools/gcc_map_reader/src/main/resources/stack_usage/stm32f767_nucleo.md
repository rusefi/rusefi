| Image | Entry | Name | Retained | Proxy | Scenario |
|---|---|---|---:|---:|---|
| firmware | main | main/process | - | - | - |
| firmware | exception/ISR | exception/ISR | - | - | - |
| firmware | __idle_thread | idle | - | - | - |
| firmware | MainLoop::PeriodicTask | main loop | - | - | - |
| firmware | LuaThread::ThreadTask | Lua | - | - | - |
| firmware | CanRead::ThreadTask | CAN RX | 296 | 1212 | normal IOBox receive |
| firmware | BenchController::ThreadTask | bench test | - | - | - |
| firmware | StepperMotor::ThreadTask | stepper | 220 | 652 | normal startup positioning |
| firmware | TunerstudioThread::ThreadTask | TunerStudio | 804 | 1244 | normal tune burn |
| firmware | LoggingBufferFlusher::ThreadTask | log flush | 120 | 636 | mailbox wait/flush |
| firmware | MicrosecondTimerWatchdogController::PeriodicTask | timer watchdog | 112 | 528 | normal watchdog sleep |
| firmware | CanWrite::PeriodicTask | CAN TX | 648 | 836 | Honda-K dashboard output |
| firmware | DcThread::PeriodicTask | electronic throttle | 256 | 8 | normal ETB update |
| firmware | storageManagerThread(void*) | storage manager | 308 | 628 | internal-flash tune burn |
| firmware | lwip_thread | lwIP driver | 240 | 804 | normal Ethernet delivery |
| firmware | tcpip_thread | lwIP TCP/IP | - | - | - |
| bootloader | main | main/process | - | - | - |
| bootloader | exception/ISR | exception/ISR | - | - | - |
| bootloader | __idle_thread | idle | - | - | - |
| bootloader | BlinkyThread::main | LED | - | - | - |
