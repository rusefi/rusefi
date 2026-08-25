/**
 * @file    m74_9_lin.h
 *
 * LIN master for the m74_9 smart alternator (VDA generator-regulator family).
 *
 * The stock 21129 alternator is LIN-controlled:
 *   master frame  0x16 (PID 0xD6) - 6 bytes, enhanced checksum - setpoint + LRC
 *   slave response 0x08 (PID 0xC8) - 8 bytes, classic checksum - status
 * Bus: 19200 baud on USART3 (PB10 TX / PB11 RX), hardware LIN break (SBK).
 *
 * @date 2026-08-25
 */

#pragma once

#if EFI_PROD_CODE

/**
 * Initialize the LIN UART and register the console diagnostics.
 * Call from board setup after the engine configuration is loaded.
 */
void initM74_9LinAlternator();

/**
 * Periodic control tick (called from the board slow callback, ~50 ms).
 * Computes the setpoint from alternatorVoltageTargetTable, applies the
 * rpm/MAP gates and the LRC shaping, and transmits the control frame.
 */
void m74_9LinAlternatorPeriodic();

#endif // EFI_PROD_CODE
