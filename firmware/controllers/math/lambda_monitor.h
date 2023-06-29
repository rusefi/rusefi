#pragma once

struct LambdaMonitorBase {
	void update(float rpm, float load);
	bool isCut() const;

protected:
	// Returns whether lambda checking should happen at all
	bool shouldCheckLambda(float rpm, float load) const;

	// Returns false if the current lambda reading is leaner than allowable.
	// Returns true in any other case (rich enough, bad sensor, recent fuel cut, rpm to low, load too low, etc)
	bool isCurrentlyGood(float rpm, float load) const;
	virtual float getMaxAllowedLambda(float rpm, float load) const = 0;

	// Determine whether fuel should be restored after a cut occurs
	// Returns true if OK to leave the "cut" state
	bool restoreConditionsMet(float rpm, float load) const;

private:
	Timer m_timeSinceGoodLambda;
	bool m_isCut = false;
};

class LambdaMonitor : public LambdaMonitorBase {
	float getMaxAllowedLambda(float rpm, float load) const override;
};
