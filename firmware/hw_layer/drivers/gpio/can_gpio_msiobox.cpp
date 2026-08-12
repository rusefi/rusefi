/**
 * can_gpio_msiobox.cpp
 *
 * GPIO chip driver for the MegaSquirt I/O box ("IOBox"): a CAN-attached
 * expander that provides additional discrete/PWM outputs, ADC inputs,
 * switch inputs and wheel-speed (VSS/tach) capture.
 *
 * The driver implements two roles at once:
 *  - GpioChip:    exposes the IOBox pins to the rest of the firmware as if
 *                 they were ordinary GPIOs (writePad/readPad/setPadPWM).
 *  - CanListener: drives the CAN protocol state machine, sends commands to
 *                 the device and decodes the telemetry frames it broadcasts.
 *
 * Current status:
 *  - discrete output works
 *  - PWM output works
 *
 * see also iobox.yaml
 *
 * TODO list:
 *  - implement input reading
 *  - support PWM out mode
 *  - support VSS capture
 */

#include "pch.h"
#include "can_gpio_msiobox.h"

#if EFI_CAN_GPIO

#include "gpio/gpio_ext.h"
#include "can_listener.h"
#include "can_msg_tx.h"
#include <rusefi/endian.h>

/*==========================================================================*/
/* Driver local definitions.												*/
/*==========================================================================*/
#define DRIVER_NAME					"IOBox"

/*==========================================================================*/
/* Driver local definitions.												*/
/*==========================================================================*/

/* HW capabilities exported outside */
/* Actualy only 7 outputs, add one for aligment */
#define MSIOBOX_OUTPUTS			8
/* first 4 capable to capture wheel speed of other frequency signal:
 * then goes 3 switch inputs.
 * SW2, SW3 are shared with VSS3, VSS4. See tachin_mask */
#define MSIOBOX_INPUTS			8
/* total */
#define MSIOBOX_SIGNALS			(MSIOBOX_OUTPUTS + MSIOBOX_INPUTS)

/* Actual HW capabilities */
/* On/Off or PWM mode */
#define MSIOBOX_OUT_COUNT		7
/* ADC input count */
#define MSIOBOX_ADC_IN_COUNT	7
/* Speed in sensors */
#define MSIOBOX_TACH_IN_COUNT	4

/* timeouts for different states */
#define MSIOBOX_PING_TIMEOUT	100
#define MSIOBOX_RESTART_TIMEOUT	1000

/*==========================================================================*/
/* Driver exported variables.												*/
/*==========================================================================*/

/*==========================================================================*/
/* MegaSquirt I/O box protocol definitions.									*/
/*==========================================================================*/
#define CAN_IOBOX_BASE1         0x200
#define CAN_IOBOX_BASE2         0x220
#define CAN_IOBOX_BASE3         0x240

/* Packets from ECU to device */
#define CAN_IOBOX_PING          0x00
#define CAN_IOBOX_CONFIG        0x01
#define CAN_IOBOX_SET_PWM(n)    (0x02 + ((n) & 0x03))
#define CAN_IOBOX_LAST_IN       0x05

/* Packets from device to ECU */
#define CAN_IOBOX_WHOAMI        0x08
#define CAN_IOBOX_ADC14         0x09
#define CAN_IOBOX_ADC57         0x0A
#define CAN_IOBOX_TACH1			0x0B
/* ... */
#define CAN_IOBOX_TACH4			0x0E

struct pwm_settings {
    beuint16_t on;
    beuint16_t off;
} __attribute__ ((packed));

/* Base + 0x00 */
/* "Are you there?" packet with zero payload */
struct iobox_ping {
	/* empty */
} __attribute__((packed));

/* Base + 0x01 */
struct iobox_cfg {
    uint8_t pwm_mask;   // 0 - On/Off, 1 - PWM
    uint8_t pad0;
    uint8_t tachin_mask;
    uint8_t pad1;
    uint8_t adc_broadcast_interval;     // mS
    uint8_t tach_broadcast_interval;    // mS
    uint8_t pad2[2];
} __attribute__((packed));

/* Base + 0x02, 0x03, 0x04 */
struct iobox_pwm {
    pwm_settings ch[2];
} __attribute__ ((packed));

static_assert(sizeof(iobox_pwm) == 8);

