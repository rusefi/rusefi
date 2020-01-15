#include "stepper.h"
#include "dc_motor.h"

static const int8_t phaseA[] = 
{
    1,
    0,
    -1,
    0
};

static const int8_t phaseB[] = 
{
    0,
    1,
    0,
    -1
};

class DualHBridgeStepper final : public StepperHw {
    void initialize(DcMotor* motorPhaseA, DcMotor* motorPhaseB, float reactionTime);

    void step(bool positive) override;

private:
    DcMotor* m_motorPhaseA;
    DcMotor* m_motorPhaseB;

    uint8_t m_phase = 0;
};

void DualHBridgeStepper::initialize(DcMotor* motorPhaseA, DcMotor* motorPhaseB, float reactionTime)
{
    setReactionTime(reactionTime);

    m_motorPhaseA = motorPhaseA;
    m_motorPhaseB = motorPhaseB;
}

void DualHBridgeStepper::step(bool positive) {
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
}
