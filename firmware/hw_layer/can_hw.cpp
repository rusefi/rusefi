/**
 * @file	can_hw.cpp
 * @brief	CAN bus low level code
 *
 * todo: this file should be split into two - one for CAN transport level ONLY and
 * another one with actual messages
 *
 * @date Dec 11, 2013
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "main.h"
#include "can_hw.h"
#include "string.h"
#include "obd2.h"

#if EFI_PROD_CODE || defined(__DOXYGEN__)


#endif /* EFI_PROD_CODE */

#if EFI_CAN_SUPPORT || defined(__DOXYGEN__)

#include "pin_repository.h"
#include "mpu_util.h"
#include "engine_state.h"
#include "engine_configuration.h"
#include "vehicle_speed.h"

EXTERN_ENGINE
;

static int canReadCounter = 0;
static int canWriteOk = 0;
static int canWriteNotOk = 0;
static bool isCanEnabled = false;
static LoggingWithStorage logger("CAN driver");
static THD_WORKING_AREA(canTreadStack, UTILITY_THREAD_STACK_SIZE);

/*
 * 500KBaud
 * automatic wakeup
 * automatic recover from abort mode
 * See section 22.7.7 on the STM32 reference manual.
 *
 * speed = 42000000 / (BRP + 1) / (1 + TS1 + 1 + TS2 + 1)
 * 42000000 / 7 / 12 = 500000
 *
 * 29 bit would be CAN_TI0R_EXID (?) but we do not mention it here
 * CAN_TI0R_STID "Standard Identifier or Extended Identifier"? not mentioned as well
 */
static const CANConfig canConfig = {
CAN_MCR_ABOM | CAN_MCR_AWUM | CAN_MCR_TXFP,
CAN_BTR_SJW(0) | CAN_BTR_TS2(1) | CAN_BTR_TS1(8) | CAN_BTR_BRP(6) };

static CANRxFrame rxBuffer;
CANTxFrame txmsg;

static void printPacket(CANRxFrame *rx) {
//	scheduleMsg(&logger, "CAN FMI %x", rx->FMI);
//	scheduleMsg(&logger, "TIME %x", rx->TIME);
	scheduleMsg(&logger, "Got CAN message: SID %x/%x %x %x %x %x %x %x %x %x", rx->SID, rx->DLC, rx->data8[0], rx->data8[1],
			rx->data8[2], rx->data8[3], rx->data8[4], rx->data8[5], rx->data8[6], rx->data8[7]);

	if (rx->SID == CAN_BMW_E46_CLUSTER_STATUS) {
		int odometerKm = 10 * (rx->data8[1] << 8) + rx->data8[0];
		int odometerMi = (int) (odometerKm * 0.621371);
		scheduleMsg(&logger, "GOT odometerKm %d", odometerKm);
		scheduleMsg(&logger, "GOT odometerMi %d", odometerMi);
		int timeValue = (rx->data8[4] << 8) + rx->data8[3];
		scheduleMsg(&logger, "GOT time %d", timeValue);
	}
}

static void setShortValue(CANTxFrame *txmsg, int value, int offset) {
	txmsg->data8[offset] = value;
	txmsg->data8[offset + 1] = value >> 8;
}

void setTxBit(int offset, int index) {
	txmsg.data8[offset] = txmsg.data8[offset] | (1 << index);
}

void commonTxInit(int eid) {
	memset(&txmsg, 0, sizeof(txmsg));
	txmsg.IDE = CAN_IDE_STD;
	txmsg.EID = eid;
	txmsg.RTR = CAN_RTR_DATA;
	txmsg.DLC = 8;
}

/**
 * send CAN message from txmsg buffer
 */
static void sendCanMessage2(int size) {
	CANDriver *device = detectCanDevice(boardConfiguration->canRxPin,
			boardConfiguration->canTxPin);
	if (device == NULL) {
		warning(CUSTOM_ERR_CAN_CONFIGURATION, "CAN configuration issue");
		return;
	}
	txmsg.DLC = size;
	// 1 second timeout
	msg_t result = canTransmit(device, CAN_ANY_MAILBOX, &txmsg, MS2ST(1000));
	if (result == MSG_OK) {
		canWriteOk++;
	} else {
		canWriteNotOk++;
	}
}

/**
 * send CAN message from txmsg buffer, using default packet size
 */
void sendCanMessage() {
	sendCanMessage2(8);
}

