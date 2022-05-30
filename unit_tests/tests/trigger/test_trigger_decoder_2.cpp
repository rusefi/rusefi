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
	MOCK_METHOD(void, onTriggerError, (), (override));
};

static auto makeTriggerShape(operation_mode_e mode, const TriggerConfiguration& config) {
	TriggerWaveform shape;
	shape.initializeTriggerWaveform(mode, true, config);

	return shape;
}

#define doTooth(dut, shape, cfg, t) dut.decodeTriggerEvent("", shape, nullptr, nullptr, cfg, SHAFT_PRIMARY_RISING, t)

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

	t += MS2NT(1);
	doTooth(dut, shape, cfg, t);
	EXPECT_TRUE(dut.getShaftSynchronized());
	EXPECT_EQ(2, dut.currentCycle.current_index);

	t += MS2NT(1);
	doTooth(dut, shape, cfg, t);
	EXPECT_TRUE(dut.getShaftSynchronized());
	EXPECT_EQ(4, dut.currentCycle.current_index);

	// This tooth is extra - expect a call to onTriggerError() and loss of sync!
	t += MS2NT(1);
	doTooth(dut, shape, cfg, t);
	EXPECT_FALSE(dut.getShaftSynchronized());
	EXPECT_EQ(6, dut.currentCycle.current_index);
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

	// Missing tooth, but it comes early - not enough teeth have happened yet!
	t += MS2NT(2);
	doTooth(dut, shape, cfg, t);

	// Sync is lost until we get to another sync point
	EXPECT_FALSE(dut.getShaftSynchronized());
	EXPECT_EQ(0, dut.currentCycle.current_index);
	EXPECT_TRUE(dut.someSortOfTriggerError());
}
