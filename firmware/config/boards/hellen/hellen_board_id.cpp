/**
 * @file boards/hellen/hellen_board_id.cpp
 * @brief Board-Id detector for Hellen boards
 *
 * @author andreika <prometheus.pcb@gmail.com>
 * @author Andrey Belomutskiy, (c) 2012-2022
 *
 * The main idea is to measure the capacitor charge/discharge time
 * through a series resistors using standard digital I/O pins.
 * One pin is used to provide a Vcc(3.3) or Vdd(0) voltage to the capacitor
 * through a resistor, and another pin is used as a digital input. Then vice versa.
 *
 * The algo:
 * 1) Completely discharge the capacitor (all pins are low)
 * 2) Charge the capacitor until the voltage crosses the 0->1 voltage threshold (Vt) and measure the charging time #1 (Tc1).
 * 3) Immediately discharge the capacitor to some unknown low voltage (Vl) - it should be well below the Vt threshold,
 *    using the same period of time used for charging as the discharge period (Td = Tc1).
 * 4) Immediately charge the capacitor again and measure the time crossing the same 0->1 voltage threshold again (Tc2).
 * 5) Repeat the procedure several times to get more precise timings.
 * 6) Do some math and find the R and C values.
 * 7) Board_Id = the unique combination of indices of the "measured" R1 and R2.
 *
 * The math proof:
 * - Charging formula #1:
 *	  Vt = V�� * (1 - exp(-Tc1 / RC))
 * - Discharging formula:
 *	  Vl = Vt * exp(-Td / RC)
 * - Charging formula #2:
 *	  Vl = V�� * (1 - exp(-Tl / (RC)))
 * - Where Tl is a charging time from 0 to Vl:
 *	  Tl = Tc1 - Tc2
 * - Solve the equations:
 *    Vl = V�� * (1 - exp(-Tl / RC)) = Vt * exp(-Td / RC)
 *    V�� * (1 - exp(-Tl / RC)) = V�� * (1 - exp(-Tc1 / RC)) * exp(-Td / RC)
 *    (1 - exp(-Tl / RC)) = (1 - exp(-Tc1 / RC)) * exp(-Td / RC)
 * - Simplify the equation:
 *    X = exp(-1/(RC))
 *    (1 - X^Tc1) * X^Td + X^Tl - 1 = 0
 *
 *    X^Td - X^(Tc1+Td) + X^(Tc2-Tc1) - 1 = 0
 *
 *    Td, Tc1 and Tc2 are known.
 * - Solve the power function for X and get the desired R or C.
 *
 *   We use Newton's method (a fast-converging numerical solver when the 1st derivative is known) 
 *   with estimated initial values.
 */

#include "pch.h"
#include "hellen_meta.h"
#include "digital_input_exti.h"

#include "hellen_board_id.h"

/* We use known standard E24 series resistor values (1%) to find the closest match.
   The 16 major values should have a guarateed spacing of 15% in a row (1% R tolerance + 10% C tolerance)
   These should match the values in the gen_board_id script!
*/
#include "hellen_board_id_resistors.h"

//#define HELLEN_BOARD_ID_DEBUG

#if EFI_PROD_CODE

static void hellenBoardIdInputCallback(void *arg, efitick_t nowNt) {
	UNUSED(arg);
	HellenBoardIdFinderState *state = (HellenBoardIdFinderState *)arg;
	// Now start discharging immediately! This should be the first command in the interrupt handler.
	palClearPad(state->rOutputPinPort, state->rOutputPinIdx);

	state->timeChargeNt = nowNt;

	chibios_rt::CriticalSectionLocker csl;
	chSemSignalI(&state->boardId_wake);  // no need to call chSchRescheduleS() because we're inside the ISR
}

#endif /* EFI_PROD_CODE */

// Newton's numerical method (x is R and y is C, or vice-versa)
float HellenBoardIdSolver::solve(float Tc1, float Tc2, float x0, float y, float deltaX) {
	// the discharge time equals to the charge time
	float Td = Tc1;

	float iC = -1.0f / y;
	k1 = iC * Td;
	k2 = iC * (Tc1 + Td);
	k3 = iC * (Tc1 - Tc2);
	
	// the same method works for R (if C is known) or C (if R is known)
	float Xcur, Xnext;
	Xnext = x0;

	// since we had https://github.com/rusefi/rusefi/issues/4084 let's add paranoia check
	// All real cases seem to converge in <= 5 iterations, so we don't need to try more than 20.
	int safetyLimit = 20;
	do {
		if (safetyLimit-- < 0) {
			firmwareError(OBD_PCM_Processor_Fault, "hellen boardID is broken");
			break;
		}
		Xcur = Xnext;
		Xnext = Xcur - fx(Xcur) / dfx(Xcur);

#ifdef HELLEN_BOARD_ID_DEBUG
		efiPrintf ("* %f", Xnext);
#endif /* HELLEN_BOARD_ID_DEBUG */
	} while (absF(Xnext - Xcur) > deltaX);

	return Xnext;
}

