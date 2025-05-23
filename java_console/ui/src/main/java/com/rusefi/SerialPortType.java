package com.rusefi;

// this enum covers truly serial use-cases meaning NOT 'manual DFU' scanning and not st-link scanning
public enum SerialPortType {
    Ecu("ECU", 20),
    EcuWithOpenblt("ECU w/ BL", 20),
    OpenBlt("OpenBLT Bootloader", 10),
    CAN("CAN", 30),
    Unknown("Unknown", 100),
    // note that somewhere down we have DFU detection but not handled using this enum
    ;

    public final String friendlyString;
    public final int sortOrder;

    SerialPortType(String friendlyString, int sortOrder) {
        this.friendlyString = friendlyString;
        this.sortOrder = sortOrder;
    }
}
