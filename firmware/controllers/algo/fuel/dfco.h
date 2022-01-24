#pragma once

class DfcoController : public EngineModule {
public:
	void update();

	bool cutFuel() const;

private:
	bool getState() const;
	bool m_isDfco = false;
};
