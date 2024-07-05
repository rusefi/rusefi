#pragma once

#include "pch.h"

struct LuaPid final {
	LuaPid() = default;

	LuaPid(float kp, float ki, float kd, float min, float max)
		: m_pid(&m_params)
	{
		m_params.pFactor = kp;
		m_params.iFactor = ki;
		m_params.dFactor = kd;

		m_params.offset = 0;
		m_params.periodMs = 0;
		m_params.minValue = min;
		m_params.maxValue = max;

		m_lastUpdate.reset();
	}

	float get(float target, float input) {
#if EFI_UNIT_TEST
		// this is how we avoid zero dt
		advanceTimeUs(1000);
#endif

		float dt = m_lastUpdate.getElapsedSecondsAndReset(getTimeNowNt());

		return m_pid.getOutput(target, input, dt);
	}

	void setOffset(float offset) {
		m_params.offset = offset;
		reset();
	}

	void reset() {
		m_pid.reset();
	}

private:
	Pid m_pid;
	Timer m_lastUpdate;
	pid_s m_params;
};

// todo: use templates and reduce duplication between LuaPid and LuaIndustrialPid?
struct LuaIndustrialPid final {
	LuaIndustrialPid() = default;

	LuaIndustrialPid(float kp, float ki, float kd, float min, float max)
		: m_pid(&m_params)
	{
		m_params.pFactor = kp;
		m_params.iFactor = ki;
		m_params.dFactor = kd;

		m_params.offset = 0;
		m_params.periodMs = 0;
		m_params.minValue = min;
		m_params.maxValue = max;

		m_lastUpdate.reset();
	}

	float get(float target, float input) {
#if EFI_UNIT_TEST
		// this is how we avoid zero dt
		advanceTimeUs(1000);
#endif

		float dt = m_lastUpdate.getElapsedSecondsAndReset(getTimeNowNt());

		return m_pid.getOutput(target, input, dt);
	}

	void setOffset(float offset) {
		m_params.offset = offset;
		reset();
	}

	void setDerivativeFilterLoss(float derivativeFilterLoss) {
		m_pid.derivativeFilterLoss = derivativeFilterLoss;
		reset();
	}

	void setAntiwindupFreq(float antiwindupFreq) {
		m_pid.antiwindupFreq = antiwindupFreq;
		reset();
	}

	void reset() {
		m_pid.reset();
	}

private:
	PidIndustrial m_pid;
	Timer m_lastUpdate;
	pid_s m_params;
};
