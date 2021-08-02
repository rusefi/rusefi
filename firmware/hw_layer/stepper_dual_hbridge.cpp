
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

void DualHBridgeStepper::initialize(DcMotor* motorPhaseA, DcMotor* motorPhaseB, float reactionTime)
{
    setReactionTime(reactionTime);

    m_motorPhaseA = motorPhaseA;
    m_motorPhaseB = motorPhaseB;
}

bool DualHBridgeStepper::step(bool positive) {
	// Check that we've been initialized
	if (!m_motorPhaseA || !m_motorPhaseB) {
		return false;
	}

    // step phase, wrapping
    if (positive) {
        m_phase = (m_phase + 1) & 0x03;
    } else {
        m_phase = (m_phase - 1) & 0x03;
    }

    // Set phases according to the table
    m_motorPhaseA->set(phaseA[m_phase]);
    m_motorPhaseB->set(phaseB[m_phase]);

    pause();
    return true;
}

#endif
