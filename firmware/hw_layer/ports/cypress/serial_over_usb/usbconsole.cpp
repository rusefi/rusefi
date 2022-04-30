#include "pch.h"

#if EFI_USB_SERIAL

#include "pdl_header.h"
#include "usb.h"
#include "UsbDeviceCdcCom.h"

// 10 seconds
#define USB_WRITE_TIMEOUT 10000

// See uart_dma_s
#define USB_FIFO_BUFFER_SIZE (BLOCKING_FACTOR + 30)

// struct needed for async transfer mode
typedef struct {
	// secondary FIFO buffer for async. transfer
	uint8_t buffer[USB_FIFO_BUFFER_SIZE];
	// input FIFO Rx queue
	input_queue_t fifoRxQueue;
} usb_buf_s;

static usb_buf_s usbBuf;


static bool isUsbSerialInitialized = false;

static bool isUsbSerialInitStarted = false;

static thread_reference_t threadrx = NULL;

// called from the USB IRQ handler
static void onUsbDataReceived(uint8_t* pu8Data, uint32_t u32ReceviedSize) {
	osalSysLockFromISR();
 
	// copy the data to the FIFO buffer
	for (int i = 0; i < u32ReceviedSize; i++) {
		if (iqPutI(&usbBuf.fifoRxQueue, *pu8Data++) != Q_OK) {
			break; // todo: ignore overflow?
		}
	}
		
	// tell the reader thread to wake up
#if 0
	if (threadrx != NULL) {
   		osalThreadResumeI(&threadrx, MSG_OK);
   	}
#endif

	osalSysUnlockFromISR();
}

// To use UART driver instead of Serial, we need to imitate "BaseChannel" streaming functionality
static msg_t _putt(void *ip, uint8_t b, sysinterval_t timeout) {
	(void)ip;
	(void)timeout;
	UsbDeviceCdcCom_SendByte(b);
	return MSG_OK;
}
static size_t _writet(void *ip, const uint8_t *bp, size_t n, sysinterval_t timeout) {
	(void)ip;
	(void)timeout;
	UsbDeviceCdcCom_SendBuffer((uint8_t *)bp, n);
	return n;
}
static msg_t _put(void *ip, uint8_t b) {
	(void)ip;
	UsbDeviceCdcCom_SendByte(b);
	return MSG_OK;
}
static size_t _write(void *ip, const uint8_t *bp, size_t n) {
	return _writet(ip, bp, n, USB_WRITE_TIMEOUT);
}
static size_t _readt(void *ip, uint8_t *bp, size_t n, sysinterval_t timeout) {
	size_t numBytesRead;
	//numBytesRead = UsbDeviceCdcCom_ReceiveBuffer(bp, n);

	return (size_t)iqReadTimeout(&usbBuf.fifoRxQueue, bp, n, timeout);
/*
	// if we don't have all bytes immediately
	if (numBytesRead < n) {
		osalSysLock();
		threadrx = chThdGetSelfX();
		osalThreadSuspendTimeoutS(&threadrx, timeout);
		osalSysUnlock();
		numBytesRead += UsbDeviceCdcCom_ReceiveBuffer(bp + numBytesRead, n - numBytesRead);
	}
	return numBytesRead;
*/
}
static msg_t _gett(void *ip, sysinterval_t timeout) {
	(void)ip;
	(void)timeout;
	//msg_t msg = UsbDeviceCdcCom_ReceiveByte();
	uint8_t b;
	if (_readt(ip, &b, 1, timeout) == 1)
		return (msg_t)b;
	return MSG_TIMEOUT;
}
static msg_t _get(void *ip) {
	return _gett(ip, USB_WRITE_TIMEOUT);
}
static size_t _read(void *ip, uint8_t *bp, size_t n) {
	(void)ip;
	return _readt(ip, bp, n, USB_WRITE_TIMEOUT);
}
static msg_t _ctl(void *ip, unsigned int operation, void *arg) {
	return MSG_OK;
}

// This is a "fake" channel for getConsoleChannel() filled with our handlers
static const struct BaseChannelVMT usbChannelVmt = {
  .instance_offset = (size_t)0, .write = _write, .read = _read, .put = _put, .get = _get,
  .putt = _putt, .gett = _gett, .writet = _writet, .readt = _readt, .ctl = _ctl
};

BaseChannel SDU1 = { .vmt = &usbChannelVmt };

 
static void usb_VBus_handler(uint8_t channel) {
	// call it only if the USB driver is already initialized
	if (isUsbSerialInitialized)
		UsbConfig_SwitchMode();
}

/**
 ******************************************************************************
 ** \brief  Main function of PDL
 **
 ** \return uint32_t return value, if needed
 ******************************************************************************/
void usb_serial_start(void) {
	if (isUsbSerialInitStarted)
		return;

	isUsbSerialInitStarted = true;

    UsbConfig_UsbInit();

	// init FIFO queue
	iqObjectInit(&usbBuf.fifoRxQueue, usbBuf.buffer, sizeof(usbBuf.buffer), NULL, NULL);

    UsbDeviceCdcCom_SetReceivedCallback(onUsbDataReceived);

	UsbConfig_SwitchMode();

	// init VBus detector for P60 (INT31_0)
	SetPinFunc_INT31_0(0u);
	_pal_lld_setpadeventhandler(31, ExIntRisingEdge, usb_VBus_handler);


    isUsbSerialInitialized = true;

}

bool is_usb_serial_ready(void) {
	return isUsbSerialInitialized /*&& SDU1.config->usbp->state == USB_ACTIVE*/;
}

#endif /* EFI_USB_SERIAL */
