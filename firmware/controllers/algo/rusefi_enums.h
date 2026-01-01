/**
 * @file	rusefi_enums.h
 * @brief	Fundamental rusEFI enumerable types live here
 *
 * This and few over headers are part of integration API between C/C++ and code generator for memory meta and java code.
 * TODO: move enums which should not be visible outside of the firmware out of 'integration API' headers like this one
 *
 * @note this file should probably not include any other files
 *
 * @date Jan 14, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include <rusefi/true_false.h>
#include "efifeatures.h"
#include "obd_error_codes.h"
#include "engine_types.h"
#include "engine_type_e.h"
// we do not want to start the search for header from current folder so we use brackets here
// https://stackoverflow.com/questions/21593/what-is-the-difference-between-include-filename-and-include-filename
#include <rusefi_hw_enums.h>
#include "rusefi_hw_pin_mode.h"

#define PERCENT_MULT 100.0f
#define PERCENT_DIV 0.01f

/* diagnostic for brain pins
 * can be combination of few bits
 * defined as bit mask
 * PIN_UNKNOWN is reported for pins with no diagnostic support, like on-chip gpio
 */
typedef enum __attribute__ ((__packed__))
{
	PIN_OK = 0,
	PIN_OPEN = 0x01,
	PIN_SHORT_TO_GND = 0x02,
	PIN_SHORT_TO_BAT = 0x04,
	PIN_OVERLOAD =	0x08,
	PIN_DRIVER_OVERTEMP = 0x10,
	PIN_DRIVER_OFF = 0x20,
	PIN_UNKNOWN = 0x80
} brain_pin_diag_e;

// see also PWM_PHASE_MAX_WAVE_PER_PWM
// todo: better names?
enum class TriggerWheel : uint8_t {
	T_PRIMARY = 0,
	T_SECONDARY = 1,
};

typedef enum  __attribute__ ((__packed__)) {
	/**
	 * This mode is useful for troubleshooting and research - events are logged but no effects on phase synchronization
	 */
	VVT_INACTIVE = 0,

	/**
	 * Single tooth on the camshaft anywhere in the 720 degree cycle
	 */
	VVT_SINGLE_TOOTH = 1,
	/**
	 * Toyota 2JZ has three cam tooth. We pick one of these three tooth to synchronize based on the expected angle position of the event
	 */
	VVT_TOYOTA_3_TOOTH = 2,
	/**
	 * Mazda NB2 has three cam tooth. We synchronize based on gap ratio.
	 * @see TT_VVT_MIATA_NB
	 */
	VVT_MIATA_NB = 3,

	VVT_MITSUBISHI_4G69 = 4,

	/**
	 * @see TT_VVT_BOSCH_QUICK_START
	 */
	VVT_BOSCH_QUICK_START = 5,

	/**
	 * 1.8l Toyota 1ZZ-FE https://rusefi.com/forum/viewtopic.php?f=3&t=1735
	 * 4 minus one
	 */
	VVT_TOYOTA_4_1 = 6,

	VVT_FORD_ST170 = 7,

	VVT_BARRA_3_PLUS_1 = 8,

	VVT_NISSAN_VQ = 9,

	/**
	 * 4 equally spaced no way to sync
	 */
	VVT_HONDA_K_INTAKE = 10,

	VVT_NISSAN_MR = 11,

	VVT_MITSUBISHI_3A92 = 12,

	VVT_MAP_V_TWIN = 13,

  VVT_CHRYSLER_PHASER = 14,

	VVT_MAZDA_SKYACTIV = 15,

	/**
	 * 4 plus one
	 */
	VVT_HONDA_K_EXHAUST = 16,

	VVT_UNUSED_17 = 17,
	// also 4G92/93/94
	VVT_MITSUBISHI_4G63 = 18,

	VVT_FORD_COYOTE = 19,

  VVT_MITSUBISHI_6G72 = 20,

  VVT_HONDA_CBR_600 = 21,

  VVT_MAZDA_L = 22,

  VVT_DEV = 23,

  VVT_HR12DDR_IN = 24,

  VVT_CUSTOM_25 = 25,

  VVT_CUSTOM_26 = 26,

  VVT_TOYOTA_3TOOTH_UZ = 27,

  VVT_SUBARU_7TOOTH = 28,

  VVT_CUSTOM_1 = 29,

  VVT_CUSTOM_2 = 30,
} vvt_mode_e;