/* Base + 0x05 */
struct iobox_pwm_last {
    pwm_settings ch[1];
    uint8_t out_state;
} __attribute__ ((packed));

static_assert(sizeof(iobox_pwm_last) == 5);

/* Base + 0x08 */
struct iobox_whoami {
    uint8_t version;
    uint8_t pad[3];
    beuint16_t pwm_period;      // PWM clock periods in 0.01 uS
    beuint16_t tachin_period;   // Tach-in clock periods in 0.01 uS
} __attribute__((packed));

static_assert(sizeof(iobox_whoami) == 8);

/* Base + 0x09 */
struct iobox_adc14 {
    beuint16_t adc[4];
} __attribute__((packed));

static_assert(sizeof(iobox_adc14) == 8);

/* Base + 0x0A */
struct iobox_adc57 {
    uint8_t inputs;
    uint8_t pad;
    beuint16_t adc[3];
} __attribute__((packed));

static_assert(sizeof(iobox_adc57) == 8);

/* Base + 0x0B..0x0E */
struct iobox_tach {
    beuint32_t period;
    beuint16_t n_teeth;
    beuint16_t total_tooth;
} __attribute__((packed));

static_assert(sizeof(iobox_tach) == 8);

/*==========================================================================*/
/* Driver local variables and types.										*/
/*==========================================================================*/

/* Connection state machine for a single IOBox device.
 * Flow: WAIT_INIT -> (ping) -> WAIT_WHOAMI -> (whoami received) -> READY.
 * On timeout we go to FAILED and later retry from WAIT_INIT. */
typedef enum {
	MSIOBOX_DISABLED = 0,	// device not used / not configured
	MSIOBOX_WAIT_INIT,		// need to send the initial ping
	MSIOBOX_WAIT_WHOAMI, // 2	// ping sent, waiting for the whoami reply
	MSIOBOX_READY,			// device answered, normal operation
	MSIOBOX_FAILED // 4		// communication lost, waiting before retry
} msiobox_state;

class MsIoBox final : public GpioChip, public CanListener {
	/* gpio stuff */
	int writePad(size_t pin, int value) override;
	int readPad(size_t pin) override;
	int setPadPWM(size_t pin, float frequency, float duty) override;
	brain_pin_diag_e getDiag(size_t pin) override;

public:
	MsIoBox();
	MsIoBox(uint32_t bus, uint32_t base, uint16_t period);

	void printState() {
	  efiPrintf("IO state: %d", (int)state);
	  efiPrintf("pwmBaseFreq: %d", (int)pwmBaseFreq);
	}

	CanListener* request() override;
	bool acceptFrame(const size_t busIndex, const CANRxFrame& frame) const override;

	int init() override;
	int config(uint32_t bus, uint32_t base, uint16_t period);

#if 0
	/* pin argument is pin number within gpio chip, not a global number */
	int writePad(size_t pin, int value) override {
		state[pin] = value;
		return 0;
	}
#endif

protected:
	void decodeFrame(const CANRxFrame& frame, efitick_t nowNt) override;

private:
	int ping();
	int setup();
	int update();
	void checkState();

	/* PWM output helpers: convert frequency/duty into device on/off clock counts */
	void CalcOnOffPeriod(int ch, pwm_settings &pwm);

	/* Output states */
	uint8_t OutMode;	// on/off (0) vs pwm (1) bitfield
	uint8_t OutVal;		// for on/off outputs
	struct {
		float frequency;
		float duty;
	} OutPwm[MSIOBOX_OUT_COUNT];
	/* ADC inputs */
	uint16_t AdcValue[MSIOBOX_ADC_IN_COUNT];
	/* Input mode */
	uint8_t InMode;		//Tach-in config bitfield
	/* Logical inputs */
	uint8_t InVal;
	/* Wheel speed */
	struct {
		uint32_t period;
		uint16_t teeths;
		uint16_t totalTooth;
	} Tach[MSIOBOX_TACH_IN_COUNT];
	/* Can settings */
	uint32_t m_bus;
	uint32_t m_base;
	uint32_t m_period;
	/* IOBox timebase */
	/* PWM clock period in 0.01 uS units, default is 5000 */
	uint32_t pwmBaseFreq = 1000 * 1000 * 100 / 5000;	/* Hz */
	/* Tach-in clock period in 0.01 uS units, default is 66 */
	uint32_t tachinBaseFreq = 1000 * 1000 * 100 / 66;	/* Hz */
	/* Misc */
	uint8_t version;
	/* Flags */
	bool needUpdate;
	bool needUpdateConfig;

