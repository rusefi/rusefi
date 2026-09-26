// Template compiled by test_storage_sd.py with the actual production implementations.
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <map>
#include <string>
#include <type_traits>
#include <vector>
#include "storage.h"
#include "storage_detail.h"

#define TRUE 1
#define FALSE 0
#define EFI_STORAGE_SD TRUE
#define EFI_SUPPORT_FATFS TRUE
#define EFI_PROD_CODE 1
#define EFI_SHAFT_POSITION_INPUT 1
#define EFI_LTFT_CONTROL 1
#ifndef HAL_USE_USB_MSD
#define HAL_USE_USB_MSD 1
#endif
#define EFI_TOOTH_LOGGER 0
#define NO_CACHE
#define US2MS(x) (x)
#define NT2US(x) (x)
using efitick_t = int;
// Match the host's size_t; production firmware uses 32-bit sizes for both.
using UINT = size_t;
using FSIZE_t = size_t;
enum FRESULT { FR_OK, FR_NO_FILE, FR_DISK_ERR, FR_EXIST };
enum { FA_READ = 1, FA_WRITE = 2, FA_CREATE_ALWAYS = 4 };
enum SD_MODE { SD_MODE_IDLE, SD_MODE_ECU, SD_MODE_PC, SD_MODE_UNMOUNT, SD_MODE_FORMAT };
enum class ObdCode { CUSTOM_ERR_SD_MOUNT_FAILED };
struct FIL { std::string name; bool writing = false; };
static std::map<std::string, std::vector<uint8_t>> files;
static std::string fault;
static bool locked = true;
static SD_MODE currentMode = SD_MODE_ECU;
static int timeNowMs = 0;
static int getTimeNowNt() { return timeNowMs; }
[[maybe_unused]] static int sdCardGetCurrentMode() { return currentMode; }
static void efiPrintf(const char*, ...) {}
static void printFatFsError(const char*, FRESULT) {}
static void warning(ObdCode, const char*, ...) {}
struct FsGuard { bool isLocked() { return locked; } };
bool storageRegisterStorage(StorageType, SettingStorageBase*) { return true; }
bool storageUnregisterStorage(StorageType) { return true; }

static bool suffix(const std::string& value, const char* ending) {
    const auto length = std::strlen(ending);
    return value.size() >= length && value.compare(value.size() - length, length, ending) == 0;
}

static FRESULT f_open(FIL* fd, const char* name, int flags) {
    const bool writing = (flags & FA_WRITE) != 0;
    if ((fault == "open" && writing) || (fault == "inspect" && !writing && suffix(name, ".bin"))) {
        return FR_DISK_ERR;
    }
    if (flags & FA_CREATE_ALWAYS) {
        files[name].clear();
    }
    if (!files.count(name)) {
        return FR_NO_FILE;
    }
    fd->name = name;
    fd->writing = writing;
    return FR_OK;
}

static FRESULT f_write(FIL* fd, const void* ptr, size_t count, UINT* written) {
    *written = (fault == "write" || fault == "short_write") ? count / 2 : count;
    const auto* bytes = static_cast<const uint8_t*>(ptr);
    files[fd->name].assign(bytes, bytes + *written);
    return fault == "write" ? FR_DISK_ERR : FR_OK;
}

static FRESULT f_read(FIL* fd, void* ptr, size_t count, UINT* read) {
    const bool failing = suffix(fd->name, ".bin") && (fault == "read_error" || fault == "short_read");
    const auto& data = files[fd->name];
    *read = std::min(count, data.size());
    if (failing) {
        *read /= 2;
    }
    std::memcpy(ptr, data.data(), *read);
    return failing && fault == "read_error" ? FR_DISK_ERR : FR_OK;
}

static FRESULT f_sync(FIL*) { return fault == "sync" ? FR_DISK_ERR : FR_OK; }
static FRESULT f_close(FIL* fd) {
    if ((fault == "close" && fd->writing) ||
        ((fault == "inspect_close" || fault == "read_close") && suffix(fd->name, ".bin"))) {
        return FR_DISK_ERR;
    }
    return FR_OK;
}
static FSIZE_t f_size(FIL* fd) { return files[fd->name].size(); }
static FRESULT f_unlink(const char* name) {
    if (fault == "unlink") {
        return FR_DISK_ERR;
    }
    return files.erase(name) ? FR_OK : FR_NO_FILE;
}
static FRESULT f_rename(const char* from, const char* to) {
    if ((fault == "rotate" && suffix(to, ".bak")) ||
        (fault == "promote" && suffix(from, ".tmp")) ||
        (fault == "restore" && suffix(to, ".bin"))) {
        return FR_DISK_ERR;
    }
    if (!files.count(from)) {
        return FR_NO_FILE;
    }
    if (files.count(to)) {
        return FR_EXIST;
    }
    files[to] = files[from];
    files.erase(from);
    return FR_OK;
}