static void canDashboardBMW(void) {
	//BMW Dashboard
	commonTxInit(CAN_BMW_E46_SPEED);
	setShortValue(&txmsg, 10 * 8, 1);
	sendCanMessage();

	commonTxInit(CAN_BMW_E46_RPM);
	setShortValue(&txmsg, (int) (getRpmE(engine) * 6.4), 2);
	sendCanMessage();

	commonTxInit(CAN_BMW_E46_DME2);
	setShortValue(&txmsg, (int) ((engine->sensors.clt + 48.373) / 0.75), 1);
	sendCanMessage();
}

static void canMazdaRX8(void) {
	commonTxInit(CAN_MAZDA_RX_STEERING_WARNING);
	// todo: something needs to be set here? see http://rusefi.com/wiki/index.php?title=Vehicle:Mazda_Rx8_2004
	sendCanMessage();

	commonTxInit(CAN_MAZDA_RX_RPM_SPEED);

	float kph = getVehicleSpeed();

	setShortValue(&txmsg, SWAP_UINT16(getRpmE(engine) * 4), 0);
	setShortValue(&txmsg, 0xFFFF, 2);
	setShortValue(&txmsg, SWAP_UINT16((int )(100 * kph + 10000)), 4);
	setShortValue(&txmsg, 0, 6);
	sendCanMessage();

	commonTxInit(CAN_MAZDA_RX_STATUS_1);
	txmsg.data8[0] = 0xFE; //Unknown
	txmsg.data8[1] = 0xFE; //Unknown
	txmsg.data8[2] = 0xFE; //Unknown
	txmsg.data8[3] = 0x34; //DSC OFF in combo with byte 5 Live data only seen 0x34
	txmsg.data8[4] = 0x00; // B01000000; // Brake warning B00001000;  //ABS warning
	txmsg.data8[5] = 0x40; // TCS in combo with byte 3
	txmsg.data8[6] = 0x00; // Unknown
	txmsg.data8[7] = 0x00; // Unused
	sendCanMessage();

	commonTxInit(CAN_MAZDA_RX_STATUS_2);
	txmsg.data8[0] = (uint8_t)(engine->sensors.clt + 69); //temp gauge //~170 is red, ~165 last bar, 152 centre, 90 first bar, 92 second bar
	txmsg.data8[1] = ((int16_t)(engine->engineState.vssEventCounter*(engineConfiguration->vehicleSpeedCoef*0.277*2.58))) & 0xff;
	txmsg.data8[2] = 0x00; // unknown
	txmsg.data8[3] = 0x00; //unknown
	txmsg.data8[4] = 0x01; //Oil Pressure (not really a gauge)
	txmsg.data8[5] = 0x00; //check engine light
	txmsg.data8[6] = 0x00; //Coolant, oil and battery
	if ((getRpmE(engine)>0) && (engine->sensors.vBatt<13)) {
		setTxBit(6, 6); // battery light
	}
	if (engine->sensors.clt > 105) {
		setTxBit(6, 1); // coolant light, 101 - red zone, light means its get too hot
	}
	//oil pressure warning lamp bit is 7
	txmsg.data8[7] = 0x00; //unused
	sendCanMessage();
}

static void canDashboardFiat(void) {
	//Fiat Dashboard
	commonTxInit(CAN_FIAT_MOTOR_INFO);
	setShortValue(&txmsg, (int) (engine->sensors.clt - 40), 3); //Coolant Temp
	setShortValue(&txmsg, getRpmE(engine) / 32, 6); //RPM
	sendCanMessage();
}

static void canDashboardVAG(void) {
	//VAG Dashboard
	commonTxInit(CAN_VAG_RPM);
	setShortValue(&txmsg, getRpmE(engine) * 4, 2); //RPM
	sendCanMessage();

	commonTxInit(CAN_VAG_CLT);
	setShortValue(&txmsg, (int) ((engine->sensors.clt + 48.373) / 0.75), 1); //Coolant Temp
	sendCanMessage();
}

static void canInfoNBCBroadcast(can_nbc_e typeOfNBC) {
	switch (typeOfNBC) {
	case CAN_BUS_NBC_BMW:
		canDashboardBMW();
		break;
	case CAN_BUS_NBC_FIAT:
		canDashboardFiat();
		break;
	case CAN_BUS_NBC_VAG:
		canDashboardVAG();
		break;
	case CAN_BUS_MAZDA_RX8:
		canMazdaRX8();
		break;
	default:
		break;
	}
}

