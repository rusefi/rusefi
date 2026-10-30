/**
 * @file	mass_storage_device.cpp
 *
 * @date April 9, 2021
 * @author Matthew Kennedy, (c) 2021
 *
 * This file implements a USB mass storage device with multiple LUNs, so multiple drives can be mounted at once.
 */

#include "pch.h"

#include "mass_storage_device.h"

#if HAL_USE_USB_MSD

#define MSD_REQ_RESET                   0xFF
#define MSD_REQ_GET_MAX_LUN             0xFE

#define MSD_CBW_SIGNATURE               0x43425355
#define MSD_CSW_SIGNATURE               0x53425355

#define CBW_FLAGS_RESERVED_MASK         0b01111111
#define CBW_FLAGS_DIRECTION_IN          0b10000000
#define CBW_LUN_RESERVED_MASK           0b11110000
#define CBW_CMD_LEN_RESERVED_MASK       0b11000000

#define CSW_STATUS_PASSED               0x00
#define CSW_STATUS_FAILED               0x01
#define CSW_STATUS_PHASE_ERROR          0x02

#define MSD_SETUP_WORD(setup, index) (uint16_t)(((uint16_t)setup[index+1] << 8)\
                                                | (setup[index] & 0x00FF))

#define MSD_SETUP_VALUE(setup)  MSD_SETUP_WORD(setup, 2)
#define MSD_SETUP_INDEX(setup)  MSD_SETUP_WORD(setup, 4)
#define MSD_SETUP_LENGTH(setup) MSD_SETUP_WORD(setup, 6)

// the one and only mass storage controller, used by the EP0 setup ISR hook and the
// transport callbacks below which only get a USBDriver/SCSITransport pointer
static MassStorageController *s_msdInstance = nullptr;

// After a Bulk-Only Mass Storage Reset (or a data-phase timeout, see below) the host
// has abandoned the command the SCSI layer is still executing. lib_scsi ignores
// transport errors and keeps pumping its data phase, so if we kept doing real USB
// transfers here the leftover data phase would consume the fresh post-reset CBWs as
// if they were data. Refuse all transfers until the command unwinds; ThreadTask then
// waits for the next CBW.
static bool transportAbandoned() {
	return (s_msdInstance != nullptr) && s_msdInstance->isCommandAbandoned();
}

// A host that is alive keeps a data phase moving within milliseconds; usbstor's own
// give-up timeout is ~20s. A data phase quiet for this long means the host canceled
// its transfer and is never coming back for this command.
#define MSD_DATA_PHASE_TIMEOUT TIME_S2I(10)

// Timeout-capable versions of usbTransmit/usbTransmitWait/usbReceive (hal_usb.c),
// which suspend with no timeout. Needed because a host can abandon a command
// mid-data-phase by cancelling its transfer host-side: the bus stays active, so
// neither _usb_reset()/_usb_suspend() nor the Bulk-Only Reset hook ever wakes the
// endpoint wait, and the plain hal_usb.c calls sleep forever (observed: MSD thread
// stuck in one READ(10) for 40+ minutes while Windows reset the composite device
// every ~20s, dropping the CDC console each time). On timeout the endpoint's
// active flag is cleared - same as onBulkOnlyResetIsr() - so the next transfer can
// re-arm without tripping the "already transmitting/receiving" assert.
static msg_t msdUsbTransmitTimeout(USBDriver *usbp, usbep_t ep,
								   const uint8_t *buf, size_t n, sysinterval_t timeout) {
	osalSysLock();

	if (usbGetDriverStateI(usbp) != USB_ACTIVE) {
		osalSysUnlock();
		return MSG_RESET;
	}

	usbStartTransmitI(usbp, ep, buf, n);
	msg_t msg = osalThreadSuspendTimeoutS(&usbp->epc[ep]->in_state->thread, timeout);
	if (MSG_TIMEOUT == msg) {
		usbp->transmitting &= (uint16_t)~(1U << ep);
	}
	osalSysUnlock();

	return msg;
}

