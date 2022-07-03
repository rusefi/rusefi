#include "pch.h"

using ::testing::StrictMock;

class MockTriggerConfiguration : public TriggerConfiguration {
public:
	MockTriggerConfiguration(bool useOnlyRise, trigger_config_s type)
		: TriggerConfiguration("Mock")
		, m_useOnlyRise(useOnlyRise)
		, m_type(type)
	{ }

protected:
	bool isUseOnlyRisingEdgeForTrigger() const override {
		return m_useOnlyRise;
	}

	bool isVerboseTriggerSynchDetails() const override {
		return false;
	}

	trigger_config_s getType() const override {
		return m_type;
	}

private:
	const bool m_useOnlyRise;
	const trigger_config_s m_type;
};

struct MockTriggerDecoder : public TriggerDecoderBase {
	MockTriggerDecoder() : TriggerDecoderBase("mock") { }

	MOCK_METHOD(void, onTriggerError, (), (override));
};

static auto makeTriggerShape(operation_mode_e mode, const TriggerConfiguration& config) {
	TriggerWaveform shape;
	shape.initializeTriggerWaveform(mode, config);

	return shape;
}

#define doTooth(dut, shape, cfg, t) dut.decodeTriggerEvent("", shape, nullptr, cfg, SHAFT_PRIMARY_RISING, t)

TEST(TriggerDecoder, FindsFirstSyncPoint) {
	MockTriggerConfiguration cfg(true, {TT_TOOTHED_WHEEL, 4, 1});
	cfg.update();

	auto shape = makeTriggerShape(FOUR_STROKE_CAM_SENSOR, cfg);

	efitick_t t = 0;

	// Strict so it complains about unexpected calls to onTriggerError()
	StrictMock<MockTriggerDecoder> dut;

	// Fire a few boring evenly spaced teeth
	t += MS2NT(1);
	doTooth(dut, shape, cfg, t);
	EXPECT_FALSE(dut.getShaftSynchronized());
	EXPECT_EQ(2, dut.currentCycle.current_index);

	t += MS2NT(1);
	doTooth(dut, shape, cfg, t);
	EXPECT_FALSE(dut.getShaftSynchronized());
	EXPECT_EQ(4, dut.currentCycle.current_index);

	t += MS2NT(1);
	doTooth(dut, shape, cfg, t);
	EXPECT_FALSE(dut.getShaftSynchronized());
	EXPECT_EQ(6, dut.currentCycle.current_index);

	// Missing tooth, 2x normal length!
	t += MS2NT(2);
	doTooth(dut, shape, cfg, t);
	EXPECT_TRUE(dut.getShaftSynchronized());
	EXPECT_EQ(0, dut.currentCycle.current_index);

	// Normal tooth after the missing tooth, should be index 2 now (one rise + one fall)
	t += MS2NT(1);
	doTooth(dut, shape, cfg, t);
	EXPECT_TRUE(dut.getShaftSynchronized());
	EXPECT_EQ(2, dut.currentCycle.current_index);

	EXPECT_FALSE(dut.someSortOfTriggerError());
}