@SD_SOURCE@

static constexpr size_t storagesCount = 2;
static SettingStorageBase* storages[storagesCount];
@READ_SOURCE@

@LTFT_DIMENSIONS@
@LTFT_DECLARATION@
static LtftState ltftLoadState;
@LTFT_LOAD@

#define BIT(x) (1U << (x))
#define MSG_CMD_PING 3
static uint32_t pendingReads = 0;
namespace chibios_rt {
struct CriticalSectionLocker { CriticalSectionLocker() {} };
}
@REQUEST_HELPERS@
static bool isReadPending(StorageItemId id) { return (getPendingReads() & BIT(id)) != 0; }
static bool pendingBeforeWakeup = false;
static bool storageManagerSendCmd(uint32_t, uint32_t) {
    pendingBeforeWakeup = isReadPending(EFI_LTFT_RECORD_ID);
    return false; // Deliberately full mailbox: periodic polling must still see the read.
}
@REQUEST_READ@
static void pollReads();
static unsigned slept = 0;
static unsigned completeAfter = 0;
static bool pollWhileSleeping = false;
static void chThdSleepMilliseconds(unsigned ms) {
    slept += ms;
    timeNowMs += static_cast<int>(ms);
    if (completeAfter && slept >= completeAfter) {
        pendingReads &= ~BIT(EFI_LTFT_RECORD_ID);
    }
    if (pollWhileSleeping) {
        pollReads();
    }
}
@WAIT_READ@
namespace withoutStorage {
@WAIT_STUB@
}
#define for_all_storages SettingStorageBase* storage = nullptr; \
    for (size_t i = 0; i < storagesCount; i++) if ((storage = storages[i]) != nullptr)
@AVAILABLE@

// Only the controller state and platform services are mocked; methods below
// are extracted unchanged from the firmware, with production reads enabled.
class LongTermFuelTrim {
public:
    bool ltftLoadPending = false;
    bool ltftLoadError = false;
    void init(LtftState* state);
    bool load();
    void onSlowCallback();
    void onEngineStop();
private:
    LtftState* m_state = nullptr;
};
struct SdLogTrigger {
    int getState() { return 1; }
    bool shouldLog() { return true; }
};
struct FakeEngine {
    struct {
        bool stopped = true;
        bool isStopped() { return stopped; }
        float getSecondsSinceEngineStart(int now) { return static_cast<float>(now) / 1000; }
    } rpmCalculator;
    struct {
        uint8_t sdLoggingState = 0;
        bool sd_logging_internal = true;
    } outputChannels;
    LongTermFuelTrim* ltft = nullptr;
    SdLogTrigger trigger;
    template <typename T> T* module() {
        if constexpr (std::is_same_v<T, LongTermFuelTrim>) {
            return ltft;
        } else {
            return &trigger;
        }
    }
};
static FakeEngine engineObject;
static FakeEngine* engine = &engineObject;
@LTFT_MODULE@
static void loadExtraPage(StorageItemId) {}
@READ_ID@
static void pollReads() {
@POLL_READS@
}

static struct { bool alwaysWriteSdCard = false; } configuration;
[[maybe_unused]] static auto* engineConfiguration = &configuration;
static bool usbConnected = false;
static bool sdTargetModeRequested = false;
static bool sdNeedRemoveReports = false;
static bool sdLoggedSuppressed = false;
static bool sdLoggerFailed = false;
static bool sdLoggerInitDone = false;
static unsigned loggerStarts = 0, loggerWrites = 0;
enum { SD_LOG_SUPPRESSED, SD_LOG_FAILED };
enum class SDLoggerMode { Mlg, None, Dtc };
static auto sdLoggerMode = SDLoggerMode::Mlg;
static struct { FIL fd; } resources;
static void errorHandlerDeleteReports() {}
static void sdLoggerStart() { ++loggerStarts; }
static void sdLoggerStop() {}
static int sdLoggerMlg(FIL*) { ++loggerWrites; return 1; }
@SD_EXECUTOR@

struct FakeStorage : SettingStorageBase {
    StorageStatus status;
    uint8_t value;
    FakeStorage(StorageStatus result, uint8_t byte) : status(result), value(byte) {}
    bool isReady() override { return true; }
    bool isIdSupported(size_t) override { return true; }
    StorageStatus store(size_t, const uint8_t*, size_t) override { return status; }
    StorageStatus read(size_t, uint8_t* ptr, size_t) override { *ptr = value; return status; }
    StorageStatus format() override { return StorageStatus::NotSupported; }
};