float HellenBoardIdFinderBase::findClosestResistor(float R, bool testOnlyMajorSeries, int *rIdx) {
	// the first "major" resistor uses less values (with more spacing between them) so that even less precise method cannot fail.
	static const float rOnlyMajorValues[] = { 
		HELLEN_BOARD_ID_MAJOR_RESISTORS
	};
	// the minor resistor is always measured after the major one, when the exact capacitance is already knows,
	// so we can use more values and detect them with better precision.
	static const float rAllValues[] = { 
		// these are equal to the major values and should be used first
		HELLEN_BOARD_ID_MAJOR_RESISTORS
		// these are extended series if 256 board IDs aren't enough (16*16).
		HELLEN_BOARD_ID_MINOR_RESISTORS
	};

	size_t rValueSize = testOnlyMajorSeries ? efi::size(rOnlyMajorValues) : efi::size(rAllValues);

	*rIdx = -1;
	float minDelta = 1.e6f;
	for (size_t i = 0; i < rValueSize; i++) {
		// Find the nearest resistor by least ratio error
		float delta = absF(1 - (R / rAllValues[i]));
		if (delta < minDelta) {
			minDelta = delta;
			*rIdx = i;
#ifdef HELLEN_BOARD_ID_DEBUG
			efiPrintf("* [%d] R = %.0f, delta = %f", i, rAllValues[i], delta);
#endif /* HELLEN_BOARD_ID_DEBUG */		
		}
	}
	return rAllValues[*rIdx];
}

float HellenBoardIdFinderBase::calcEstimatedResistance(float Tc1_us, float C) {
	constexpr float Vcc = 3.3f - 0.1f;	// STM32 digital I/O voltage (adjusted for minor voltage drop)
	constexpr float V01 = Vcc * 0.5f;	// let it be 1.6 volts (closer to the datasheet value), the exact value doesn't matter
	// macos compiler doesn't like log() in constexpr
	float log1V01Vcc = log(1.0f - V01 / Vcc);
	// this is only an estimated value, we cannot use it for Board-ID detection!
	float Rest = -Tc1_us / (C * log1V01Vcc);
	return Rest;
}

float HellenBoardIdFinderBase::calc(float Tc1_us, float Tc2_us, float Rest, float C, bool testOnlyMajorSeries, float *Rmeasured, float *newC, int *rIdx) {
	constexpr float Cest = HELLEN_BOARD_ID_CAPACITOR;
	// Now calculate the resistance value
	HellenBoardIdSolver rSolver;

	// solve the equation for R (1 Ohm precision is more than enough)
    *Rmeasured = rSolver.solve(Tc1_us, Tc2_us, Rest, C, 1.0f);

	// add 22 Ohms for pin's internal resistance
	// (according to the STM32 datasheets, the voltage drop on an output pin can be up to 0.4V for 8 mA current)
	// Actual measured value was is in the low-20s on most chips.
	constexpr float Rinternal = 22.0f;
	float R = findClosestResistor(*Rmeasured - Rinternal, testOnlyMajorSeries, rIdx);

	// Find the 'real' capacitance value and use it for the next resistor iteration (gives more precision)
	HellenBoardIdSolver cSolver;

	// We expect the capacitance to be +-10%
	constexpr float capacitorPrecision = 0.1f;
	constexpr float Cmin = Cest * (1.0f - capacitorPrecision);
	constexpr float Cmax = Cest * (1.0f + capacitorPrecision);
	
	// solve the equation for C (1% precision)
    *newC = cSolver.solve(Tc1_us, Tc2_us, Cmin, R + Rinternal, 0.01f);
    // in case something went wrong, we must be in the allowed range
    *newC = clampF(Cmin, *newC, Cmax);

    return R;
}