	/* */
	msiobox_state state;
	Timer stateTimer;
};

MsIoBox::MsIoBox()
	: CanListener(0), m_bus(0), m_base(0), m_period(20) {
}

MsIoBox::MsIoBox(uint32_t bus, uint32_t base, uint16_t period)
	: CanListener(0), m_bus(bus), m_base(base), m_period(period) {
	/* init state */
	state = MSIOBOX_WAIT_INIT;
	stateTimer.reset();
}

/* GpioChip init hook, called once when the chip is registered. */
int MsIoBox::init()
{
	/* TODO: register can listener here */
	return 0;
}

/* (Re)configure the CAN binding for this device and force a reconnect.
 * bus    - CAN bus index to talk on
 * base   - base CAN id of the device (0x200 / 0x220 / 0x240)
 * period - broadcast/refresh period in milliseconds */
int MsIoBox::config(uint32_t bus, uint32_t base, uint16_t period)
{
	/* TODO: sanity checks? */
	m_bus = bus;
	m_base = base;
	m_period = period;

	/* Force init */
	state = MSIOBOX_WAIT_INIT;
	stateTimer.reset();

	/* TODO: */
	//registerCanListener(this);

	return 0;
}

/* Filter: decide whether an incoming CAN frame belongs to this device.
 * Only the frames the IOBox emits (base + 8 .. base + 14) are accepted. */
bool MsIoBox::acceptFrame(const size_t busIndex, const CANRxFrame& frame) const {
	/* TODO: check busIndex! */
	UNUSED(busIndex);

	/* 11 bit only */
	if (CAN_ISX(frame)) {
		return false;
	}

	uint32_t id = CAN_ID(frame);

	/* packets with ID (base + 0) to (base + 5) are received by MSIOBox
	 * (base + 8) to (base + 14) are emited by MSIOBox */
	if ((id >= m_base + 8) && (id <= m_base + 14)) {
		return true;
	}

	return false;
}

/* Ping iobox */
int MsIoBox::ping() {
	CanTxTyped<iobox_ping> frame(CanCategory::CAN_IOBOX, m_base + CAN_IOBOX_PING, false, 0);

	return 0;
}

/* Send init settings */
int MsIoBox::setup() {
	CanTxTyped<iobox_cfg> cfg(CanCategory::CAN_IOBOX, m_base + CAN_IOBOX_CONFIG, false, 0);

	cfg->pwm_mask = OutMode;
	cfg->tachin_mask = InMode;
	cfg->adc_broadcast_interval = m_period;
	cfg->tach_broadcast_interval = m_period;

	return 0;
}

/* Fill a pwm_settings structure for the given output channel.
 * The device counts time in its own clock ticks (pwmBaseFreq), so the
 * requested frequency/duty is converted into on/off tick counts here. */
void MsIoBox::CalcOnOffPeriod(int channel, pwm_settings &pwm)
{
	/* channel is in on/off mode (not PWM) - nothing to drive */
	if ((OutMode & BIT(channel)) == 0) {
		pwm.on = pwm.off = 0;
		return;
	}

	/* total period in device clock ticks, rounded to nearest */
	int period = (pwmBaseFreq + (OutPwm[channel].frequency / 2)) / OutPwm[channel].frequency;

	/* split the period into high (on) and low (off) parts by duty cycle */
	pwm.on = period * OutPwm[channel].duty;
	pwm.off = period - pwm.on;
}

/* Send current gpio and pwm states */
int MsIoBox::update() {
	/* TODO: protect against OutPwm/OutVal change while we are here */

	/* PWM1 .. PWM6 */
	for (size_t i = 0; i < 3; i++) {
		/* sent only if PWMs is in use */
		if ((OutMode & (BIT(i) | BIT(i + 1))) == 0)
			continue;

		CanTxTyped<iobox_pwm> pwm(CanCategory::CAN_IOBOX, m_base + CAN_IOBOX_SET_PWM(i), false, 0);
		for (size_t j = 0; j < 2; j++) {
			CalcOnOffPeriod(i + j, pwm->ch[j]);
		}
	}

	/* PWM7 periods and on/off outputs bitfield - sent always */
	{
		CanTxTyped<iobox_pwm_last> pwm(CanCategory::CAN_IOBOX, m_base + CAN_IOBOX_SET_PWM(3), false, 0);

		CalcOnOffPeriod(MSIOBOX_OUT_COUNT - 1, pwm->ch[0]);

		pwm->out_state = OutVal;
	}

	return 0;
}