int main(int argc, char** argv) {
    assert(argc == 3);
    const std::string scenario = argv[1];
    const std::string record = argv[2];
    if (scenario == "ready_closed") {
        locked = false;
        std::printf("{\"ready\":%s}\n", storageSD.isReady() ? "true" : "false");
        return 0;
    }
    if (scenario.rfind("usb_", 0) == 0) {
        usbConnected = true;
        configuration.alwaysWriteSdCard = scenario == "usb_always";
        sdTargetModeRequested = scenario == "usb_requested";
        (void)sdModeExecuter(SD_MODE_ECU);
        std::printf("{\"started\":%u,\"written\":%u,\"logging\":%s}\n", loggerStarts, loggerWrites,
                    engine->outputChannels.sd_logging_internal ? "true" : "false");
        return 0;
    }
    if (scenario.rfind("wait_", 0) == 0) {
        if (scenario == "wait_invalid") {
            const bool zero = storageWaitReadDone(static_cast<StorageItemId>(0), 0);
            const bool limit = storageWaitReadDone(EFI_STORAGE_TOTAL_ITEMS, 0);
            std::printf("{\"zero\":%s,\"limit\":%s,\"stub_invalid\":%s,\"stub_valid\":%s}\n",
                        zero ? "true" : "false", limit ? "true" : "false",
                        withoutStorage::storageWaitReadDone(EFI_STORAGE_TOTAL_ITEMS, 0) ? "true" : "false",
                        withoutStorage::storageWaitReadDone(EFI_LTFT_RECORD_ID, 0) ? "true" : "false");
            return 0;
        }
        pendingReads = BIT(EFI_LTFT_RECORD_ID) | BIT(EFI_SECOND_TABLES_RECORD_ID);
        if (scenario == "wait_complete") {
            completeAfter = 20;
        } else if (scenario == "wait_unrelated") {
            pendingReads &= ~BIT(EFI_LTFT_RECORD_ID);
        }
        const bool done = storageWaitReadDone(EFI_LTFT_RECORD_ID, 25);
        std::printf("{\"done\":%s,\"slept\":%u,\"unrelated\":%s}\n", done ? "true" : "false", slept,
                    isReadPending(EFI_SECOND_TABLES_RECORD_ID) ? "true" : "false");
        return 0;
    }
    if (scenario == "startup" || scenario == "queue_full" || scenario.rfind("module_", 0) == 0) {
        storages[1] = &storageSD;
        LtftState active{}, saved{};
        for (auto& bank : active.trims) {
            for (auto& row : bank) {
                for (auto& value : row) { value = 0.125f; }
            }
        }
        for (auto& bank : saved.trims) {
            for (auto& row : bank) {
                for (auto& value : row) { value = 0.25f; }
            }
        }
        const auto* bytes = reinterpret_cast<const uint8_t*>(saved.trims);
        files["ltft.bin"] = {bytes, bytes + sizeof(saved.trims)};
        LongTermFuelTrim controller;
        engine->ltft = &controller;
        controller.init(&active);
        if (scenario == "startup") {
            currentMode = SD_MODE_IDLE;
            pollWhileSleeping = true;
            const bool done = storageWaitReadDone(EFI_LTFT_RECORD_ID, 25);
            std::printf("{\"done\":%s,\"slept\":%u,\"pending\":%s}\n", done ? "true" : "false", slept,
                        isReadPending(EFI_LTFT_RECORD_ID) ? "true" : "false");
        } else if (scenario == "queue_full") {
            const bool accepted = storageReqestReadID(EFI_LTFT_RECORD_ID);
            pollReads();
            std::printf("{\"accepted\":%s,\"before_wakeup\":%s,\"done\":%s,\"pending\":%s}\n",
                        accepted ? "true" : "false", pendingBeforeWakeup ? "true" : "false",
                        storageWaitReadDone(EFI_LTFT_RECORD_ID, 0) ? "true" : "false",
                        isReadPending(EFI_LTFT_RECORD_ID) ? "true" : "false");
        } else if (scenario == "module_stopped") {
            timeNowMs = 8000;
            controller.onSlowCallback();
            std::printf("{\"pending\":%s,\"error\":%s}\n", controller.ltftLoadPending ? "true" : "false",
                        controller.ltftLoadError ? "true" : "false");
        } else if (scenario == "module_late") {
            engine->rpmCalculator.stopped = false;
            timeNowMs = 8000;
            controller.onSlowCallback();
            const bool deferred = !controller.load();
            const bool intact = active.trims[0][0][0] == 0.125f;
            engine->rpmCalculator.stopped = true;
            controller.onEngineStop();
            const bool retried = controller.ltftLoadPending;
            pollReads();
            const bool loaded = active.trims[0][0][0] == 0.25f && !isReadPending(EFI_LTFT_RECORD_ID);
            std::printf("{\"deferred\":%s,\"intact\":%s,\"retried\":%s,\"loaded\":%s,\"error\":%s}\n",
                        deferred ? "true" : "false", intact ? "true" : "false", retried ? "true" : "false",
                        loaded ? "true" : "false", controller.ltftLoadError ? "true" : "false");
        } else {
            if (scenario == "module_missing") {
                files.clear();
            } else {
                fault = "read_error";
            }
            pollReads();
            const bool done = storageWaitReadDone(EFI_LTFT_RECORD_ID, 0);
            const bool error = controller.ltftLoadError;
            controller.onEngineStop();
            std::printf("{\"done\":%s,\"error\":%s,\"retry\":%s,\"intact\":%s}\n",
                        done ? "true" : "false", error ? "true" : "false",
                        isReadPending(EFI_LTFT_RECORD_ID) ? "true" : "false",
                        active.trims[0][0][0] == 0.125f ? "true" : "false");
        }
        return 0;
    }
    if (scenario == "priority") {
        FakeStorage lower(StorageStatus::Ok, 17), higher(StorageStatus::Failed, 99);
        storages[0] = &lower;
        storages[1] = &higher;
        uint8_t value = 0;
        const auto status = storageRead(EFI_LTFT_RECORD_ID, &value, 1);
        std::printf("{\"status\":%d,\"value\":%u}\n", int(status), unsigned(value));
        return 0;
    }
    if (scenario.rfind("ltft_", 0) == 0) {
        storages[1] = &storageSD;
        LtftState active{}, saved{};
        for (auto& bank : active.trims) {
            for (auto& row : bank) {
                for (auto& value : row) { value = 0.125f; }
            }
        }
        for (auto& bank : saved.trims) {
            for (auto& row : bank) {
                for (auto& value : row) { value = 0.25f; }
            }
        }
        const auto* bytes = reinterpret_cast<const uint8_t*>(saved.trims);
        files["ltft.bin"] = {bytes, bytes + sizeof(saved.trims)};
        if (scenario == "ltft_partial" || scenario == "ltft_backup") {
            fault = "read_error";
        } else if (scenario == "ltft_close") {
            fault = "read_close";
        } else if (scenario == "ltft_missing") {
            files.clear();
        }
        if (scenario == "ltft_backup") {
            files["ltft.bak"] = files["ltft.bin"];
        }
        active.load();
        const bool success = scenario == "ltft_success" || scenario == "ltft_backup";
        bool intact = true;
        for (const auto& bank : active.trims) {
            for (const auto& row : bank) {
                for (auto value : row) { intact &= value == (success ? 0.25f : 0.125f); }
            }
        }
        std::printf("{\"intact\":%s,\"bytes\":%u}\n", intact ? "true" : "false", unsigned(sizeof(active.trims)));
        return 0;
    }

    const auto id = record == "ltft" ? EFI_LTFT_RECORD_ID :
        (record == "second_tables" ? EFI_SECOND_TABLES_RECORD_ID : EFI_LUA_PAGE_RECORD_ID);
    const auto primary = record + ".bin";
    const auto backup = record + ".bak";
    const size_t size = record == "lua_script" ? 8032 : (record == "ltft" ? 2048 : 1280);
    const std::vector<uint8_t> old(size, 17), replacement(size, 99);
    files[primary] = old;
    if (scenario.rfind("missing", 0) == 0) {
        files.erase(primary);
        files[backup] = old;
    } else if (scenario.rfind("truncated", 0) == 0) {
        files[primary] = {1};
        files[backup] = old;
    } else if (scenario.rfind("oversized", 0) == 0) {
        files[primary] = std::vector<uint8_t>(size + 1, 1);
        files[backup] = old;
    } else if (scenario == "closed") {
        locked = false;
    }

    const bool reading = scenario == "missing" || scenario == "truncated" || scenario == "oversized" ||
        scenario == "read_error" || scenario == "short_read" || scenario == "read_close";
    if (scenario == "read_error" || scenario == "short_read" || scenario == "read_close") {
        files[backup] = old;
    }
    fault = suffix(scenario, "_promote") ? "promote" : scenario;
    StorageStatus status = StorageStatus::Ok;
    if (!reading) {
        status = storageSD.store(id, replacement.data(), replacement.size());
        fault.clear();
        locked = true;
    }
    std::vector<uint8_t> readBack(size);
    const auto readStatus = storageSD.read(id, readBack.data(), readBack.size());
    const auto foundBackup = files.find(backup);
    const bool goodBackup = foundBackup != files.end() && foundBackup->second == old;
    std::printf("{\"status\":%d,\"read_status\":%d,\"old\":%s,\"new\":%s,\"backup\":%s}\n",
        int(status), int(readStatus), readBack == old ? "true" : "false",
        readBack == replacement ? "true" : "false", goodBackup ? "true" : "false");
}