TEST(TriggerDecoder, FindsSyncPointMultipleRevolutions) {
	MockTriggerConfiguration cfg(true, {TT_TOOTHED_WHEEL, 4, 1});
	cfg.update();

	auto shape = makeTriggerShape(FOUR_STROKE_CAM_SENSOR, cfg);

	efitick_t t = 0;

	// Strict so it complains about unexpected calls to onTriggerError()
	StrictMock<MockTriggerDecoder> dut;

	// normal tooth
	t += MS2NT(1);
	doTooth(dut, shape, cfg, t);

	// normal tooth
	t += MS2NT(1);
	doTooth(dut, shape, cfg, t);

	// Missing tooth, 2x normal length!
	t += MS2NT(2);
	doTooth(dut, shape, cfg, t);
	EXPECT_TRUE(dut.getShaftSynchronized());
	EXPECT_EQ(0, dut.currentCycle.current_index);
	EXPECT_EQ(0, dut.getTotalRevolutionCounter());

	// Do 100 turns and make sure we stay synchronized
	for (int i = 0; i < 100; i++) {
		// normal tooth
		t += MS2NT(1);
		doTooth(dut, shape, cfg, t);
		EXPECT_EQ(2, dut.currentCycle.current_index);

		// normal tooth
		t += MS2NT(1);
		doTooth(dut, shape, cfg, t);
		EXPECT_EQ(4, dut.currentCycle.current_index);

		// Missing tooth, 2x normal length!
		t += MS2NT(2);
		doTooth(dut, shape, cfg, t);
		EXPECT_TRUE(dut.getShaftSynchronized());
		EXPECT_EQ(0, dut.currentCycle.current_index);
		EXPECT_FALSE(dut.someSortOfTriggerError());

		// We do one revolution per loop iteration
		EXPECT_EQ(i + 1, dut.getTotalRevolutionCounter());
	}
}

TEST(TriggerDecoder, TooManyTeeth_CausesError) {
	MockTriggerConfiguration cfg(true, {TT_TOOTHED_WHEEL, 4, 1});
	cfg.update();

	auto shape = makeTriggerShape(FOUR_STROKE_CAM_SENSOR, cfg);

	efitick_t t = 0;

	StrictMock<MockTriggerDecoder> dut;
	// We expect one call to onTriggerError().
	EXPECT_CALL(dut, onTriggerError());

	// Fire a few boring evenly spaced teeth
	t += MS2NT(1);
	doTooth(dut, shape, cfg, t);
	EXPECT_FALSE(dut.getShaftSynchronized());

	t += MS2NT(1);
	doTooth(dut, shape, cfg, t);
	EXPECT_FALSE(dut.getShaftSynchronized());

	t += MS2NT(1);
	doTooth(dut, shape, cfg, t);
	EXPECT_FALSE(dut.getShaftSynchronized());

	// Missing tooth, 2x normal length!
	t += MS2NT(2);
	doTooth(dut, shape, cfg, t);
	EXPECT_TRUE(dut.getShaftSynchronized());
	EXPECT_EQ(0, dut.currentCycle.current_index);

	// Fake that we have RPM so that all trigger error detection is enabled
	Sensor::setMockValue(SensorType::Rpm, 1000);

	t += MS2NT(1);
	doTooth(dut, shape, cfg, t);
	EXPECT_TRUE(dut.getShaftSynchronized());
	EXPECT_EQ(2, dut.currentCycle.current_index);

	t += MS2NT(1);
	doTooth(dut, shape, cfg, t);
	EXPECT_TRUE(dut.getShaftSynchronized());
	EXPECT_EQ(4, dut.currentCycle.current_index);
	EXPECT_EQ(0, dut.totalTriggerErrorCounter);

	// This tooth is extra - expect a call to onTriggerError() and loss of sync!
	t += MS2NT(1);
	doTooth(dut, shape, cfg, t);
	EXPECT_FALSE(dut.getShaftSynchronized());
	EXPECT_EQ(6, dut.currentCycle.current_index);
	EXPECT_EQ(1, dut.totalTriggerErrorCounter);

	Sensor::resetAllMocks();
}

