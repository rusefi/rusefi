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
#define CBW_LUN_RESERVED_MASK           0b11110000
#define CBW_CMD_LEN_RESERVED_MASK       0b11000000

// bit 7 of bmCBWFlags: data phase direction, set = Device-to-Host (IN)
#define CBW_FLAGS_DIRECTION_IN          0b10000000

#define CSW_STATUS_PASSED               0x00
#define CSW_STATUS_FAILED               0x01
#define CSW_STATUS_PHASE_ERROR          0x02

#define MSD_SETUP_WORD(setup, index) (uint16_t)(((uint16_t)setup[index+1] << 8)\
                                                | (setup[index] & 0x00FF))

#define MSD_SETUP_VALUE(setup)  MSD_SETUP_WORD(setup, 2)
#define MSD_SETUP_INDEX(setup)  MSD_SETUP_WORD(setup, 4)
#define MSD_SETUP_LENGTH(setup) MSD_SETUP_WORD(setup, 6)

static uint32_t scsi_transport_transmit(const SCSITransport *transport,
										const uint8_t *data, size_t len) {
	usb_scsi_transport_handler_t *trp = reinterpret_cast<usb_scsi_transport_handler_t*>(transport->handler);
	msg_t status = usbTransmit(trp->usbp, trp->ep, data, len);
	if (MSG_OK == status)
		return len;
	else
		return 0;
}

static uint32_t scsi_transport_transmit_start(const SCSITransport *transport,
											  const uint8_t *data, size_t len) {
	usb_scsi_transport_handler_t *trp = reinterpret_cast<usb_scsi_transport_handler_t*>(transport->handler);
	msg_t status = usbTransmitStart(trp->usbp, trp->ep, data, len);
	if (MSG_OK == status)
		return len;
	else
		return 0;
}

static uint32_t scsi_transport_transmit_wait(const SCSITransport *transport) {
	usb_scsi_transport_handler_t *trp = reinterpret_cast<usb_scsi_transport_handler_t*>(transport->handler);
	msg_t status = usbTransmitWait(trp->usbp, trp->ep);
	if (MSG_OK == status)
		return 0;
	else
		return 1;
}

static uint32_t scsi_transport_receive(const SCSITransport *transport,
										uint8_t *data, size_t len) {
	usb_scsi_transport_handler_t *trp = reinterpret_cast<usb_scsi_transport_handler_t*>(transport->handler);
	msg_t status = usbReceive(trp->usbp, trp->ep, data, len);
	if (MSG_RESET != status)
		return status;
	else
		return 0;
}

MassStorageController::MassStorageController(USBDriver* usb)
	: ThreadController("MSD", MSD_THD_PRIO)
	, m_usb(usb) {
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

		if (!cbwValid(m_cbw, status) || !cbwMeaningful(m_cbw)) {
			// BOT spec 6.6.1: an invalid CBW is answered by stalling both bulk endpoints,
			// no CSW is sent. The host recovers with Reset Recovery (Bulk-Only Mass Storage
			// Reset plus Clear Feature HALT on both endpoints, serviced at interrupt level),
			// after which we are back at usbReceive() above waiting for a fresh CBW.
			// Silently ignoring a bad CBW instead would leave the host waiting forever for
			// a CSW - which presents as a frozen file manager on the PC.
			stallTransport(/*stallIn =*/ true, /*stallOut =*/ true);
			continue;
		}

		// Get the LUN from the incoming CBW packet, range-checked by cbwMeaningful()
		uint8_t target_lun = m_cbw.lun & 0x0F; // Mask out reserved bits
		auto target = &m_luns[target_lun].target;

		// lib_scsi sets residue on a short data phase but never clears it between
		// commands, so reset it here or a stale value leaks into every following CSW
		target->residue = 0;

		if (SCSI_SUCCESS == scsiExecCmd(target, m_cbw.cmd_data)) {
			sendCsw(CSW_STATUS_PASSED, scsiResidue(target));
		} else {
			// The command failed and the SCSI layer fails before performing the data phase
			// (LBA out of range, unhandled command, unit attention). If the CBW announced a
			// data phase the host is now about to transfer data, not read a CSW - so per BOT
			// spec 6.7.2/6.7.3 we must stall the endpoint the host will use for that data,
			// otherwise host and device desynchronize and the transport locks up. The host
			// clears the stall and then picks up the CSW queued below - queueing behind the
			// stall is safe since clearing a halt only clears the endpoint STALL flag.
			if (m_cbw.data_len > 0) {
				bool dataIn = (m_cbw.flags & CBW_FLAGS_DIRECTION_IN) != 0;
				stallTransport(/*stallIn =*/ dataIn, /*stallOut =*/ !dataIn);
			}
			// none of the announced data phase was performed
			sendCsw(CSW_STATUS_FAILED, m_cbw.data_len);
		}
	}
}

void MassStorageController::stallTransport(bool stallIn, bool stallOut) {
	osalSysLock();
	// USB may have been reset/disconnected since the command was received - endpoint
	// records are gone in that case and there is nothing to stall
	if (usbGetDriverStateI(m_usb) == USB_ACTIVE) {
		if (stallIn) {
			usbStallTransmitI(m_usb, USB_MSD_DATA_EP);
		}
		if (stallOut) {
			usbStallReceiveI(m_usb, USB_MSD_DATA_EP);
		}
	}
	osalSysUnlock();
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

void MassStorageController::sendCsw(uint8_t status, uint32_t residue) {
	m_csw.signature = MSD_CSW_SIGNATURE;
	m_csw.data_residue = residue;
	m_csw.tag = m_cbw.tag;
	m_csw.status = status;

	usbTransmit(m_usb, USB_MSD_DATA_EP, (uint8_t*)&m_csw, sizeof(m_csw));
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
		This class-specific request shall ready the device for the next CBW from the host. */
		/* Do any special reset code here. */
		/* The device shall NAK the status stage of the device request until
		* the Bulk-Only Mass Storage Reset is complete.
		* NAK EP1 in and out */
		// usbp->otg->ie[1].DIEPCTL = DIEPCTL_SNAK;
		// usbp->otg->oe[1].DOEPCTL = DOEPCTL_SNAK;
		/* response to this request using EP0 */
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
