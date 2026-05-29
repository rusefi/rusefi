// file msl_to_csv.cpp
//
// Reader of the binary MLG/MLVLG log written by unit_test_logger.cpp.
// Produces a CSV with one header row of field names (with units) and one
// data row per logged record. The raw integer/float value of each field is
// multiplied by its declared scale (per MLG field descriptor) before being
// written out, so the CSV contains physical values just like TunerStudio.

#include "msl_to_csv.h"

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>

namespace {

// All multi-byte values on disk are big-endian (see binary_mlg_logging.cpp
// and Field::memcpy_swapend in mlg_field.h).

static uint16_t readU16BE(const uint8_t* p) {
    return (uint16_t(p[0]) << 8) | uint16_t(p[1]);
}

static uint32_t readU32BE(const uint8_t* p) {
    return (uint32_t(p[0]) << 24) | (uint32_t(p[1]) << 16)
         | (uint32_t(p[2]) << 8)  |  uint32_t(p[3]);
}

static float readF32BE(const uint8_t* p) {
    uint32_t u = readU32BE(p);
    float f;
    std::memcpy(&f, &u, sizeof(f));
    return f;
}

// MLG scalar type ids - see mlg_types.h
enum class ScalarType : uint8_t {
    U08 = 0, S08 = 1, U16 = 2, S16 = 3, U32 = 4, S32 = 5, S64 = 6, F32 = 7,
};

static size_t sizeOfType(ScalarType t) {
    switch (t) {
        case ScalarType::U08: case ScalarType::S08: return 1;
        case ScalarType::U16: case ScalarType::S16: return 2;
        case ScalarType::U32: case ScalarType::S32: case ScalarType::F32: return 4;
        case ScalarType::S64: return 8;
    }
    return 0;
}

struct FieldDesc {
    ScalarType type;
    std::string name;
    std::string units;
    float scale;       // raw * scale = physical value
    int8_t digits;     // decimal places
    size_t size;       // size in bytes of the raw value
};

// Reads the raw value of `f` from `p` and returns the (already scaled) double
// physical value plus the size in bytes consumed.
static double readScaledValue(const FieldDesc& f, const uint8_t* p) {
    double raw = 0;
    switch (f.type) {
        case ScalarType::U08: raw = double(uint8_t(p[0])); break;
        case ScalarType::S08: raw = double(int8_t(p[0]));  break;
        case ScalarType::U16: raw = double(readU16BE(p));  break;
        case ScalarType::S16: raw = double(int16_t(readU16BE(p))); break;
        case ScalarType::U32: raw = double(readU32BE(p));  break;
        case ScalarType::S32: raw = double(int32_t(readU32BE(p))); break;
        case ScalarType::S64: {
            uint64_t u = (uint64_t(readU32BE(p)) << 32) | uint64_t(readU32BE(p + 4));
            raw = double(int64_t(u));
            break;
        }
        case ScalarType::F32: raw = double(readF32BE(p)); break;
    }
    return raw * double(f.scale);
}

static std::string cleanField(std::string s) {
    // CSV escape - if field contains comma or quote, wrap in quotes and escape
    bool needsQuoting = s.find(',') != std::string::npos
                     || s.find('"') != std::string::npos;
    if (!needsQuoting) {
        return s;
    }
    std::string out = "\"";
    for (char c : s) {
        if (c == '"') {
            out += "\"\"";
        } else {
            out += c;
        }
    }
    out += "\"";
    return out;
}

} // namespace

