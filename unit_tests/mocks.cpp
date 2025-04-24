#include "pch.h"

// Constructors/destructors for all mocks live here.  If they only exist
// in the header (ie, implicitely defined), then that code has to be generated
// in every cpp file, instead of generating them only once, which we do here.

// See https://github.com/google/googletest/blob/master/docs/gmock_cook_book.md#making-the-compilation-faster
/*
* Note: if you are mocking an EngineModule to test *another* EngineModule
* both have to be mocked and not have references to unmock().etc,
* otherwise they will end up pointing to different places and you will have unexpected results!
*
* ex:
*    HpfpController hpfp;
*    engine->engineModules.get<HpfpController>().set(&hpfp);
*    LimpManager dut;
*    engine->engineModules.get<LimpManager>().set(&dut);
*/

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

MockIdleController::MockIdleController() { }
MockIdleController::~MockIdleController() { }

MockIgnitionController::MockIgnitionController() { }
MockIgnitionController::~MockIgnitionController() { }

MockHpfpController::MockHpfpController() { }
MockHpfpController::~MockHpfpController() { }

MockLimpManager::MockLimpManager() { }
MockLimpManager::~MockLimpManager() { }