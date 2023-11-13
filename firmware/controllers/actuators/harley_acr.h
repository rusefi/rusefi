#pragma once

class HarleyAcr : public EngineModule {
public:
    void updateAcr();
	void onSlowCallback() override;

	bool isActive() const;

private:
	bool m_active = false;
};