typedef enum __attribute__ ((__packed__)) {
	DM_NONE = 0,
	DM_HD44780 = 1,
	DM_HD44780_OVER_PCF8574 = 2,


} display_mode_e;

typedef enum  __attribute__ ((__packed__)) {
	TL_AUTO = 0,
	TL_SEMI_AUTO = 1,
	TL_MANUAL = 2,
	TL_HALL = 3,

} tle8888_mode_e;

typedef enum __attribute__ ((__packed__)) {
	DWELL_2MS = 0,
	DWELL_4MS = 1,
	DWELL_8MS = 2,
	DWELL_16MS = 3,
	DWELL_32MS = 4,
	DWELL_64MS = 5,

} mc33810maxDwellTimer_e;

enum class idle_mode_e : uint8_t {
	/**
	 * In auto mode we currently have some pid-like-but-not really PID logic which is trying
	 * to get idle RPM to desired value by dynamically adjusting idle valve position.
	 * TODO: convert to PID
	 */
	IM_AUTO = 0,
	/**
	 * Manual idle control is extremely simple: user just specifies desired idle valve position
	 * which could be adjusted according to current CLT
	 */
	IM_MANUAL = 1,

};

enum class SentEtbType : uint8_t {
	NONE = 0,
	GM_TYPE_1 = 1,
	FORD_TYPE_1 = 2,
	CUSTOM = 3,
};

enum class SentFuelHighPressureType : uint8_t {
	NONE = 0,
	GM_TYPE = 1,
};

enum class SentInput : uint8_t {
	NONE = 0,
	INPUT1 = 1,
	INPUT2 = 2,
	INPUT3 = 3,
	INPUT4 = 4,
	INPUT5 = 5,
	INPUT6 = 6,
	INPUT7 = 7
};

enum class CanGpioType : uint8_t {
	NONE = 0,
	DRT = 1,
	MS = 2,
};

enum class MsIoBoxId : uint8_t {
	OFF = 0,
	ID200 = 1,
	ID220 = 2,
	ID240 = 3
};

enum class MsIoBoxVss : uint8_t {
	OFF = 0,
	VR12 = 1,
	HALL34 = 2,
	ALL1234 = 3
};

enum class UiMode : uint8_t {
	FULL = 0,
	INSTALLATION = 1,
	TUNING = 2,
};

typedef enum __attribute__ ((__packed__)) {
	/**
	 * GND for logical OFF, VCC for logical ON
	 */
	OM_DEFAULT = 0,
	/**
	 * GND for logical ON, VCC for logical OFF
	 */
	OM_INVERTED = 1,
	/**
	 * logical OFF is floating, logical ON is GND
	 */
	OM_OPENDRAIN = 2,
	OM_OPENDRAIN_INVERTED = 3
} pin_output_mode_e;

typedef enum __attribute__ ((__packed__)) {
	PI_DEFAULT = 0,
	PI_PULLUP = 1,
	PI_PULLDOWN = 2,
	/**
	 * bit 2 is used as inverted flag, so skip one here
	 */
	PI_INVERTED_DEFAULT = 4,
	PI_INVERTED_PULLUP = 5,
	PI_INVERTED_PULLDOWN = 6
} pin_input_mode_e;

/**
 * @see getCycleDuration
 * @see getEngineCycle
 */
// todo: better enum name
typedef enum {
	OM_NONE = 0,
	/**
	 * 720 degree engine cycle but trigger is defined using a 360 cycle which is when repeated.
	 * For historical reasons we have a pretty weird approach where one crank trigger revolution is
	 * defined as if it's stretched to 720 degrees. See CRANK_MODE_MULTIPLIER
	 */
	FOUR_STROKE_CRANK_SENSOR = 1,
	/**
	 * 720 degree engine and trigger cycle
	 */
	FOUR_STROKE_CAM_SENSOR = 2,
	/**
	 * 360 degree cycle
	 */
	TWO_STROKE = 3,

	/**
	 * 720 degree engine cycle but trigger is defined using a 180 cycle which is when repeated three more times
	 * In other words, same pattern is repeated on the crank wheel twice.
	 */
	FOUR_STROKE_SYMMETRICAL_CRANK_SENSOR = 4,

	/**
	 * Same pattern repeated three times on crank wheel. Crazy, I know!
	 */
	FOUR_STROKE_THREE_TIMES_CRANK_SENSOR = 5,

	// Same pattern TWELVE TIMES on the crank wheel!
	// This usually means Honda, which often has a 12 tooth crank wheel or 24 tooth cam wheel
	// without a missing tooth, plus a single tooth cam channel to resolve the engine phase.
	FOUR_STROKE_TWELVE_TIMES_CRANK_SENSOR = 6,

	/**
	 * Same pattern repeated six times on crank wheel like 1995 Lamborghini Diablo
	 */
	FOUR_STROKE_SIX_TIMES_CRANK_SENSOR = 7,
} operation_mode_e;

