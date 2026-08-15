/**
 * @file test_uninitialized_members.cpp
 *
 * Engine modules live inside the global Engine, so the compiler zero initializes their members
 * whether or not they carry an initializer. Several classes lean on that: they null-check or
 * flag-check a member which is only ever written by init().
 *
 * That holds right up until someone constructs one somewhere other than file scope - which is
 * exactly what a unit test does. GearControllerBase::transmissionController was the same shape and
 * segfaulted the test binary (see the #6380 work); these pin the contract so the next one does not.
 */

#include "pch.h"

#include "long_term_fuel_trim.h"
#include "tachometer.h"



/**
 * TachometerModule::onFastCallback() gates all of its behaviour on tachHasInit, which only init()
 * writes. A stack instance must therefore do nothing at all rather than act on a garbage flag.
 */
TEST(UninitializedMembers, tachometerIsInertBeforeInit) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	TachometerModule dut;

	// no init() call - the module must treat itself as disabled
	dut.onFastCallback();
	dut.onFastCallback();

	SUCCEED() << "onFastCallback() before init() must be a no-op, not a read of an indeterminate flag";
}

/**
 * LongTermFuelTrim::store() null-checks m_state, which only init() writes. The guard is only
 * meaningful if the pointer starts null.
 */
TEST(UninitializedMembers, ltftStoreIsSafeBeforeInit) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	LongTermFuelTrim dut;

	// no init() call - store() must take its null branch rather than dereference garbage
	dut.store();

	SUCCEED() << "store() before init() must hit its null guard";
}