static msg_t msdUsbTransmitWaitTimeout(USBDriver *usbp, usbep_t ep, sysinterval_t timeout) {
	osalSysLock();

	if (usbGetDriverStateI(usbp) != USB_ACTIVE) {
		osalSysUnlock();
		return MSG_RESET;
	}

	if ((usbp->transmitting & (uint16_t)(1U << ep)) == 0) {
		osalSysUnlock();
		return MSG_OK;
	}

	msg_t msg = osalThreadSuspendTimeoutS(&usbp->epc[ep]->in_state->thread, timeout);
	if (MSG_TIMEOUT == msg) {
		usbp->transmitting &= (uint16_t)~(1U << ep);
	}
	osalSysUnlock();

	return msg;
}

static msg_t msdUsbReceiveTimeout(USBDriver *usbp, usbep_t ep,
								  uint8_t *buf, size_t n, sysinterval_t timeout) {
	osalSysLock();

	if (usbGetDriverStateI(usbp) != USB_ACTIVE) {
		osalSysUnlock();
		return MSG_RESET;
	}

	usbStartReceiveI(usbp, ep, buf, n);
	msg_t msg = osalThreadSuspendTimeoutS(&usbp->epc[ep]->out_state->thread, timeout);
	if (MSG_TIMEOUT == msg) {
		usbp->receiving &= (uint16_t)~(1U << ep);
	}
	osalSysUnlock();

	return msg;
}

static uint32_t scsi_transport_transmit(const SCSITransport *transport,
										const uint8_t *data, size_t len) {
	if (transportAbandoned()) {
		return 0;
	}

	usb_scsi_transport_handler_t *trp = reinterpret_cast<usb_scsi_transport_handler_t*>(transport->handler);
	msg_t status = msdUsbTransmitTimeout(trp->usbp, trp->ep, data, len, MSD_DATA_PHASE_TIMEOUT);
	if (MSG_TIMEOUT == status) {
		s_msdInstance->onDataPhaseTimeout();
	}
	if (MSG_OK == status) {
		s_msdInstance->onDataPhaseBytes(len);
		return len;
	} else {
		return 0;
	}
}

static uint32_t scsi_transport_transmit_start(const SCSITransport *transport,
											  const uint8_t *data, size_t len) {
	if (transportAbandoned()) {
		return 0;
	}

	usb_scsi_transport_handler_t *trp = reinterpret_cast<usb_scsi_transport_handler_t*>(transport->handler);
	msg_t status = usbTransmitStart(trp->usbp, trp->ep, data, len);
	if (MSG_OK == status) {
		s_msdInstance->onDataPhaseBytes(len);
		return len;
	} else {
		return 0;
	}
}

static uint32_t scsi_transport_transmit_wait(const SCSITransport *transport) {
	if (transportAbandoned()) {
		return 1;
	}

	usb_scsi_transport_handler_t *trp = reinterpret_cast<usb_scsi_transport_handler_t*>(transport->handler);
	msg_t status = msdUsbTransmitWaitTimeout(trp->usbp, trp->ep, MSD_DATA_PHASE_TIMEOUT);
	if (MSG_TIMEOUT == status) {
		s_msdInstance->onDataPhaseTimeout();
	}
	if (MSG_OK == status)
		return 0;
	else
		return 1;
}

static uint32_t scsi_transport_receive(const SCSITransport *transport,
										uint8_t *data, size_t len) {
	if (transportAbandoned()) {
		return 0;
	}

	usb_scsi_transport_handler_t *trp = reinterpret_cast<usb_scsi_transport_handler_t*>(transport->handler);
	msg_t status = msdUsbReceiveTimeout(trp->usbp, trp->ep, data, len, MSD_DATA_PHASE_TIMEOUT);
	if (MSG_TIMEOUT == status) {
		s_msdInstance->onDataPhaseTimeout();
		return 0;
	}
	if (MSG_RESET != status) {
		// usbReceive returns the received byte count when non-negative
		if (status > 0) {
			s_msdInstance->onDataPhaseBytes(status);
		}
		return status;
	} else {
		return 0;
	}
}

