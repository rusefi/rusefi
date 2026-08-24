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

	// Bulk-Only Mass Storage Reset handling, called from the EP0 setup hook in ISR context
	void onBulkOnlyResetIsr(USBDriver *usbp);

	// called by the SCSI transport (on the MSD thread) when a data-phase USB transfer
	// timed out, meaning the host abandoned the command without a Bulk-Only Reset
	void onDataPhaseTimeout();

	// called by the SCSI transport with the number of data-phase bytes actually moved,
	// so ThreadTask can detect a command that owed the host data but sent none (BOT
	// case 4/5, e.g. Read Capacity on a medium-less LUN). lib_scsi's scsiResidue()
	// cannot tell: it is only written on short transfers and never reset per command.
	void onDataPhaseBytes(uint32_t bytes) {
		m_dataPhaseBytes = m_dataPhaseBytes + bytes;
	}

	// polled by the SCSI transport so an abandoned command stops touching USB
	bool isCommandAbandoned() const {
		return m_botResetPending || m_dataPhaseTimedOut;
	}

protected:
	void ThreadTask() override;

private:
	static bool cbwValid(const msd_cbw_t& cbw, msg_t status);
	static bool cbwMeaningful(const msd_cbw_t& cbw);

	// returns true if the host actually read the CSW (false: timeout/reset)
	bool sendCsw(uint8_t status, uint32_t residue);

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

	// Set by onBulkOnlyResetIsr(), tells ThreadTask to abandon the in-flight command
	// without sending its CSW. Cleared when the next CBW arrives.
	volatile bool m_botResetPending = false;

	// Set by onDataPhaseTimeout() when the host stops moving a data phase without ever
	// sending a Bulk-Only Reset (Windows usbstor cancels its URBs host-side, the bus
	// stays active and nothing wakes the endpoint wait - observed wedging the MSD
	// thread for 40+ minutes and causing recurring CDC disconnects as usbstor reset
	// the whole composite device every ~20s). Cleared when the next CBW arrives.
	volatile bool m_dataPhaseTimedOut = false;
	volatile uint32_t m_timeoutCount = 0;
	// Of those timeouts, how many the host closed out by collecting the phase-error
	// CSW after the data-endpoint stall - i.e. recoveries that avoided a composite
	// device reset (and its CDC console drop).
	volatile uint32_t m_timeoutCswDeliveredCount = 0;

	// Data-phase bytes moved by the current command, reset before each scsiExecCmd.
	// Zero on a command that declared a data-IN phase means the host's data URB is
	// still armed and a straight CSW would babble into it - close it with a ZLP
	// (short packet) so the URB completes without a stall + EP0 clear-halt that
	// would disturb the CDC endpoints of the composite device.
	volatile uint32_t m_dataPhaseBytes = 0;
	volatile uint32_t m_noDataZlpCount = 0;

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