static void canRead(void) {
	CANDriver *device = detectCanDevice(boardConfiguration->canRxPin,
			boardConfiguration->canTxPin);
	if (device == NULL) {
		warning(CUSTOM_ERR_CAN_CONFIGURATION, "CAN configuration issue");
		return;
	}
//	scheduleMsg(&logger, "Waiting for CAN");
	msg_t result = canReceive(device, CAN_ANY_MAILBOX, &rxBuffer, MS2ST(1000));
	if (result == MSG_TIMEOUT) {
		return;
	}

	canReadCounter++;
	printPacket(&rxBuffer);
	obdOnCanPacketRx(&rxBuffer);
}

static void writeStateToCan(void) {
	canInfoNBCBroadcast(engineConfiguration->canNbcType);
}

static msg_t canThread(void *arg) {
	(void)arg;
	chRegSetThreadName("CAN");
	while (true) {
		if (engineConfiguration->canWriteEnabled)
			writeStateToCan();

		if (engineConfiguration->canReadEnabled)
			canRead(); // todo: since this is a blocking operation, do we need a separate thread for 'write'?

		if (engineConfiguration->canSleepPeriod < 10) {
			warning(CUSTOM_OBD_LOW_CAN_PERIOD, "%d too low CAN", engineConfiguration->canSleepPeriod);
			engineConfiguration->canSleepPeriod = 50;
		}

		chThdSleepMilliseconds(engineConfiguration->canSleepPeriod);
	}
#if defined __GNUC__ || defined(__DOXYGEN__)
	return -1;
#endif
}

static void canInfo(void) {
	if (!isCanEnabled) {
		scheduleMsg(&logger, "CAN is not enabled, please enable & restart");
		return;
	}

	scheduleMsg(&logger, "CAN TX %s", hwPortname(boardConfiguration->canTxPin));
	scheduleMsg(&logger, "CAN RX %s", hwPortname(boardConfiguration->canRxPin));
	scheduleMsg(&logger, "type=%d canReadEnabled=%s canWriteEnabled=%s period=%d", engineConfiguration->canNbcType,
			boolToString(engineConfiguration->canReadEnabled), boolToString(engineConfiguration->canWriteEnabled),
			engineConfiguration->canSleepPeriod);

	scheduleMsg(&logger, "CAN rx count %d/tx ok %d/tx not ok %d", canReadCounter, canWriteOk, canWriteNotOk);
}

void setCanType(int type) {
	engineConfiguration->canNbcType = (can_nbc_e)type;
	canInfo();
}

void postCanState(TunerStudioOutputChannels *tsOutputChannels) {
	tsOutputChannels->debugIntField1 = isCanEnabled ? canReadCounter : -1;
	tsOutputChannels->debugIntField2 = isCanEnabled ? canWriteOk : -1;
	tsOutputChannels->debugIntField3 = isCanEnabled ? canWriteNotOk : -1;
}

void enableFrankensoCan(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	boardConfiguration->canTxPin = GPIOB_6;
	boardConfiguration->canRxPin = GPIOB_12;
	engineConfiguration->canReadEnabled = false;
}

void stopCanPins(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	unmarkPin(activeConfiguration.bc.canTxPin);
	unmarkPin(activeConfiguration.bc.canRxPin);
}

void startCanPins(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	efiSetPadMode("CAN TX", boardConfiguration->canTxPin, PAL_MODE_ALTERNATE(EFI_CAN_TX_AF));
	efiSetPadMode("CAN RX", boardConfiguration->canRxPin, PAL_MODE_ALTERNATE(EFI_CAN_RX_AF));
}

void initCan(void) {
	isCanEnabled = (boardConfiguration->canTxPin != GPIO_UNASSIGNED) && (boardConfiguration->canRxPin != GPIO_UNASSIGNED);
	if (isCanEnabled) {
		if (!isValidCanTxPin(boardConfiguration->canTxPin))
			firmwareError(CUSTOM_OBD_70, "invalid CAN TX %s", hwPortname(boardConfiguration->canTxPin));
		if (!isValidCanRxPin(boardConfiguration->canRxPin))
			firmwareError(CUSTOM_OBD_70, "invalid CAN RX %s", hwPortname(boardConfiguration->canRxPin));
	}

	addConsoleAction("caninfo", canInfo);
	if (!isCanEnabled)
		return;

#if STM32_CAN_USE_CAN2 || defined(__DOXYGEN__)
	// CAN1 is required for CAN2
	canStart(&CAND1, &canConfig);
	canStart(&CAND2, &canConfig);
#else
	canStart(&CAND1, &canConfig);
#endif /* STM32_CAN_USE_CAN2 */

	chThdCreateStatic(canTreadStack, sizeof(canTreadStack), NORMALPRIO, (tfunc_t) canThread, NULL);

	startCanPins();

}

#endif /* EFI_CAN_SUPPORT */
