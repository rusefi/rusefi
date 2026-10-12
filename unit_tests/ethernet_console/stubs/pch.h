#pragma once
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <mutex>

#define EFI_ETHERNET 1
#define ETHERNET_PORT 29001
#define RUSEFI_STACK_FOREIGN_ROOT(...)

namespace chibios_rt {
using Mutex = std::mutex;
using MutexLocker = std::lock_guard<Mutex>;
}
void chThdSleepMilliseconds(int milliseconds);

class TsChannelBase {
public:
    explicit TsChannelBase(const char*) { }
    virtual bool isReady() const { return true; }
    virtual void write(const uint8_t*, size_t, bool) = 0;
    virtual void flush() { }
    virtual size_t readTimeout(uint8_t*, size_t, int) = 0;
    bool in_sync = false;
    bool settingsBurnPending = false;
};

class TunerstudioThread {
public:
    explicit TunerstudioThread(const char*) { }
    virtual TsChannelBase* setupChannel() = 0;
    void start();
};
struct EthernetConsoleModule { void initNoConfiguration(); };
