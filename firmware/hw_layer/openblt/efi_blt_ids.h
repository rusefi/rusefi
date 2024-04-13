// file efi_blt_ids.h

#ifndef BOOT_COM_CAN_BAUDRATE
/** \brief Configure the desired CAN baudrate. */
#define BOOT_COM_CAN_BAUDRATE           (500000)
#endif

#ifndef BOOT_COM_CAN_RX_MSG_ID
// PC or Android device doing the programming
/** \brief Configure CAN message ID host->target. */
#define BOOT_COM_CAN_RX_MSG_ID          (0x667 /*| 0x80000000*/)
#endif

#ifndef BOOT_COM_CAN_TX_MSG_ID
// ECU talks this CAN is
/** \brief Configure CAN message ID target->host. */
#define BOOT_COM_CAN_TX_MSG_ID          (0x7E1 /*| 0x80000000*/)
#endif