/**
 * @brief Ignition Mode
 */
typedef enum __attribute__ ((__packed__)) {
	/**
	 * in this mode only SPARKOUT_1_OUTPUT is used
	 */
	IM_ONE_COIL = 0,
	/**
	 * in this mode we use as many coils as we have cylinders
	 */
	IM_INDIVIDUAL_COILS = 1,
	IM_WASTED_SPARK = 2,

	/**
	 * some v12 engines line BMW M70 and M73 run two distributors, one for each bank of cylinders
	 */
	IM_TWO_COILS = 3,

} ignition_mode_e;

/**
 * @see getNumberOfInjections
 */
typedef enum __attribute__ ((__packed__)) {
	/**
	 * each cylinder has it's own injector but they all works in parallel
	 */
	IM_SIMULTANEOUS = 0,
	/**
	 * each cylinder has it's own injector, each injector is wired separately
	 */
	IM_SEQUENTIAL = 1,
	/**
	 * each cylinder has it's own injector but these injectors work in pairs. Injectors could be wired in pairs or separately.
	 * Each pair is fired once per engine cycle
	 * todo: we might want to implement one additional mode where each pair of injectors is floating twice per engine cycle.
	 * todo: this could reduce phase offset from injection to stroke but would not work great for large injectors
	 */
	IM_BATCH = 2,
	/**
	 * only one injector located in throttle body
	 */
	IM_SINGLE_POINT = 3,

} injection_mode_e;

typedef enum __attribute__ ((__packed__)) {
	UART_NONE = 0,
	UART_DEVICE_1 = 1,
	UART_DEVICE_2 = 2,
	UART_DEVICE_3 = 3,
	UART_DEVICE_4 = 4,
} uart_device_e;

/**
 * See spi3mosiPin
 * See spi2MisoMode
 */
typedef enum __attribute__ ((__packed__)) {
	SPI_NONE = 0,
	SPI_DEVICE_1 = 1,
	SPI_DEVICE_2 = 2,
	SPI_DEVICE_3 = 3,
	SPI_DEVICE_4 = 4,
	SPI_DEVICE_5 = 5,
	SPI_DEVICE_6 = 6,
} spi_device_e;

#define SPI_TOTAL_COUNT 6

typedef enum __attribute__ ((__packed__)) {
	BMW_e46 = 0,
	W202 = 1,
	BMW_e90 = 2,
	NISSAN_350 = 3,
	HYUNDAI_PB = 4,
  HONDA_CIVIC9 = 5,
} can_vss_nbc_e;

/**
 * inertia measurement unit, yawn accelerometer
 * By the way both kinds of BOSCH use Housing : TE 1-967640-1, pins 144969-1 seal 967056-1 plug 967067-2
 */
typedef enum __attribute__ ((__packed__)) {
	IMU_NONE = 0,
	IMU_VAG = 1,
	/**
	 * f037000002
	 * https://github.com/rusefi/rusefi_documentation/blob/master/OEM-Docs/Bosch/Data%20Sheet_68903691_Acceleration_Sensor_MM5.10.pdf
	 */
	IMU_MM5_10 = 2,
	IMU_TYPE_3 = 3,
	IMU_TYPE_4 = 4,
	/**
	 * Mercedes pn: A 006 542 26 18
	 * Almost the same as BOSCH above, but XY only and different CAN IDs
	 */
	IMU_TYPE_MB_A0065422618 = 5,
} imu_type_e;

