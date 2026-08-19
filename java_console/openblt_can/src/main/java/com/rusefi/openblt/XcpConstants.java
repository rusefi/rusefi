package com.rusefi.openblt;

/**
 * XCP 1.0 constants for the rusEFI OpenBLT bootloader build (see
 * firmware/hw_layer/openblt/efi_blt_ids.h and ext/openblt/Target/Source/xcp.*).
 *
 * The rusEFI transport is one XCP packet per CAN frame, maximum 8 bytes; there
 * is no multi-frame assembly. Addresses and lengths are little-endian
 * (BOOT_CPU_BYTE_ORDER_MOTOROLA = 0).
 */
public final class XcpConstants {
    /** Response/error packet identifiers (first payload byte). */
    public static final int PID_RES = 0xFF;
    public static final int PID_ERR = 0xFE;

    /** Command codes. */
    public static final int CMD_CONNECT = 0xFF;
    public static final int CMD_DISCONNECT = 0xFE;
    public static final int CMD_GET_STATUS = 0xFD;
    public static final int CMD_GET_ID = 0xFA;
    public static final int CMD_SET_MTA = 0xF6;
    public static final int CMD_SHORT_UPLOAD = 0xF4;
    public static final int CMD_BUILD_CHECKSUM = 0xF3;
    public static final int CMD_PROGRAM_START = 0xD2;
    public static final int CMD_PROGRAM_CLEAR = 0xD1;
    public static final int CMD_PROGRAM = 0xD0;
    public static final int CMD_PROGRAM_RESET = 0xCF;
    public static final int CMD_PROGRAM_MAX = 0xC9;

    /** Error codes we care about (xcp.h). */
    public static final int ERR_CMD_BUSY = 0x10;
    public static final int ERR_CMD_UNKNOWN = 0x20;
    public static final int ERR_OUT_OF_RANGE = 0x22;
    public static final int ERR_GENERIC = 0x36;

    /** BUILD_CHECKSUM algorithm ids (xcp.h); ADD11 is the OpenBLT default. */
    public static final int CS_ADD11 = 0x01;

    /**
     * rusEFI default CAN ids (efi_blt_ids.h): 29-bit extended frames.
     * TX is what the HOST transmits (the bootloader's RX id) and vice versa.
     */
    public static final int TX_ID = 0x667;
    public static final int RX_ID = 0x7E1;
    public static final boolean EXTENDED = true;

    /** CAN baudrate of the bootloader. */
    public static final int BAUDRATE = 500_000;

    /**
     * Where the application image lives. The bootloader occupies the first
     * 32 KB of flash and the target rejects erase/write below this address.
     */
    public static final int APP_BASE = 0x08008000;

    /** CTO (command transfer object) length on CAN: one 8-byte frame. */
    public static final int CTO_LEN = 8;

    /** PROGRAM_MAX carries the whole remaining payload: 8 - 1 command byte. */
    public static final int PROGRAM_MAX_PAYLOAD = CTO_LEN - 1;

    /** PROGRAM carries a size byte plus data: 8 - 2 bytes. */
    public static final int PROGRAM_PAYLOAD = CTO_LEN - 2;

    /** Erase granularity used by the host (BootCommander erases 32 KB chunks). */
    public static final int ERASE_CHUNK = 32 * 1024;

    /** Timeout for one program frame (matches BootCommander's -t7=2000). */
    public static final int PROGRAM_TIMEOUT_MS = 2000;

    /** Timeout for one erase chunk; AT32 sector erase can take a while. */
    public static final int ERASE_TIMEOUT_MS = 15_000;

    /** Timeout for connect/status/checksum commands. */
    public static final int COMMAND_TIMEOUT_MS = 5000;

    private XcpConstants() {
    }
}