/* Decode a telemetry frame received from the device.
 * Which payloads are valid depends on the current connection state:
 * in WAIT_WHOAMI we only expect the whoami packet, in READY we expect
 * ADC/input/tach broadcasts. */
void MsIoBox::decodeFrame(const CANRxFrame& frame, efitick_t) {
	uint32_t id = CAN_ID(frame);
	/* packet type is encoded as an offset from the device base id */
	uint32_t offset = id - m_base;
	bool handled = true;

	if (state == MSIOBOX_READY) {
		if (offset == CAN_IOBOX_ADC14) {
			auto data = reinterpret_cast<const iobox_adc14*>(&frame.data8[0]);

			for (size_t i = 0; i < 4; i++) {
				AdcValue[i] = data->adc[i];
			}
		} else if (offset == CAN_IOBOX_ADC57) {
			auto data = reinterpret_cast<const iobox_adc57*>(&frame.data8[0]);

			InVal = data->inputs;
			for (size_t i = 0; i < 3; i++) {
				AdcValue[i + 4] = data->adc[i];
			}
		} else if ((offset >= CAN_IOBOX_TACH1) && (offset <= CAN_IOBOX_TACH4)) {
			size_t i = offset - CAN_IOBOX_TACH1;
			auto data = reinterpret_cast<const iobox_tach*>(&frame.data8[0]);

			/* TODO: should be atomic, add lock here? */
			/* TODO: accumulate totalTooth? */
			Tach[i].period = data->period;
			Tach[i].teeths = data->n_teeth;
			Tach[i].totalTooth = data->total_tooth;
		} else {
			handled = false;
		}
	} else if (state == MSIOBOX_WAIT_WHOAMI) {
		if (offset == CAN_IOBOX_WHOAMI) {
			auto data = reinterpret_cast<const iobox_whoami*>(&frame.data8[0]);

			version = data->version;
			/* convert from 0.01 uS units to Hz */
			pwmBaseFreq = 1000 * 1000 * 100 / data->pwm_period;
			tachinBaseFreq = 1000 * 1000 * 100 / data->tachin_period;

			/* apply settings and set sync output states */
			setup();
			update();

			/* now we are ready */
			state = MSIOBOX_READY;
			stateTimer.reset();
		} else {
			handled = false;
		}
		/* ignore everything else */
	} else {
		handled = false;
	}

	if (handled) {
		/* TODO: check that we receive EVERY expected packed? */
		stateTimer.reset();
	}
}

/* gpio chip stuff */
/* Set a discrete (on/off) output. Switches the pin out of PWM mode and
 * just records the new state; the actual CAN frame is sent from update(). */
int MsIoBox::writePad(size_t pin, int value) {
	if (pin >= MSIOBOX_OUTPUTS)
		return -1;

	uint8_t OutModeNew = OutMode & (~BIT(pin));
	uint8_t OutValNew = OutVal;
	if (value) {
		OutValNew |= BIT(pin);
	} else {
		OutValNew &= ~BIT(pin);
	}

	if (OutValNew != OutVal) {
		OutVal = OutValNew;
		needUpdate = true;
	}
	if (OutModeNew != OutMode) {
		OutMode = OutModeNew;
		needUpdateConfig = true;
	}

	return 0;
}

/* Read a logical (switch) input. Input pins are numbered after the outputs,
 * so the output count is subtracted to get the input index. */
int MsIoBox::readPad(size_t pin) {
	if ((pin < MSIOBOX_OUTPUTS) || (pin >= MSIOBOX_SIGNALS))
		return -1;

	pin -= MSIOBOX_OUTPUTS;

	if (InMode & BIT(pin)) {
		/* pin is configured for VSS */
		return -1;
	}

	return !!(InVal & BIT(pin));
}

