// Template compiled by test_storage_sd.py with the actual production implementations.
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <map>
#include <string>
#include <vector>
#include "storage.h"
#include "storage_detail.h"

#define TRUE 1
#define FALSE 0
#define EFI_STORAGE_SD TRUE
#define EFI_SUPPORT_FATFS TRUE
#define EFI_PROD_CODE 1
#define NO_CACHE
#define US2MS(x) (x)
#define NT2US(x) (x)
using efitick_t = int;
// Match the host's size_t; production firmware uses 32-bit sizes for both.
using UINT = size_t;
using FSIZE_t = size_t;
enum FRESULT { FR_OK, FR_NO_FILE, FR_DISK_ERR, FR_EXIST };
enum { FA_READ = 1, FA_WRITE = 2, FA_CREATE_ALWAYS = 4, SD_MODE_ECU = 1 };
enum class ObdCode { CUSTOM_ERR_SD_MOUNT_FAILED };
struct FIL { std::string name; bool writing = false; };
static std::map<std::string, std::vector<uint8_t>> files;
static std::string fault;
static bool locked = true;
static int getTimeNowNt() { return 0; }
static int sdCardGetCurrentMode() { return SD_MODE_ECU; }
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
