
#include "pch.h"

#if !EFI_UNIT_TEST

#include "stepper.h"
#include "dc_motor.h"

static const int8_t phaseA[] = 
{
    1,
    1,
    -1,
    -1
};

static const int8_t phaseB[] = 
{
    -1,
    1,
    1,
    -1
};

static const int8_t microPhases[] = {
    0,    20,  38,  56,  71,  83,  92,  98,
    100,  98,  92,  83,  71,  56,  38,  20,
    0,   -20, -38, -56, -71, -83, -92, -98,
   -100, -98, -92, -83, -71, -56, -38, -20
};

static const int maxNumSteps = 8;
static constexpr int tableSizeMask = efi::size(microPhases) - 1;
static constexpr float phaseDutyCycleDivisor = 1.0f / (100.0f * 100.0f);	// both the phase degrees and duty cycle settings are in %


void DualHBridgeStepper::initialize(DcMotor* motorPhaseA, DcMotor* motorPhaseB, float reactionTime)
{
    setReactionTime(reactionTime);

    m_motorPhaseA = motorPhaseA;
    m_motorPhaseB = motorPhaseB;

    efiAssertVoid(CUSTOM_ERR_ASSERT, CONFIG(stepperNumMicroSteps) <= maxNumSteps, "stepperNumMicroSteps");
}

bool DualHBridgeStepper::step(bool positive) {
	// Check that we've been initialized
	if (!m_motorPhaseA || !m_motorPhaseB) {
		return false;
	}

	if (CONFIG(stepperNumMicroSteps) > 1) {
		float dutyMult = CONFIG(stepperMaxDutyCycle) * phaseDutyCycleDivisor;
		int numStepIncr = maxNumSteps / CONFIG(stepperNumMicroSteps);
		if (!positive)
			numStepIncr = -numStepIncr;
		for (int i = CONFIG(stepperNumMicroSteps); i > 0; i--) {
			m_phase = (m_phase + numStepIncr) & tableSizeMask;
			update(dutyMult);
			// sleep 1/Nth of the pause time
			pause(CONFIG(stepperNumMicroSteps));
		}
		return true;
	}

    // For the full-stepping mode, we use a traditional "two phase on" drive model
    // because "wave drive" (one phase on) method has less torque.
    // For explanation, pls see: https://github.com/rusefi/rusefi/pull/3213#discussion_r700746453

    // step phase, wrapping
    if (positive) {
        m_phase = (m_phase + 1) & 0x03;
    } else {
        m_phase = (m_phase - 1) & 0x03;
    }

    update(1.0f);
    pause();

    return true;
}

bool DualHBridgeStepper::update(float dutyMult) {
	if (!m_motorPhaseA || !m_motorPhaseB) {
		return false;
	}

	if (CONFIG(stepperNumMicroSteps) > 1) {
	    // phase B is 90 degrees shifted
	    uint8_t m_phaseB = (m_phase + CONFIG(stepperNumMicroSteps)) & tableSizeMask;
	    
	    // Set phases according to the table
	    m_motorPhaseA->set(dutyMult * microPhases[m_phase]);
	    m_motorPhaseB->set(dutyMult * microPhases[m_phaseB]);
	    return true;
	}
    // Set phases according to the table
    m_motorPhaseA->set(phaseA[m_phase]);
    m_motorPhaseB->set(phaseB[m_phase]);
	return true;
}

void DualHBridgeStepper::sleep() {
	float sleepingCoef = minI(CONFIG(stepperMinDutyCycle), CONFIG(stepperMaxDutyCycle)) * phaseDutyCycleDivisor;
	update(sleepingCoef);
	pause();
}

#endif
