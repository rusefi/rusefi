#include "pch.h"

// Constructors/destructors for all mocks live here.  If they only exist
// in the header (ie, implicitely defined), then that code has to be generated
// in every cpp file, instead of generating them only once, which we do here.

// See https://github.com/google/googletest/blob/master/docs/gmock_cook_book.md#making-the-compilation-faster

MockEtb::MockEtb() { }
MockEtb::~MockEtb() { }

MockMotor::MockMotor() { }
MockMotor::~MockMotor() { }

MockVp3d::MockVp3d() { }
MockVp3d::~MockVp3d() { }

MockPwm::MockPwm() { }
MockPwm::~MockPwm() { }

MockOutputPin::MockOutputPin() { }
MockOutputPin::~MockOutputPin() { }

MockExecutor::MockExecutor() { }
MockExecutor::~MockExecutor() { }

MockAirmass::MockAirmass() : AirmassVeModelBase(veTable) { }
MockAirmass::~MockAirmass() { }

MockInjectorModel2::MockInjectorModel2() { }
MockInjectorModel2::~MockInjectorModel2() { }

MockStepperHardware::MockStepperHardware() { }
MockStepperHardware::~MockStepperHardware() { }

MockTsChannel::MockTsChannel() : TsChannelBase("mock") { }
MockTsChannel::~MockTsChannel() { }