MassStorageController::MassStorageController(USBDriver* usb)
	: ThreadController("MSD", MSD_THD_PRIO)
	, m_usb(usb) {
	s_msdInstance = this;
	for (size_t i = 0; i < USB_MSD_LUN_COUNT; i++) {
		scsiObjectInit(&m_luns[i].target);
	}

	m_scsiTransportHandler.usbp = usb;
	m_scsiTransportHandler.ep = USB_MSD_DATA_EP;
	m_scsiTransport.handler  = &m_scsiTransportHandler;
	m_scsiTransport.transmit = scsi_transport_transmit;
	m_scsiTransport.transmit_start = scsi_transport_transmit_start;
	m_scsiTransport.transmit_wait = scsi_transport_transmit_wait;
	m_scsiTransport.receive  = scsi_transport_receive;
}

void MassStorageController::ThreadTask() {
	while (!chThdShouldTerminateX()) {
		const msg_t status = usbReceive(m_usb, USB_MSD_DATA_EP, (uint8_t*)&m_cbw, sizeof(m_cbw));

		if (MSG_RESET == status) {
			chThdSleepMilliseconds(50);
			continue;
		}

		if (cbwValid(m_cbw, status) && cbwMeaningful(m_cbw)) {
			// The host only sends a CBW after any Bulk-Only Mass Storage Reset it issued
			// has completed, so receiving one means an earlier reset is fully handled.
			// Likewise a fresh CBW after a data-phase timeout means the host has moved
			// on from the abandoned command.
			m_botResetPending = false;
			m_dataPhaseTimedOut = false;

			// Get the LUN from the incoming CBW packet
			uint8_t target_lun = m_cbw.lun & 0x0F; // Mask out reserved bits
			if (target_lun <= USB_MSD_LUN_COUNT) {
				// Hold the LUN lock for the whole command, CSW included: attachLun() (the
				// SD mode switch swapping LUN1's backing device) must not return while
				// lib_scsi is still using the old block device. scsiExecCmd captures the
				// blkdev pointer at command start, so an unsynchronized swap lets the SD
				// thread mount/log on the same MMC/SPI driver this command is still
				// reading - ChibiOS sync SPI holds a single waiter per driver, the second
				// thread's DMA wakeup is lost and both threads suspend forever (observed
				// on hardware). The wait is bounded because every USB data-phase wait in
				// this command carries MSD_DATA_PHASE_TIMEOUT.
				chibios_rt::MutexLocker lock(m_lunMutex);

				auto target = &m_luns[target_lun].target;

				// mark busy so 'sdinfo' can show what we are stuck on if this never returns
				m_cmdCount = m_cmdCount + 1;
				m_busyOpcode = m_cbw.cmd_data[0];
				m_busySince = chVTGetSystemTime();

				m_dataPhaseBytes = 0;

				uint8_t cswStatus;
				uint32_t residue;
				if (SCSI_SUCCESS == scsiExecCmd(target, m_cbw.cmd_data)) {
					cswStatus = CSW_STATUS_PASSED;
					residue = 0;
				} else {
					m_failedCmdCount = m_failedCmdCount + 1;
					m_lastFailedOpcode = m_cbw.cmd_data[0];
					cswStatus = CSW_STATUS_FAILED;
					// dCSWDataResidue = expected minus actually moved. Not scsiResidue():
					// lib_scsi only writes that on a short transfer and never resets it
					// between commands, so for a command that moved no data at all it
					// reports a stale value from some earlier command.
					residue = (m_cbw.data_len > m_dataPhaseBytes) ? (m_cbw.data_len - m_dataPhaseBytes) : 0;
				}

				m_busyOpcode = -1;

				if (m_botResetPending) {
					// A Bulk-Only Mass Storage Reset arrived while this command was executing
					// (the reset hook also woke us out of any blocked USB transfer with
					// MSG_RESET). The host explicitly abandoned the command and is not
					// expecting its CSW - sending one anyway would desynchronize the
					// transport.
					continue;
				}

				if (m_dataPhaseTimedOut) {
					// The data phase stopped moving for MSD_DATA_PHASE_TIMEOUT. Either the
					// host canceled its transfers (it will escalate to a reset no matter
					// what we do), or it is still blocked on a transfer this command failed
					// to complete - and going silent then guarantees usbstor's ~20 s
					// give-up: a reset of the whole composite device that tears down the
					// CDC console along with MSD (observed: loss-of-cdc.pcapng, one
					// all-endpoint cancel storm -> ~10 s serial reconnect). Instead, finish
					// the command at the transport level: STALL the data endpoint in the
					// CBW's direction (the BOT-sanctioned "cannot complete this data
					// phase") so a waiting host completes its data URB with an error right
					// now, then offer a phase-error CSW for it to collect after the
					// clear-halt. Recovery stays class-level on this one interface
					// (clear-halt -> CSW -> Bulk-Only Reset); the CDC endpoints never
					// notice. Arming the CSW while the endpoint is still stalled is the
					// same sequence the pre-ZLP medium-less path used, validated on
					// Windows (STALL -> clear-halt -> CSW observed on the wire, #9860).
					osalSysLock();
					if ((m_cbw.flags & CBW_FLAGS_DIRECTION_IN) != 0) {
						usbStallTransmitI(m_usb, USB_MSD_DATA_EP);
					} else {
						usbStallReceiveI(m_usb, USB_MSD_DATA_EP);
					}
					osalSysUnlock();

					uint32_t timeoutResidue = (m_cbw.data_len > m_dataPhaseBytes) ? (m_cbw.data_len - m_dataPhaseBytes) : 0;
					if (sendCsw(CSW_STATUS_PHASE_ERROR, timeoutResidue)) {
						// the host was still listening - command closed without a reset
						m_timeoutCswDeliveredCount = m_timeoutCswDeliveredCount + 1;
					}
					continue;
				}

				if ((m_cbw.data_len > 0) && ((m_cbw.flags & CBW_FLAGS_DIRECTION_IN) != 0)
						&& (m_dataPhaseBytes == 0)) {
					// BOT "13 cases" case 4 (Hi > Dn): the host armed a data-IN URB but
					// this command produced no data (e.g. Read Capacity on a medium-less
					// LUN), so that URB is still waiting. We must NOT send the 13-byte CSW
					// into it: if the URB is smaller (Read Capacity arms 8 bytes) the CSW
					// overruns it, the host takes a babble error, kills the URB, loses the
					// CSW and retries the command forever.
					//
					// Close the data phase with a zero-length packet instead of STALLing.
					// A ZLP is a short packet, so the host's data URB completes with 0
					// bytes and it then collects the CSW into its separate 13-byte read -
					// the same end state as a stall, but WITHOUT the
					// CLEAR_FEATURE(ENDPOINT_HALT) control round-trip a stall forces. That
					// EP0 clear-halt is the last shared-core event that coincided with, and
					// swallowed, an in-flight CDC console reply during Windows' ~20 s
					// medium-less-LUN probe, dropping the serial link into a ~10 s host
					// reconnect (observed: cdc-reconnect.pcapng, CDC gaps aligned to every
					// MSD probe). A ZLP touches only this endpoint, so the CDC interfaces of
					// the composite device are left alone.
					//
					// BOT case 4 nominally recommends STALL; Windows and Linux both accept a
					// ZLP short data phase followed by CSW(failed). Validate on hardware
					// (see USB serial stability, #9860) before relying on it across hosts.
					uint8_t dummy = 0;
					msg_t zlpStatus = msdUsbTransmitTimeout(m_usb, USB_MSD_DATA_EP, &dummy, 0, MSD_DATA_PHASE_TIMEOUT);
					if (MSG_TIMEOUT == zlpStatus) {
						// host abandoned the data phase - don't chase it with a CSW
						onDataPhaseTimeout();
						continue;
					}
					m_noDataZlpCount = m_noDataZlpCount + 1;
				}

				sendCsw(cswStatus, residue);
			}
		} else {
			// ignore incorrect CBW
			m_invalidCbwCount = m_invalidCbwCount + 1;
		}
	}
}

