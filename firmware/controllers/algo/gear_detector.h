
class GearDetector : public EngineModule {
public:
	void onSlowCallback() override;

	float getGearboxRatio() const;

private:
	float computeGearboxRatio() const;

	float m_gearboxRatio = 0;
};
