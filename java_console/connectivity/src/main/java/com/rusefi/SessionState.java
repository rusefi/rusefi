package com.rusefi;

/**
 * The single-session device state shared by the splash and the live console [tag:better_ux_for_flashing].
 * <p>
 * A single console instance must handle every board transition — hook, remove, hook-in-DFU,
 * hook-in-OpenBLT, re-connect — without a restart. {@link DeviceSessionManager} derives one of
 * these values from {@link com.rusefi.io.ConnectionStatusLogic} plus the current
 * {@link AvailableHardware} snapshot and broadcasts it so any UI (Device tab, status icon) can
 * reflect the current reality.
 */
public enum SessionState {
    /** No live ECU connection and no board detected in a bootloader/DFU state. */
    DISCONNECTED,
    /** A connect attempt is in flight (LinkManager started, not yet CONNECTED). */
    CONNECTING,
    /** Live ECU connection is up. */
    CONNECTED,
    /** A firmware-update job (DFU or OpenBLT) owns the port; other operations must wait. */
    FLASHING,
    /** A board is sitting in the STM32 built-in bootloader (DFU) — flashable, not connectable. */
    DEVICE_IN_DFU,
    /** A board is sitting in the OpenBLT bootloader — flashable, not connectable. */
    DEVICE_IN_BLT,
}
