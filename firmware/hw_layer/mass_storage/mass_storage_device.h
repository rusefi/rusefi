/**
 * @file	mass_storage_device.h
 *
 * @date April 9, 2021
 * @author Matthew Kennedy, (c) 2021
 *
 * This file implements a USB mass storage device with multiple LUNs, so multiple drives can be mounted at once.
 */

#include "hal_usb_msd.h"
#include "thread_controller.h"

#if HAL_USE_USB_MSD

class MassStorageController : public ThreadController<USB_MSD_THREAD_WA_SIZE> {
public:
	MassStorageController(USBDriver* usb);

	void attachLun(uint8_t lunIndex, BaseBlockDevice *blkdev, uint8_t *blkbuf, size_t blkbufsize,
					const scsi_inquiry_response_t *inquiry,
					const scsi_unit_serial_number_inquiry_response_t *serialInquiry);

	// console diagnostics, called from another thread ('sdinfo' command)
	void printDiagnostics() const;

protected:
	void ThreadTask() override;

private:
	static bool cbwValid(const msd_cbw_t& cbw, msg_t status);
	static bool cbwMeaningful(const msd_cbw_t& cbw);

	void sendCsw(uint8_t status, uint32_t residue);

	// Diagnostic counters, written only by the MSD thread, read by the console thread.
	// volatile so the console thread sees fresh values while the MSD thread is busy.
	volatile uint32_t m_cmdCount = 0;
	volatile uint32_t m_failedCmdCount = 0;
	volatile uint32_t m_invalidCbwCount = 0;
	volatile uint8_t m_lastFailedOpcode = 0;
	// SCSI opcode being executed right now, -1 when between commands. If the transport
	// wedges (host frozen on a write), 'sdinfo' shows which command never completed.
	volatile int m_busyOpcode = -1;
	volatile systime_t m_busySince = 0;

	usbmsdstate_t m_state;

	USBDriver* const m_usb;

	// temporary storage
	msd_cbw_t m_cbw;
	msd_csw_t m_csw;

	SCSITransport m_scsiTransport;
	usb_scsi_transport_handler_t m_scsiTransportHandler;

	chibios_rt::Mutex m_lunMutex;

	struct LunEntry {
		SCSITarget target;
		SCSITargetConfig config;
	};

	LunEntry m_luns[USB_MSD_LUN_COUNT];
};

#endif // HAL_USE_USB_MSD
