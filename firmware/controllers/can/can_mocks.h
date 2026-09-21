#pragma once

// This corresponds to ChibiOS/os/hal/ports/STM32/LLD/CANv1/hal_can_lld.h

typedef uint32_t canmbx_t;
typedef int32_t can_msg_t;
typedef int32_t can_sysinterval_t;
typedef int32_t msg_t;

#ifndef MSG_OK
#define MSG_OK ((msg_t)0)
#endif

#ifndef MSG_TIMEOUT
#define MSG_TIMEOUT ((msg_t)-1)
#endif

struct CANDriver {
};

#define CAN_MSG_OK (can_msg_t)0
#define CAN_MSG_TIMEOUT (can_msg_t)-1

typedef struct {
  struct {
    uint8_t                 DLC:4;          /**< @brief Data length.        */
    uint8_t                 RTR:1;          /**< @brief Frame type.         */
    uint8_t                 IDE:1;          /**< @brief Identifier type.    */
  };
  union {
    struct {
      uint32_t              SID:11;         /**< @brief Standard identifier.*/
    };
    struct {
      uint32_t              EID:29;         /**< @brief Extended identifier.*/
    };
  };
  union {
    uint8_t                 data8[8];       /**< @brief Frame data.         */
    uint16_t                data16[4];      /**< @brief Frame data.         */
    uint32_t                data32[2];      /**< @brief Frame data.         */
    uint64_t                data64[1];      /**< @brief Frame data.         */
  };
} CANTxFrame;

// Unfortunately we cannot utilize the common base class for CANTxFrame and CANRxFrame,
// because FMI and TIME fields come first, and we don't want to affect the fields order.
typedef struct {
  struct {
    uint8_t                 FMI;            /**< @brief Filter id.          */
    uint16_t                TIME;           /**< @brief Time stamp.         */
  };
  struct {
    uint8_t                 DLC:4;          /**< @brief Data length.        */
    uint8_t                 RTR:1;          /**< @brief Frame type.         */
    uint8_t                 IDE:1;          /**< @brief Identifier type.    */
  };
  union {
    struct {
      uint32_t              SID:11;         /**< @brief Standard identifier.*/
    };
    struct {
      uint32_t              EID:29;         /**< @brief Extended identifier.*/
    };
  };
  union {
    uint8_t                 data8[8];       /**< @brief Frame data.         */
    uint16_t                data16[4];      /**< @brief Frame data.         */
    uint32_t                data32[2];      /**< @brief Frame data.         */
    uint64_t                data64[1];      /**< @brief Frame data.         */
  };
} CANRxFrame;

#define CAN_IDE_STD                 0           /**< @brief Standard id.    */
#define CAN_IDE_EXT                 1           /**< @brief Extended id.    */

#define CAN_RTR_DATA                0           /**< @brief Data frame.     */
#define CAN_RTR_REMOTE              1           /**< @brief Remote frame.   */

#define CAN_ANY_MAILBOX             0U

// Unit tests install this to model the result and elapsed time of a real CAN
// transmit without requiring ChibiOS's CAN driver.
using can_transmit_mock_t = msg_t (*)(CANDriver* canp, canmbx_t mailbox, CANTxFrame* ctfp, can_sysinterval_t timeout);
inline can_transmit_mock_t canTransmitMock = nullptr;

inline msg_t canTransmit(CANDriver* canp, canmbx_t mailbox, CANTxFrame* ctfp, can_sysinterval_t timeout) {
	return canTransmitMock ? canTransmitMock(canp, mailbox, ctfp, timeout) : MSG_OK;
}