TEST(TriggerDecoder, NotEnoughTeeth_CausesError) {
	MockTriggerConfiguration cfg(true, {TT_TOOTHED_WHEEL, 4, 1});
	cfg.update();

	auto shape = makeTriggerShape(FOUR_STROKE_CAM_SENSOR, cfg);

	efitick_t t = 0;

	StrictMock<MockTriggerDecoder> dut;
	// We expect one call to onTriggerError().
	EXPECT_CALL(dut, onTriggerError());

	// Fire a few boring evenly spaced teeth
	t += MS2NT(1);
	doTooth(dut, shape, cfg, t);
	EXPECT_FALSE(dut.getShaftSynchronized());

	t += MS2NT(1);
	doTooth(dut, shape, cfg, t);
	EXPECT_FALSE(dut.getShaftSynchronized());

	t += MS2NT(1);
	doTooth(dut, shape, cfg, t);
	EXPECT_FALSE(dut.getShaftSynchronized());

	// Missing tooth, 2x normal length!
	t += MS2NT(2);
	doTooth(dut, shape, cfg, t);
	EXPECT_TRUE(dut.getShaftSynchronized());
	EXPECT_EQ(0, dut.currentCycle.current_index);

	t += MS2NT(1);
	doTooth(dut, shape, cfg, t);
	EXPECT_TRUE(dut.getShaftSynchronized());
	EXPECT_EQ(2, dut.currentCycle.current_index);
	EXPECT_FALSE(dut.someSortOfTriggerError());
	EXPECT_EQ(0, dut.totalTriggerErrorCounter);

	// Missing tooth, but it comes early - not enough teeth have happened yet!
	t += MS2NT(2);
	doTooth(dut, shape, cfg, t);

	// Sync is lost until we get to another sync point
	EXPECT_FALSE(dut.getShaftSynchronized());
	EXPECT_EQ(0, dut.currentCycle.current_index);
	EXPECT_EQ(1, dut.totalTriggerErrorCounter);
	EXPECT_TRUE(dut.someSortOfTriggerError());
}

TEST(TriggerDecoder, PrimaryDecoderNoDisambiguation) {
	MockTriggerConfiguration cfg(true, {TT_TOOTHED_WHEEL, 4, 1});
	cfg.update();

	auto shape = makeTriggerShape(FOUR_STROKE_CAM_SENSOR, cfg);

	efitick_t t = 0;

	PrimaryTriggerDecoder dut("test");

	// This is not the right place for this, but further refactoring has to happen before it can get moved.
	dut.setNeedsDisambiguation(shape.needsDisambiguation());

	// Fire a few boring evenly spaced teeth
	t += MS2NT(1);
	doTooth(dut, shape, cfg, t);
	t += MS2NT(1);
	doTooth(dut, shape, cfg, t);
	t += MS2NT(1);
	doTooth(dut, shape, cfg, t);

	// Missing tooth, 2x normal length!
	t += MS2NT(2);
	doTooth(dut, shape, cfg, t);

	// Is synchronized
	EXPECT_TRUE(dut.getShaftSynchronized());
	// Has "full" sync, doesn't need cam information to sync
	EXPECT_TRUE(dut.hasSynchronizedPhase());
}

TEST(TriggerDecoder, PrimaryDecoderNeedsDisambiguation) {
	MockTriggerConfiguration cfg(true, {TT_TOOTHED_WHEEL, 4, 1});
	cfg.update();

	auto shape = makeTriggerShape(FOUR_STROKE_CRANK_SENSOR, cfg);

	efitick_t t = 0;

	PrimaryTriggerDecoder dut("test");

	// This is not the right place for this, but further refactoring has to happen before it can get moved.
	dut.setNeedsDisambiguation(shape.needsDisambiguation());

	// Fire a few boring evenly spaced teeth
	t += MS2NT(1);
	doTooth(dut, shape, cfg, t);
	t += MS2NT(1);
	doTooth(dut, shape, cfg, t);
	t += MS2NT(1);
	doTooth(dut, shape, cfg, t);

	// Missing tooth, 2x normal length!
	t += MS2NT(2);
	doTooth(dut, shape, cfg, t);

	// Is synchronized
	EXPECT_TRUE(dut.getShaftSynchronized());
	// Does not have full sync, this trigger is ambiguous
	EXPECT_FALSE(dut.hasSynchronizedPhase());

	// Provide cam assist information to the primary trigger
	dut.syncEnginePhase(2, 0, 720);

	// We now have full sync!
	EXPECT_TRUE(dut.hasSynchronizedPhase());

	// If there's a trigger error, we lose full sync
	// Tests above ensure onTriggerError() is called properly
	dut.onTriggerError();
	EXPECT_FALSE(dut.hasSynchronizedPhase());
}
