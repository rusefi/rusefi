package com.rusefi;

// covers both serial use-cases and the non-serial ones (DFU) that are surfaced as synthetic ports (#9771)
public enum SerialPortType {
    Ecu("ECU", 20),
    EcuWithOpenblt("ECU w/ BL", 20),
    OpenBlt("OpenBLT Bootloader", 10),
    // DFU is the STM32 built-in bootloader — a USB device, not a real serial port. It is surfaced as a
    // synthetic PortResult (LinkManager.DFU) so a running console can offer DFU flashing in-session,
    // while DFU detection also stays exposed as AvailableHardware.isDfuFound() for back-compat.
    Dfu("DFU", 15),
    CAN("CAN", 30),
    Unknown("Unknown", 100),
    ;

    public final String friendlyString;
    public final int sortOrder;

    SerialPortType(String friendlyString, int sortOrder) {
        this.friendlyString = friendlyString;
        this.sortOrder = sortOrder;
    }
}