int MsIoBox::setPadPWM(size_t pin, float frequency, float duty)
{
	if (pin >= MSIOBOX_OUT_COUNT)
		return -1;

	/* TODO: validate frequency? Validate duty? */

	/* Just save values.
	 * Do calculation in update() as at this point we may not receive
	 * iobox_whoami packet with pwmPeriodNs */
	OutPwm[pin].frequency = frequency;
	OutPwm[pin].duty = duty;

	if ((OutMode & BIT(pin)) == 0) {
		OutMode |= BIT(pin);
		needUpdateConfig = true;
	}

	/* TODO: chech if updated? */
	needUpdate = true;

	return 0;
}

/* Report pin diagnostics: OK only while the device is READY and we have
 * received fresh data recently, otherwise treat it as driver-off. */
brain_pin_diag_e MsIoBox::getDiag(size_t pin)
{
	if (pin >= MSIOBOX_SIGNALS)
		return PIN_UNKNOWN;

	if ((state == MSIOBOX_READY) && (!stateTimer.hasElapsedMs(m_period * 3)))
		return PIN_OK;

	/* find better state  */
	return PIN_DRIVER_OFF;
}

/* Periodic state-machine tick, driven from request().
 * Handles connection bring-up, timeouts and pushing pending config/output
 * updates to the device while it is READY. */
void MsIoBox::checkState(void)
{
	switch (state) {
	case MSIOBOX_DISABLED:
		/* nop */
		break;
	case MSIOBOX_WAIT_INIT:
		ping();

		state = MSIOBOX_WAIT_WHOAMI;
		stateTimer.reset();

		break;
	case MSIOBOX_WAIT_WHOAMI:
		if (stateTimer.hasElapsedMs(MSIOBOX_PING_TIMEOUT)) {
			state = MSIOBOX_FAILED;
			stateTimer.reset();
		}
		break;
	case MSIOBOX_READY:
		if (stateTimer.hasElapsedMs(m_period * 3)) {
			state = MSIOBOX_FAILED;
			stateTimer.reset();
		} else {
			if (needUpdateConfig) {
				setup();
				needUpdateConfig = false;
				/* Force update */
				needUpdate = true;
			}
			if (needUpdate) {
				update();
			}
		}
		break;
	case MSIOBOX_FAILED:
		if (stateTimer.hasElapsedMs(MSIOBOX_RESTART_TIMEOUT)) {
			state = MSIOBOX_WAIT_INIT;
			stateTimer.reset();
		}
		break;
	}
}

/* Called by the CAN framework to fetch the next listener; we piggy-back on
 * it to advance our own state machine on every poll. */
CanListener* MsIoBox::request(void) {
	checkState();

	/* return next */
	return CanListener::request();
}

static MsIoBox instance[BOARD_CAN_GPIO_COUNT];

/* Entry point: bring up the IOBox driver(s) according to configuration.
 * Does nothing if the IOBox is disabled in the tune. Computes the device
 * CAN base id from the selected id slot, registers a CAN listener and the
 * GPIO chip, and adds a "msioinfo" console command for diagnostics. */
void initCanGpioMsiobox() {
	if (engineConfiguration->msIoBox0.id == MsIoBoxId::OFF) {
		return;
	}

	// MSIOBOX_0_OUT_1
	for (size_t i = 0; i < BOARD_CAN_GPIO_COUNT; i++) {
		uint32_t can_id = CAN_IOBOX_BASE1 + 0x20 * (static_cast<uint32_t>(engineConfiguration->msIoBox0.id) - static_cast<uint32_t>(MsIoBoxId::ID200));

		/* TODO: pick can bus and refresh rate from settings */
		if (instance[i].config(0, can_id, 20) == 0) {
			registerCanListener(instance[i]);
			/* register */
			int ret = gpiochip_register(Gpio::MSIOBOX_0_OUT_1, DRIVER_NAME, instance[i], MSIOBOX_SIGNALS);
			if (ret < 0) {
			  // no error handling, not returning error code
				return;
			}
		}
	}

	addConsoleAction("msioinfo", [](){
	  for (size_t i = 0; i < BOARD_CAN_GPIO_COUNT; i++) {
	    instance[i].printState();
	  }
  });
}
#endif // EFI_CAN_GPIO
