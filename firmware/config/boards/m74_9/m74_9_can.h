/**
 * @file    m74_9_can.h
 *
 * BCM (Body Control Module) CAN protocol for the m74_9 board.
 *
 * The original ECU communicates with the BCM over CAN. The BCM fully controls
 * the starter relay - rusEFI does not use starterControlPin on this vehicle.
 *
 * rusEFI must:
 *   1. Send the periodic CAN messages the BCM expects from the ECU so the BCM
 *      recognises the ECU as "alive" and allows engine start.
 *   2. Receive BCM state messages to know when cranking is happening.
 *
 * @date    2026-08-12
 */

#pragma once

#if EFI_CAN_SUPPORT

/**
 * Initialize m74_9 BCM CAN communication.
 * Registers the listener and begins transmitting keepalive messages to BCM.
 * Call from setup_custom_board_overrides() after CAN is initialized.
 */
void initM74_9Can();

/**
 * Returns true when the BCM has signalled that the starter relay is active
 * or the engine is in the running state (byte 4 of frame 0x0350 >= 0x80).
 */
bool m74_9_bcmStarterActive();

/**
 * Returns true once the m74_9 BCM immobilizer challenge-response protocol
 * has completed successfully for this ignition cycle.
 */
bool m74_9_immoAuthenticated();

/**
 * Returns true when the immobilizer is currently blocking engine operation.
 * This is used by LimpManager to cut fuel/spark via ClearReason::Immobilizer.
 */
bool m74_9_isImmobilizerBlocking();

#endif // EFI_CAN_SUPPORT