void MassStorageController::onDataPhaseTimeout() {
	// runs on the MSD thread (transport callbacks and sendCsw are only called from it)
	m_dataPhaseTimedOut = true;
	m_timeoutCount = m_timeoutCount + 1;
}

void MassStorageController::printDiagnostics() const {
	efiPrintf("MSD: %lu commands, %lu failed (last failed opcode 0x%02x), %lu invalid CBWs, %lu data-phase timeouts (%lu closed by CSW), %lu no-data ZLPs",
			m_cmdCount, m_failedCmdCount, m_lastFailedOpcode, m_invalidCbwCount, m_timeoutCount, m_timeoutCswDeliveredCount, m_noDataZlpCount);

	// snapshot: m_busySince is only meaningful while m_busyOpcode is set
	int busyOpcode = m_busyOpcode;
	systime_t busySince = m_busySince;
	if (busyOpcode >= 0) {
		efiPrintf("MSD: executing opcode 0x%02x for %lu ms", busyOpcode,
				(uint32_t)TIME_I2MS(chVTTimeElapsedSinceX(busySince)));
	} else {
		efiPrintf("MSD: idle, waiting for next command");
	}
}

void MassStorageController::onBulkOnlyResetIsr(USBDriver *usbp) {
	osalSysLockFromISR();

	// tell ThreadTask the host abandoned the in-flight command, see m_botResetPending checks
	m_botResetPending = true;

	// Abort whatever transfer is in progress on the data endpoint: clear the
	// transfer-active flags (usbStartReceiveI/usbStartTransmitI assert "already
	// receiving/transmitting" on re-arm otherwise) and wake the MSD thread with
	// MSG_RESET. This mirrors what the USB stack itself does on a bus reset in
	// _usb_reset(), scoped down to this one endpoint so the CDC (console/TS)
	// endpoints of the composite device are not disturbed.
	usbp->receiving &= (uint16_t)~(1U << USB_MSD_DATA_EP);
	usbp->transmitting &= (uint16_t)~(1U << USB_MSD_DATA_EP);

	const USBEndpointConfig *epcp = usbp->epc[USB_MSD_DATA_EP];
	if (epcp != NULL) {
		if (epcp->in_state != NULL) {
			osalThreadResumeI(&epcp->in_state->thread, MSG_RESET);
		}
		if (epcp->out_state != NULL) {
			osalThreadResumeI(&epcp->out_state->thread, MSG_RESET);
		}
	}

	osalSysUnlockFromISR();
}