template <size_t NumPins>
bool HellenBoardIdFinder<NumPins>::measureChargingTimes(int i, float & Tc1_us, float & Tc2_us) {
#if EFI_PROD_CODE
	chSemReset(&state.boardId_wake, 0);

	// full charge/discharge time, and also 'timeout' time
	const int Tf_us = 50000; // 50 ms is more than enough to "fully" discharge the capacitor with any two resistors used at the same time.

	// 1. Fully discharge the capacitor through both resistors (faster)
	for (size_t k = 0; k < NumPins; k++) {
		palClearPad(getBrainPinPort(rPins[k]), getBrainPinIndex(rPins[k]));
		palSetPadMode(getBrainPinPort(rPins[k]), getBrainPinIndex(rPins[k]), PAL_MODE_OUTPUT_PUSHPULL);
	}
	// wait max. time because we don't know the resistor values yet
	chThdSleepMicroseconds(Tf_us);

	// use one pin as an charge/discharge controlling output
	state.rOutputPinPort = getBrainPinPort(rPins[i]);
	state.rOutputPinIdx = getBrainPinIndex(rPins[i]);
	palSetPadMode(state.rOutputPinPort, state.rOutputPinIdx, PAL_MODE_OUTPUT_PUSHPULL);

	// use another pin as an input to detect 0->1 crossings
	int inputIdx = 1 - i;
	state.rInputPinPort = getBrainPinPort(rPins[inputIdx]);
	state.rInputPinIdx = getBrainPinIndex(rPins[inputIdx]);
	// set only high-Z input mode, no pull-ups/pull-downs allowed!
	palSetPadMode(state.rInputPinPort, state.rInputPinIdx, PAL_MODE_INPUT);
	efiExtiEnablePin("boardId", rPins[inputIdx], PAL_EVENT_MODE_RISING_EDGE, hellenBoardIdInputCallback, (void *)&state);
    
    int pinState = palReadPad(state.rInputPinPort, state.rInputPinIdx);
    if (pinState != 0) {
    	// the input pin state should be low when the capacitor is fully discharged
    	efiPrintf("* Board detection error!");
    	return false;
    }

	// Timestamps:
	// t1 = Starts charging from 0v
	// t2 = Threshold reached, starts discharging
	// t3 = Random voltage reached, starts charging again
	// t4 = Threshold reached again, process finished.

	// 2. Start charging until the input pin triggers (V01 threshold is reached)
	state.timeChargeNt = 0;
	efitick_t t1 = getTimeNowNt();
	palSetPad(state.rOutputPinPort, state.rOutputPinIdx);
	chSemWaitTimeout(&state.boardId_wake, TIME_US2I(Tf_us));
	efitick_t t2 = state.timeChargeNt;

	// 3. At the moment, the discharging has already been started!
	// Meanwhile we need to do some checks - until some pre-selected voltage is presumably reached.

	// if voltage didn't change on the input pin (or changed impossibly fast), then the charging didn't start,
	// meaning there's no capacitor and/or resistors on these pins.
	if (t2 - t1 < US2NT(100)) {
		efiPrintf("* Hellen Board ID circuitry wasn't detected! Aborting!");
		return false;
	}

	// 4. calculate the first charging time
	efitick_t Tc1_nt = t2 - t1;
	Tc1_us = NT2USF(Tc1_nt);
	// We use the same 'charging time' to discharge the capacitor to some random voltage below the threshold voltage.
	efitick_t Td_nt = Tc1_nt;

	// 5. And now just wait for the rest of the discharge process...
	// Spin wait since chThdSleepMicroseconds() lacks the resolution we need
	efitick_t t3 = t2 + Td_nt;
	while (getTimeNowNt() < t3) ;

	// the input pin state should be low when the capacitor is discharged to Vl
	pinState = palReadPad(state.rInputPinPort, state.rInputPinIdx);

	// 6. And immediately begin charging again until the threshold voltage is reached!
	state.timeChargeNt = 0;
	palSetPad(state.rOutputPinPort, state.rOutputPinIdx);

	// Wait for the charging completion
	chSemReset(&state.boardId_wake, 0);
	chSemWaitTimeout(&state.boardId_wake, TIME_US2I(Tf_us));
	efitick_t t4 = state.timeChargeNt;

	// 7. calculate the second charge time
	Tc2_us = NT2USF(t4 - t3);

#ifdef HELLEN_BOARD_ID_DEBUG
	efitick_t nowNt4 = getTimeNowNt();
	efiPrintf("* dTime2-1 = %d", (int)(t2 - t1));
	efiPrintf("* dTime3-2 = %d", (int)(t3 - t2));
	efiPrintf("* dTime4-3 = %d", (int)(t4 - t3));
	efiPrintf("* Tc1 = %f, Tc2 = %f, Td = %f", Tc1_us, Tc2_us, Td_us);
#endif /* HELLEN_BOARD_ID_DEBUG */

    // sanity checks
    if (pinState != 0) {
		float Td_us = NT2USF(Td_nt);
		efiPrintf("* Board detection error! (Td=%f is too small)", Td_us);
		return false;
    }

	if (t4 <= t3) {
		efiPrintf("* Estimates are out of limit! Something went wrong. Aborting!");
		return false;
	}

	efiExtiDisablePin(rPins[inputIdx]);
#endif /* EFI_PROD_CODE */
	return true;
}

