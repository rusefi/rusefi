/**
 * @file boards/hellen/hellen_board_id.h
 * @brief Board-Id detector for Hellen boards
 *
 * @author andreika <prometheus.pcb@gmail.com>
 * @author Andrey Belomutskiy, (c) 2012-2022
 */

#pragma once

// this is used by the detection method and should be visible to the interrupt handler (hellenBoardIdInputCallback)
class HellenBoardIdFinderState
{
public:
	efitick_t timeChargeNt = 0;
	
	ioportid_t rOutputPinPort;
	int rOutputPinIdx;
	ioportid_t rInputPinPort;
	int rInputPinIdx;

#if EFI_PROD_CODE
	semaphore_t boardId_wake;
#endif /* EFI_PROD_CODE */
};

// We need to solve the following equation for R or C:
//   X^Td - X^(Tc1+Td) + X^(Tc2-Tc1) - 1 = 0
// where: X = exp(-1/(RC))
class HellenBoardIdSolver : public NewtonsMethodSolver
{
public:
	float fx(float x) override {
		return exp(k1 / x) - exp(k2 / x) + exp(k3 / x) - 1.0;
	}

	// first-order derivative
	float dfx(float x) override {
		return (-1.0f / (x * x)) * (k1 * exp(k1 / x) - k2 * exp(k2 / x) + k3 * exp(k3 / x));
	}

	// Newton numerical method (x is R and y is C, or vice-versa)
	float solve(float Tc1, float Tc2, float x0, float y, float deltaX);

private:
	// exponential function coefs (see solve())
	float k1, k2, k3;
};


class HellenBoardIdFinderBase
{
public:
	float calc(float Tc1_us, float Tc2_us, float Rest, float C, bool testOnlyMajorSeries, float *Rmeasured, float *Cest, int *rIdx);

	float findClosestResistor(float R, bool testOnlyMajorSeries, int *rIdx);
	float calcEstimatedResistance(float Tc1_us, float C);

public:
	HellenBoardIdFinderState state;
};

template <size_t NumPins>
class HellenBoardIdFinder : public HellenBoardIdFinderBase
{
public:
	HellenBoardIdFinder(brain_pin_e (&rP)[NumPins]) : rPins(rP) {}

	// R1 or R2
	bool measureChargingTimes(int i, float & Tc1_us, float & Tc2_us);
	bool measureChargingTimesAveraged(int i, float & Tc1_us, float & Tc2_us);

public:
	brain_pin_e (&rPins)[NumPins];
	HellenBoardIdFinderState state;
};

