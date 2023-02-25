#pragma once

class HarleyAcr : public EngineModule {
public:
	void onSlowCallback() override;

	bool isActive() const;

private:
	bool m_active = false;
};