typedef enum __attribute__ ((__packed__)) {
	ES_BPSX_D1 = 0,
	/**
	 * same as innovate LC2
	 * 0v->7.35afr, 5v->22.39
	 */
	ES_Innovate_MTX_L = 1,
	/**
	 * Same as AEM
	 * 0v->10.0afr
	 * 5v->20.0afr
	 */
	ES_14Point7_Free = 2,

	ES_PLX = 4,

	ES_Custom = 5,

	ES_AEM = 6,

} ego_sensor_e;

typedef enum __attribute__ ((__packed__)) {
	MT_CUSTOM = 0,
	MT_DENSO183 = 1,
	/**
	 * 20 to 250 kPa (2.9 to 36.3 psi) 0.2 to 4.9 V OUTPUT
	 */
	MT_MPX4250 = 2,
	MT_HONDA3BAR = 3,
	MT_DODGE_NEON_2003 = 4,
	/**
	 * 22012AA090
	 */
	MT_SUBY_DENSO = 5,
	/**
	 * 16040749
	 */
	MT_GM_3_BAR = 6,

	/**
	 * 20 to 105 kPa (2.9 to 15.2 psi) 0.3 to 4.9 V Output
	 */
	MT_MPX4100 = 7,

	/**
	 * http://rusefi.com/forum/viewtopic.php?f=3&t=906&p=18976#p18976
	 * Toyota 89420-02010
	 */
	MT_TOYOTA_89420_02010 = 8,

	/**
	 * 20 to 250 kPa (2.9 to 36.3 psi) 0.25 to 4.875 OUTPUT
	 * More precise calibration data for new NXP sensor revisions MPX4250A and MPXA4250A.
	 * For an old Freescale MPX4250D use "MT_MPX4250".
	 * See https://www.nxp.com/docs/en/data-sheet/MPX4250A.pdf
	 */
	MT_MPX4250A = 9,


	/**
	 * Bosch 2.5 Bar TMap Map Sensor with IAT
	 * 20 kPa at 0.40V, 250 kPa at 4.65V
	 * 4 pin:
	 *    Pin 1 : Sensor Ground
	 *    Pin 2 : Temp Signal
	 *    Pin 3 : 5v
	 *    Pin 4 : Map Signal
	 * Volkswagen Passat B6
	 */

	MT_BOSCH_2_5 = 10,

	MT_MAZDA_1_BAR = 11,

	MT_GM_2_BAR = 12,

	MT_GM_1_BAR = 13,

	/**
	 * 4 bar
	 */
	MT_MPXH6400 = 14,
	/**
	 * 3 bar
	 */
	MT_MPXH6300 = 15,

} air_pressure_sensor_type_e;

typedef enum {
	REVERSE = -1,
	NEUTRAL = 0,
	GEAR_1 = 1,
	GEAR_2 = 2,
	GEAR_3 = 3,
	GEAR_4 = 4,

} gear_e;

typedef enum __attribute__ ((__packed__)) {
	/**
	 * This is the default mode in which ECU controls timing dynamically
	 */
	TM_DYNAMIC = 0,
	/**
	 * Fixed timing is useful while you are playing with a timing gun - you need to have fixed
	 * timing if you want to install your distributor at some specific angle
	 */
	TM_FIXED = 1,

} timing_mode_e;

	/* I am confused: wingdi.h has CCNONE meaning TODO migrate to proper enum! */
typedef enum __attribute__ ((__packed__)) {
	CCNONE = 0,
	CC_BRAKE = 1,
	CC_CLUTCH = 2,
} cranking_condition_e;

/**
 * Net Body Computer types
 */
typedef enum __attribute__ ((__packed__)) {
	CAN_BUS_NBC_NONE = 0,
	CAN_BUS_NBC_FIAT = 1,
	CAN_BUS_NBC_VAG = 2,
	CAN_BUS_MAZDA_RX8 = 3,
	CAN_BUS_BMW_E46 = 4,
	CAN_BUS_W202_C180 = 5,
    CAN_BUS_BMW_E90 = 6,
	CAN_BUS_Haltech = 7,
	CAN_BUS_MQB = 8,
	CAN_BUS_NISSAN_VQ = 9,
	CAN_BUS_GENESIS_COUPE = 10,
	CAN_BUS_HONDA_K = 11,
	CAN_AIM_DASH = 12,
	CAN_BUS_MS_SIMPLE_BROADCAST = 13,

} can_nbc_e;

