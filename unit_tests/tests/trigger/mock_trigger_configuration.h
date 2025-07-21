#pragma once

class MockTriggerConfiguration : public TriggerConfiguration {
public:
	MockTriggerConfiguration(trigger_config_s type)
		: TriggerConfiguration("Mock")
		, m_type(type)
	{ }

protected:
	bool isVerboseTriggerSynchDetails() const override {
		return false;
	}

	trigger_config_s getType() const override {
		return m_type;
	}

private:
	const trigger_config_s m_type;
};