bool convertMslToCsv(const char* mslPath, const char* csvPath) {
    FILE* in = std::fopen(mslPath, "rb");
    if (!in) {
        std::fprintf(stderr, "convertMslToCsv: failed to open input '%s'\n", mslPath);
        return false;
    }

    // Read header (24 bytes)
    uint8_t hdr[24];
    if (std::fread(hdr, 1, sizeof(hdr), in) != sizeof(hdr)) {
        std::fprintf(stderr, "convertMslToCsv: file too short for header\n");
        std::fclose(in);
        return false;
    }
    if (std::memcmp(hdr, "MLVLG", 5) != 0) {
        std::fprintf(stderr, "convertMslToCsv: bad magic in '%s'\n", mslPath);
        std::fclose(in);
        return false;
    }

    uint32_t dataBeginIndex = readU32BE(hdr + 16);
    uint16_t recordLength   = readU16BE(hdr + 20);
    uint16_t numFields      = readU16BE(hdr + 22);

    // Read field descriptors (89 bytes each)
    std::vector<FieldDesc> fields;
    fields.reserve(numFields);
    for (uint16_t i = 0; i < numFields; i++) {
        uint8_t d[89];
        if (std::fread(d, 1, sizeof(d), in) != sizeof(d)) {
            std::fprintf(stderr, "convertMslToCsv: truncated field descriptor %u\n", i);
            std::fclose(in);
            return false;
        }
        FieldDesc f{};
        f.type   = static_cast<ScalarType>(d[0]);
        f.name   = std::string(reinterpret_cast<const char*>(d + 1),  strnlen(reinterpret_cast<const char*>(d + 1),  34));
        f.units  = std::string(reinterpret_cast<const char*>(d + 35), strnlen(reinterpret_cast<const char*>(d + 35), 10));
        f.scale  = readF32BE(d + 46);
        f.digits = static_cast<int8_t>(d[54]);
        f.size   = sizeOfType(f.type);
        fields.push_back(std::move(f));
    }

    // Skip any remaining bytes up to dataBeginIndex (info data area)
    long curPos = std::ftell(in);
    if (curPos < 0) {
        std::fclose(in);
        return false;
    }
    if (uint32_t(curPos) < dataBeginIndex) {
        if (std::fseek(in, long(dataBeginIndex), SEEK_SET) != 0) {
            std::fclose(in);
            return false;
        }
    }

    FILE* out = std::fopen(csvPath, "w");
    if (!out) {
        std::fprintf(stderr, "convertMslToCsv: failed to open output '%s'\n", csvPath);
        std::fclose(in);
        return false;
    }

    // CSV header row: "name (units)" per column
    for (size_t i = 0; i < fields.size(); i++) {
        std::string col = fields[i].name;
        if (!fields[i].units.empty()) {
            col += " (" + fields[i].units + ")";
        }
        std::fputs(cleanField(col).c_str(), out);
        std::fputc(i + 1 == fields.size() ? '\n' : ',', out);
    }

    // Per-record format (see writeSdBlock):
    //   byte 0: block type (0 = standard data block)
    //   byte 1: rolling counter
    //   bytes 2-3: timestamp (big-endian, 10us units)
    //   then `recordLength` bytes of payload (concatenated field data, big-endian)
    //   then 1 byte checksum (sum of payload bytes)
    std::vector<uint8_t> payload(recordLength);
    uint64_t records = 0;
    while (true) {
        uint8_t blockHdr[4];
        size_t r = std::fread(blockHdr, 1, sizeof(blockHdr), in);
        if (r == 0) {
            break; // EOF
        }
        if (r != sizeof(blockHdr)) {
            std::fprintf(stderr, "convertMslToCsv: truncated block header at record %llu\n",
                         (unsigned long long)records);
            break;
        }
        if (std::fread(payload.data(), 1, payload.size(), in) != payload.size()) {
            std::fprintf(stderr, "convertMslToCsv: truncated payload at record %llu\n",
                         (unsigned long long)records);
            break;
        }
        uint8_t checksum;
        if (std::fread(&checksum, 1, 1, in) != 1) {
            std::fprintf(stderr, "convertMslToCsv: missing checksum at record %llu\n",
                         (unsigned long long)records);
            break;
        }
        // Decode payload
        size_t off = 0;
        for (size_t i = 0; i < fields.size(); i++) {
            const FieldDesc& f = fields[i];
            if (off + f.size > payload.size()) {
                std::fprintf(stderr, "convertMslToCsv: payload overrun in record %llu\n",
                             (unsigned long long)records);
                break;
            }
            double v = readScaledValue(f, payload.data() + off);
            int digits = f.digits < 0 ? 0 : int(f.digits);
            if (digits > 12) digits = 12;
            char numBuf[64];
            std::snprintf(numBuf, sizeof(numBuf), "%.*f", digits, v);
            std::fputs(numBuf, out);
            std::fputc(i + 1 == fields.size() ? '\n' : ',', out);
            off += f.size;
        }
        records++;
    }

    std::fclose(in);
    std::fclose(out);
    std::printf("convertMslToCsv: wrote %llu records, %u fields to %s\n",
                (unsigned long long)records, (unsigned)numFields, csvPath);
    return true;
}

void runMsl2Csv(const char* mslFileName) {
    std::string input(mslFileName);
    std::string output;
    size_t lastDot = input.find_last_of(".");
    if (lastDot == std::string::npos) {
        output = input + ".csv";
    } else {
        output = input.substr(0, lastDot) + ".csv";
    }

    if (!convertMslToCsv(mslFileName, output.c_str())) {
        std::fprintf(stderr, "Failed to convert %s\n", mslFileName);
    }
}