typedef enum __attribute__ ((__packed__)) {
	TCHARGE_MODE_RPM_TPS = 0,
	TCHARGE_MODE_AIR_INTERP = 1,
	TCHARGE_MODE_AIR_INTERP_TABLE = 2,

} tChargeMode_e;

typedef enum __attribute__ ((__packed__)) {
	INIT = 0,
	TPS_THRESHOLD = 1,
	RPM_DEAD_ZONE = 2,
	PID_VALUE = 4,
	PID_UPPER = 16,
	BLIP = 64,

} idle_state_e;

// todo: should this be just a boolean?
typedef enum __attribute__ ((__packed__)) {
	OPEN_LOOP = 0,
	CLOSED_LOOP = 1,

} boostType_e;

typedef enum __attribute__ ((__packed__)) {
	SWITCH_INPUT_LAUNCH = 0,
	CLUTCH_INPUT_LAUNCH = 1,
	ALWAYS_ACTIVE_LAUNCH = 2,
	STOP_INPUT_LAUNCH = 3,
	LUA_LAUNCH = 4,
} launchActivationMode_e;

typedef enum __attribute__ ((__packed__)) {
	TORQUE_REDUCTION_BUTTON = 0,
	LAUNCH_BUTTON = 1,
	TORQUE_REDUCTION_CLUTCH_DOWN_SWITCH = 2,
	TORQUE_REDUCTION_CLUTCH_UP_SWITCH = 3,
} torqueReductionActivationMode_e;

typedef enum __attribute__ ((__packed__)) {
	DIGITAL_SWITCH_INPUT = 0,
	LUA_GAUGE = 1,
} nitrous_arming_method_e;

typedef enum __attribute__ ((__packed__)) {
	LUA_GAUGE_1 = 0,
	LUA_GAUGE_2 = 1,
	LUA_GAUGE_3 = 2,
	LUA_GAUGE_4 = 3,
	LUA_GAUGE_5 = 4,
	LUA_GAUGE_6 = 5,
	LUA_GAUGE_7 = 6,
	LUA_GAUGE_8 = 7,
} lua_gauge_e;

typedef enum __attribute__ ((__packed__)) {
	LUA_GAUGE_LOWER_BOUND = 0,
	LUA_GAUGE_UPPER_BOUND = 1,
} lua_gauge_meaning_e;

// this one is "Rotational Idle", it's a naming mess https://github.com/rusefi/rusefi/issues/8435
typedef enum __attribute__ ((__packed__)) {
	SWITCH_INPUT_ANTILAG = 0,
	ALWAYS_ON_ANTILAG = 1,
	LUA_ANTILAG = 2,
} antiLagActivationMode_e;

typedef enum __attribute__ ((__packed__)) {
	GPPWM_Zero = 0,
	GPPWM_Tps = 1,
	GPPWM_Map = 2,
	GPPWM_Clt = 3,
	GPPWM_Iat = 4,
	GPPWM_FuelLoad = 5,
	GPPWM_IgnLoad = 6,
	GPPWM_AuxTemp1 = 7,
	GPPWM_AuxTemp2 = 8,
	GPPWM_AccelPedal = 9,
	GPPWM_Vbatt = 10,
	GPPWM_VVT_1I = 11,
	GPPWM_VVT_1E = 12,
	GPPWM_VVT_2I = 13,
	GPPWM_VVT_2E = 14,
	GPPWM_EthanolPercent = 15,
	GPPWM_AuxLinear1 = 16,
	GPPWM_AuxLinear2 = 17,
	GPPWM_GppwmOutput1 = 18,
	GPPWM_GppwmOutput2 = 19,
	GPPWM_GppwmOutput3 = 20,
	GPPWM_GppwmOutput4 = 21,
	GPPWM_LuaGauge1 = 22,
	GPPWM_LuaGauge2 = 23,
	GPPWM_Rpm = 24,
	GPPWM_DetectedGear = 25,
	GPPWM_BaroPressure = 26,
	GPPWM_Egt1 = 27,
	GPPWM_Egt2 = 28,
	GPPWM_AuxLinear3 = 29,
	GPPWM_AuxLinear4 = 30,
	GPPWM_VehicleSpeed = 31,
	GPPWM_OilPressure = 32,
	GPPWM_OilTemp = 33,
	GPPWM_FuelPressure = 34,
	GPPWM_ThrottleRatio = 35,
	// remember to manually sync 'pwmAxisLabels' in tunerstudio.template.ini
	// todo: rename 'pwmAxisLabels' and maybe even gppwm_channel_e since we now use wider than just 'gppwm'?
} gppwm_channel_e;