template <size_t NumPins>
bool HellenBoardIdFinder<NumPins>::measureChargingTimesAveraged(int i, float & Tc1_us, float & Tc2_us) {
	const int numTries = 3;

	Tc1_us = 0;
	Tc2_us = 0;
	for (int tries = 0; tries < numTries; tries++) {
		// get the charging times
		float Tc1i_us = 0, Tc2i_us = 0;
		if (!measureChargingTimes(i, Tc1i_us, Tc2i_us))
			return false;
		Tc1_us += Tc1i_us;
		Tc2_us += Tc2i_us;
	}

	// averaging
	Tc1_us /= numTries;
	Tc2_us /= numTries;

	return true;
}


int detectHellenBoardId() {
	int boardId = -1;
#if EFI_PROD_CODE
	efiPrintf("Starting Hellen Board ID detection...");
	efitick_t beginNt = getTimeNowNt();
	
	// Hellen boards use Gpio::F0 and Gpio::F1.
	const int numPins = 2;
	Gpio rPins[numPins] = { Gpio::F0, Gpio::F1 };
	
	// We start from the estimated capacitance, but the real one can be +-10%
	float C = HELLEN_BOARD_ID_CAPACITOR;

	// we need to find the resistor values connected to the mcu pins and to the capacitor.
	float R[numPins] = { 0 };
	int rIdx[numPins] = { 0 };

	HellenBoardIdFinder<numPins> finder(rPins);

	// init some ChibiOs objects
	chSemObjectInit(&finder.state.boardId_wake, 0);

	// R1 is the first, R2 is the second
	for (int i = 0; i < numPins; i++) {
#ifdef HELLEN_BOARD_ID_DEBUG
		efiPrintf("*** Resistor R%d...", i + 1);
#endif /* HELLEN_BOARD_ID_DEBUG */		

		float Tc1_us = 0, Tc2_us = 0;
		// We need several measurements for each resistor to increase the presision.
		// But if any of the measurements fails, then abort!
		if (!finder.measureChargingTimesAveraged(i, Tc1_us, Tc2_us))
			break;

		// Now roughly estimate the resistor value using the approximate threshold voltage.
		float Rest = finder.calcEstimatedResistance(Tc1_us, C);
		// check if we are inside the range
		if (Rest < 300.0f || Rest > 15000.0f) {
			efiPrintf("* Unrealistic estimated resistor value (%f)! Aborting!", Rest);
			break;
		}

		// for the first resistor, we test only "major" values because we don't know the exact capacitance yet
		bool testOnlyMajorSeries = (i == 0);

		float Rmeasured, newC;
		// Now calculate the R and C
		R[i] = finder.calc(Tc1_us, Tc2_us, Rest, C, testOnlyMajorSeries, &Rmeasured, &newC, &rIdx[i]);
		C = newC;

#ifdef HELLEN_BOARD_ID_DEBUG
		efiPrintf("* R = %f, Rmeasured = %f, Rest = %f, Creal = %f", R[i], Rmeasured, Rest, C);
#endif /* HELLEN_BOARD_ID_DEBUG */
	}

	// in case the process was aborted
	for (size_t k = 0; k < numPins; k++) {
		efiExtiDisablePin(rPins[k]);
		// release the pins
		palSetPadMode(getBrainPinPort(rPins[k]), getBrainPinIndex(rPins[k]), PAL_MODE_RESET);
	}

	efitick_t endNt = getTimeNowNt();
	int elapsed_Ms = US2MS(NT2US(endNt - beginNt));

	// Check that all resistors were actually detected
	bool allRValid = true;
	for (size_t i = 0; i < numPins; i++) {
		allRValid &= R[i] != 0;
	}

	// Decode board ID only if all resistors could be decoded, otherwise we return -1
	if (allRValid) {
		boardId = HELLEN_GET_BOARD_ID(rIdx[0], rIdx[1]);
	} else {
		boardId = -1;
	}

	efiPrintf("* RESULT: BoardId = %d, R1 = %.0f, R2 = %.0f (Elapsed time: %d ms)", boardId, R[0], R[1], elapsed_Ms);
#endif /* EFI_PROD_CODE */
	return boardId;
}
