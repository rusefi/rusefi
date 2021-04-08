#include "mass_storage_device.h"

//#define MSD_REQ_RESET                   0xFF
//#define MSD_REQ_GET_MAX_LUN             0xFE

#define MSD_CBW_SIGNATURE               0x43425355
#define MSD_CSW_SIGNATURE               0x53425355

#define CBW_FLAGS_RESERVED_MASK         0b01111111
#define CBW_LUN_RESERVED_MASK           0b11110000
#define CBW_CMD_LEN_RESERVED_MASK       0b11000000

#define CSW_STATUS_PASSED               0x00
#define CSW_STATUS_FAILED               0x01
#define CSW_STATUS_PHASE_ERROR          0x02

static uint32_t scsi_transport_transmit(const SCSITransport *transport,
										const uint8_t *data, size_t len) {
	usb_scsi_transport_handler_t *trp = reinterpret_cast<usb_scsi_transport_handler_t*>(transport->handler);
	msg_t status = usbTransmit(trp->usbp, trp->ep, data, len);
	if (MSG_OK == status)
		return len;
	else
		return 0;
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

template <size_t TLunCount>
MassStorageController<TLunCount>::MassStorageController(USBDriver* usb)
	: ThreadController("MSD", MSD_THD_PRIO)
	, m_usb(usb) {
	for (size_t i = 0; i < TLunCount; i++) {
		scsiObjectInit(&m_luns[i].target);
	}

	m_scsiTransportHandler.usbp = usb;
	m_scsiTransportHandler.ep = USB_MSD_DATA_EP;
	m_scsiTransport.handler  = &m_scsiTransportHandler;
	m_scsiTransport.transmit = scsi_transport_transmit;
	m_scsiTransport.receive  = scsi_transport_receive;
}

template <size_t TLunCount>
void MassStorageController<TLunCount>::ThreadTask() {
	while (!chThdShouldTerminateX()) {
		const msg_t status = usbReceive(m_usb, USB_MSD_DATA_EP, (uint8_t*)&m_cbw, sizeof(m_cbw));

		if (MSG_RESET == status) {
			chThdSleepMilliseconds(50);
			continue;
		}

		if (cbwValid(m_cbw, status) && cbwMeaningful(m_cbw)) {
			chibios_rt::MutexLocker lock(m_lunMutex);

			auto target = &m_luns[0].target;
			if (SCSI_SUCCESS == scsiExecCmd(target, m_cbw.cmd_data)) {
				sendCsw(CSW_STATUS_PASSED, 0);
			} else {
				sendCsw(CSW_STATUS_FAILED, scsiResidue(target));
			}
		} else {
			// ignore incorrect CBW
		}
	}
}

template <size_t TLunCount>
/*static*/ bool MassStorageController<TLunCount>::cbwValid(const msd_cbw_t& cbw, msg_t recvd) {
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

template <size_t TLunCount>
/*static*/ bool MassStorageController<TLunCount>::cbwMeaningful(const msd_cbw_t& cbw) {
	if ((cbw.cmd_len & CBW_CMD_LEN_RESERVED_MASK) != 0) {
		return false;
	}

	if ((cbw.flags & CBW_FLAGS_RESERVED_MASK) != 0) {
		return false;
	}

	if (cbw.lun >= TLunCount) {
		return false;
	}

	return true;
}

template <size_t TLunCount>
void MassStorageController<TLunCount>::sendCsw(uint8_t status, uint32_t residue) {
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

template <size_t TLunCount>
void MassStorageController<TLunCount>::attachLun(uint8_t lunIndex,
						BaseBlockDevice *blkdev, uint8_t *blkbuf,
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
	lun.config.blkdev = blkdev;
	lun.config.transport = &m_scsiTransport;

	scsiStart(&lun.target, &lun.config);
}