/*static*/ bool MassStorageController::cbwValid(const msd_cbw_t& cbw, msg_t recvd) {
	// check valid size
	if (sizeof(msd_cbw_t) != recvd) {
		return false;
	}

	// Check valid signature
	if (cbw.signature != MSD_CBW_SIGNATURE) {
		return false;
	}

	return true;
}

/*static*/ bool MassStorageController::cbwMeaningful(const msd_cbw_t& cbw) {
	if ((cbw.cmd_len & CBW_CMD_LEN_RESERVED_MASK) != 0) {
		return false;
	}

	if ((cbw.flags & CBW_FLAGS_RESERVED_MASK) != 0) {
		return false;
	}

	if (cbw.lun >= USB_MSD_LUN_COUNT) {
		return false;
	}

	return true;
}

bool MassStorageController::sendCsw(uint8_t status, uint32_t residue) {
	m_csw.signature = MSD_CSW_SIGNATURE;
	m_csw.data_residue = residue;
	m_csw.tag = m_cbw.tag;
	m_csw.status = status;

	// a CSW the host never reads must not wedge the thread either; nothing to unwind
	// on timeout, the loop goes back to waiting for the next CBW regardless
	msg_t transmitStatus = msdUsbTransmitTimeout(m_usb, USB_MSD_DATA_EP,
			(uint8_t*)&m_csw, sizeof(m_csw), MSD_DATA_PHASE_TIMEOUT);
	if (MSG_TIMEOUT == transmitStatus) {
		onDataPhaseTimeout();
	}
	return MSG_OK == transmitStatus;
}