typedef enum __attribute__ ((__packed__)) {
	B33KBPS = 0, // 33.33kbps
	B50KBPS = 1, // 50kbps
	B83KBPS = 2, // 83.33kbps
	B100KBPS = 3, // 100kbps
	B125KBPS = 4, // 125kbps
	B250KBPS = 5, // 250kbps
	B500KBPS = 6, // 500kbps
	B666KBPS = 7, // 666kbps
	B1MBPS = 8, // 1Mbps
} can_baudrate_e;

typedef enum __attribute__ ((__packed__)) {
	RUSEFI = 0,
	AEM = 1,
	DISABLED = 2
} can_wbo_type_e;

typedef enum __attribute__ ((__packed__)) {
	WBO_RE_IDLE	= 0,
	WBO_RE_DONE = 1,
	WBO_RE_BUSY = 2,
	WBO_RE_FAILED = 3
} can_wbo_re_status_e;

typedef enum __attribute__((__packed__)) {
	WBO_RE_ID1  =  0,
	WBO_RE_ID2  =  1,
	WBO_RE_ID3  =  2,
	WBO_RE_ID4  =  3,
	WBO_RE_ID5  =  4,
	WBO_RE_ID6  =  5,
	WBO_RE_ID7  =  6,
	WBO_RE_ID8  =  7,
	WBO_RE_ID9  =  8,
	WBO_RE_ID10 =  9,
	WBO_RE_ID11 = 10,
	WBO_RE_ID12 = 11,
	WBO_RE_ID13 = 12,
	WBO_RE_ID14 = 13,
	WBO_RE_ID15 = 14,
	WBO_RE_ID16 = 15
} can_wbo_re_id_e;

// Hardware index, usually strapped by cfg pins and pull-up/pull-down resistors
typedef enum  __attribute__((__packed__)) {
	WBO_RE_HWIDX0 = 0,
	WBO_RE_HWIDX1 = 1,
	WBO_RE_HWIDX2 = 2,
	WBO_RE_HWIDX3 = 3,
	WBO_RE_HWIDX4 = 4,
	WBO_RE_HWIDX5 = 5,
	WBO_RE_HWIDX6 = 6,
	WBO_RE_HWIDX7 = 7,
} can_wbo_re_hwidx_e;

typedef enum __attribute__((__packed__)) {
	WBO_AEM_ID1  =  0,
	WBO_AEM_ID2  =  1,
	WBO_AEM_ID3  =  2,
	WBO_AEM_ID4  =  3,
	WBO_AEM_ID5  =  4,
	WBO_AEM_ID6  =  5,
	WBO_AEM_ID7  =  6,
	WBO_AEM_ID8  =  7,
	WBO_AEM_ID9  =  8,
	WBO_AEM_ID10 =  9,
	WBO_AEM_ID11 = 10,
	WBO_AEM_ID12 = 11,
	WBO_AEM_ID13 = 12,
	WBO_AEM_ID14 = 13,
	WBO_AEM_ID15 = 14,
	WBO_AEM_ID16 = 15
} can_wbo_aem_id_e;

typedef enum __attribute__ ((__packed__)) {
	GPPWM_GreaterThan = 0,
	GPPWM_LessThan = 1,
} gppwm_compare_mode_e;

typedef enum __attribute__ ((__packed__)) {
	VE_None = 0,
	VE_MAP = 1,
	VE_TPS = 2,
} ve_override_e;

typedef enum __attribute__ ((__packed__)) {
	AFR_None = 0,
	AFR_MAP = 1,
	AFR_Tps = 2,
	AFR_AccPedal = 3,
	AFR_CylFilling = 4,
} load_override_e;