/**
 * @brief   Hardcoded default SCSI inquiry response structure.
 */
static const scsi_inquiry_response_t default_scsi_inquiry_response = {
    0x00,           /* direct access block device     */
    0x80,           /* removable                      */
    0x04,           /* SPC-2                          */
    0x02,           /* response data format           */
    0x20,           /* response has 0x20 + 4 bytes    */
    0x00,
    0x00,
    0x00,
    "Chibios",
    "Mass Storage",
    {'v',CH_KERNEL_MAJOR+'0','.',CH_KERNEL_MINOR+'0'}
};

/**
 * @brief   Hardcoded default SCSI unit serial number inquiry response structure.
 */
static const scsi_unit_serial_number_inquiry_response_t default_scsi_unit_serial_number_inquiry_response =
{
    0x00,
    0x80,
    0x00,
    0x08,
    "0000000"
};

void MassStorageController::attachLun(uint8_t lunIndex,
						BaseBlockDevice *blkdev, uint8_t *blkbuf, size_t blkbufsize,
						const scsi_inquiry_response_t *inquiry,
						const scsi_unit_serial_number_inquiry_response_t *serialInquiry) {
	chibios_rt::MutexLocker lock(m_lunMutex);

	auto& lun = m_luns[lunIndex];


	if (NULL == inquiry) {
		lun.config.inquiry_response = &default_scsi_inquiry_response;
	}
	else {
		lun.config.inquiry_response = inquiry;
	}
	if (NULL == serialInquiry) {
		lun.config.unit_serial_number_inquiry_response = &default_scsi_unit_serial_number_inquiry_response;
	}
	else {
		lun.config.unit_serial_number_inquiry_response = serialInquiry;
	}
	lun.config.blkbuf = blkbuf;
	lun.config.blkbufsize = blkbufsize;
	lun.config.blkdev = blkdev;
	lun.config.transport = &m_scsiTransport;

	scsiStart(&lun.target, &lun.config);
}

bool msd_request_hook_new(USBDriver *usbp) {
	/* check that the request is for interface 0.*/
	if (MSD_SETUP_INDEX(usbp->setup) != 0) {
		return false;
	}

	if (usbp->setup[0] == (USB_RTYPE_TYPE_CLASS | USB_RTYPE_RECIPIENT_INTERFACE | USB_RTYPE_DIR_HOST2DEV)
		&& usbp->setup[1] == MSD_REQ_RESET) {
		/* Bulk-Only Mass Storage Reset (class-specific request)
		This request is used to reset the mass storage device and its associated interface.
		This class-specific request shall ready the device for the next CBW from the host.
		The host sends it when the transport hangs (a CSW it waited for never arrived, a
		transfer timed out); if we ACK without actually resetting, the MSD thread stays
		stuck in the old command and the drive is dead until the cable is re-plugged. */
		if (s_msdInstance != nullptr) {
			s_msdInstance->onBulkOnlyResetIsr(usbp);
		}
		/* reset is complete by the time we get here, ACK the request using EP0 */
		usbSetupTransfer(usbp, 0, 0, NULL);
		return true;
	} else if (usbp->setup[0] == (USB_RTYPE_TYPE_CLASS | USB_RTYPE_RECIPIENT_INTERFACE | USB_RTYPE_DIR_DEV2HOST)
		&& usbp->setup[1] == MSD_REQ_GET_MAX_LUN) {
		/* Return the maximum supported LUN. */
		static uint8_t zero = USB_MSD_LUN_COUNT - 1;
		usbSetupTransfer(usbp, &zero, 1, NULL);
		return true;
	}

	return false;
}

#endif // HAL_USE_USB_MSD