typedef enum __attribute__ ((__packed__)) {
	DC_None = 0,
	DC_Throttle1 = 1,
	DC_Throttle2 = 2,
	// this is about SINGLE DC-motor idle valve like 90s volkswagen/earlier M111 engines
	// NOT to be used in dual H-bridge stepper control
	DC_IdleValve = 3,
	DC_Wastegate = 4,
} dc_function_e;

typedef enum __attribute__ ((__packed__)) {
	STEPPER_FULL = 0,
	STEPPER_HALF = 2,
	STEPPER_FOURTH = 4,
	STEPPER_EIGHTH = 8,
} stepper_num_micro_steps_e;

typedef enum __attribute__ ((__packed__)) {
	IPT_Low = 0,
	IPT_High = 1,
} injector_pressure_type_e;

typedef enum __attribute__ ((__packed__)) {
	ICM_None = 0,
	ICM_FixedRailPressure = 1,
	ICM_SensedRailPressure = 2,
  	ICM_HPFP_Manual_Compensation = 3,
} injector_compensation_mode_e;

typedef enum __attribute__ ((__packed__)) {
	FPM_Absolute = 0,
	FPM_Gauge = 1,
	FPM_Differential = 2,
} fuel_pressure_sensor_mode_e;

typedef enum __attribute__ ((__packed__)) {
	INJ_None = 0,
	INJ_PolynomialAdder = 1,
	INJ_FordModel = 2,
} InjectorNonlinearMode;

typedef enum __attribute__ ((__packed__)) {
    HPFP_CAM_NONE = 0,
    HPFP_CAM_IN1 = 1,
    HPFP_CAM_EX1 = 2,
    HPFP_CAM_IN2 = 3,
    HPFP_CAM_EX2 = 4,
} hpfp_cam_e;

// IMPORTANT: This enum must be kept in sync with the definition in rusefi_config.txt
typedef enum __attribute__ ((__packed__)) {
	AE_MODE_MS_ADDER = 0,
	AE_MODE_PERCENT_ADDER = 1,
	AE_MODE_PREDICTIVE_MAP = 2,
} accel_enrichment_mode_e;

#if __cplusplus
#include <cstdint>

enum class TsCalMode : uint8_t {
	None = 0,
	Tps1Max = 1,
	Tps1Min = 2,
	EtbKp = 3,
	EtbKi = 4,
	EtbKd = 5,
	Tps1SecondaryMax = 6,
	Tps1SecondaryMin = 7,
	Tps2Max = 8,
	Tps2Min = 9,
	Tps2SecondaryMax = 10,
	Tps2SecondaryMin = 11,
	PedalMin = 12,
	PedalMax = 13,
	EwgPosMax = 14,
	EwgPosMin = 15,
};

enum class GearControllerMode : uint8_t {
	None = 0,
	ButtonShift = 1,
	Automatic = 2,
	Generic = 3,
};

enum class TransmissionControllerMode : uint8_t {
	None = 0,
	SimpleTransmissionController = 1,
	Generic4 = 2,
	Gm4l6x = 3,
};

enum class InjectionTimingMode : uint8_t {
	End = 0,
	Start = 1,
	Center = 2,
};

enum class SelectedGear : uint8_t {
	Invalid = 0,
	ManualPlus = 1,
	ManualMinus = 2,
	Park = 3,
	Reverse = 4,
	Neutral = 5,
	Drive = 6,
	Manual = 7,
	Manual3 = 8,
	Manual2 = 9,
	Manual1 = 10,
	Low = 11,
};

#define SC_Exhaust_First 1

typedef enum __attribute__ ((__packed__)) {
	stftEnabled = 0,
	stftDisabledSettings = 1,
	stftDisabledTuning = 2,
	stftDisabledRPM = 3,
	stftDisabledCrankingDelay = 4,
	stftDisabledClt = 5,
	// below state related to learning only
	stftDisabledAfrOurOfRange = 6,
	stftDisabledDFCO = 7,
	stftDisabledTpsAccel = 8,
	stftDisabledFuelCut = 9
} stft_state_e;

typedef enum __attribute__((__packed__)) {
	ftRegionIdle = 0,
	ftRegionOverrun = 1,
	ftRegionPower = 2,
	ftRegionCruise = 3,
} ft_region_e;

#endif // __cplusplus

#include "generated/enums/rusefi_config_generated_enums.h"

using namespace rusefi::generated::enums;
