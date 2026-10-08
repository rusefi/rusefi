#include "pch.h"
#include "injector_deadtime_autotune.h"

void InjectorDeadtimeAutotune::onSlowCallback() {
}

#if EFI_ENGINE_CONTROL && EFI_LUA && (defined(STM32F7) || EFI_UNIT_TEST)

bool InjectorDeadtimeAutotune::canTune() const {
	return !m_luaError && !hasFirmwareError() && engine->rpmCalculator.isRunning()
		&& engine->triggerCentral.triggerState.hasSynchronizedPhase()
		&& engineConfiguration->cylindersCount > 1
		&& (engineConfiguration->injectionMode == IM_SEQUENTIAL || engineConfiguration->injectionMode == IM_BATCH)
		&& !engineConfiguration->enableStagedInjection
		&& !engineConfiguration->useInjectorFlowLinearizationTable
		&& engineConfiguration->injectorNonlinearMode == INJ_None;
}

bool InjectorDeadtimeAutotune::setInjectionMode(int mode) {
	chibios_rt::CriticalSectionLocker lock;
	if (mode == -1) {
		m_requestedMode = -1;
		return true;
	}
	if ((mode != IM_SEQUENTIAL && mode != IM_BATCH) || !canTune()) {
		return false;
	}
	m_requestedMode = mode;
	m_modeExpires = getTimeNowNt() + NT_PER_SECOND;
	return true;
}

bool InjectorDeadtimeAutotune::setDeadtimeAdd(float milliseconds) {
	chibios_rt::CriticalSectionLocker lock;
	if (!std::isfinite(milliseconds) || std::abs(milliseconds) > MaxDeadtimeAddMs) {
		return false;
	}
	if (milliseconds != 0 && !canTune()) {
		return false;
	}
	m_deadtimeAdd = milliseconds;
	m_deadtimeExpires = getTimeNowNt() + NT_PER_SECOND;
	return true;
}

bool InjectorDeadtimeAutotune::setLtftLearningDisabled(bool disabled) {
	chibios_rt::CriticalSectionLocker lock;
	if (disabled && !canTune()) {
		return false;
	}
	m_disableLtftLearning = disabled;
	m_ltftExpires = getTimeNowNt() + NT_PER_SECOND;
	return true;
}

float InjectorDeadtimeAutotune::getDeadtimeAdd() const {
	chibios_rt::CriticalSectionLocker lock;
	return m_deadtimeAdd != 0 && getTimeNowNt() < m_deadtimeExpires && canTune() ? m_deadtimeAdd : 0;
}

bool InjectorDeadtimeAutotune::isLtftLearningDisabled() const {
	chibios_rt::CriticalSectionLocker lock;
	return m_disableLtftLearning && getTimeNowNt() < m_ltftExpires && canTune();
}

injection_mode_e InjectorDeadtimeAutotune::getInjectionMode(injection_mode_e configured) const {
	chibios_rt::CriticalSectionLocker lock;
	// Release is coordinated with queued pulses in begin/endFastCallback.
	return m_appliedMode < 0 ? configured : static_cast<injection_mode_e>(m_appliedMode);
}

bool InjectorDeadtimeAutotune::isTransitioning() const {
	return isSchedulingBlocked();
}

bool InjectorDeadtimeAutotune::hasModeOverride() const {
	chibios_rt::CriticalSectionLocker lock;
	return m_appliedMode >= 0 || m_requestedMode >= 0;
}

bool InjectorDeadtimeAutotune::isSchedulingBlocked() const {
	chibios_rt::CriticalSectionLocker lock;
	return m_blockScheduling || m_requestedMode != m_appliedMode
		|| (m_appliedMode >= 0 && (!canTune() || getTimeNowNt() >= m_modeExpires));
}

void InjectorDeadtimeAutotune::clearRequests() {
	m_requestedMode = -1;
	m_deadtimeAdd = 0;
	m_disableLtftLearning = false;
	dtAutotuneActive = false;
}

void InjectorDeadtimeAutotune::resetLua() {
	chibios_rt::CriticalSectionLocker lock;
	clearRequests();
	m_luaError = false;
}

void InjectorDeadtimeAutotune::onLuaError() {
	chibios_rt::CriticalSectionLocker lock;
	clearRequests();
	// An onTick failure normally leaves the VM running. Require reload before rearming.
	m_luaError = true;
}

void InjectorDeadtimeAutotune::onEngineStop() {
	chibios_rt::CriticalSectionLocker lock;
	clearRequests();
}

void InjectorDeadtimeAutotune::onConfigurationChange(engine_configuration_s const*) {
	chibios_rt::CriticalSectionLocker lock;
	clearRequests();
}

void InjectorDeadtimeAutotune::beginFastCallback() {
	chibios_rt::CriticalSectionLocker lock;
	const auto now = getTimeNowNt();
	if (!canTune()) {
		clearRequests();
	}
	if (now >= m_modeExpires) {
		m_requestedMode = -1;
	}
	if (now >= m_deadtimeExpires) {
		m_deadtimeAdd = 0;
	}
	if (now >= m_ltftExpires) {
		m_disableLtftLearning = false;
	}

	m_blockScheduling = m_appliedMode != m_requestedMode;
	if (m_blockScheduling && m_pendingCloses == 0) {
		m_appliedMode = m_requestedMode;
		m_rebuild = true;
		engine->injectionEvents.invalidate();
	}
	dtAutotuneActive = m_requestedMode >= 0 || m_deadtimeAdd != 0 || m_disableLtftLearning;
}

void InjectorDeadtimeAutotune::endFastCallback() {
	chibios_rt::CriticalSectionLocker lock;
	if (m_rebuild) {
		// No old pulse remains. Publish freshly computed fuel and mappings together.
		m_blockScheduling = false;
		engine->injectionEvents.addFuelEvents();
		m_rebuild = false;
	}
}

void InjectorDeadtimeAutotune::injectionScheduled(unsigned closes) {
	chibios_rt::CriticalSectionLocker lock;
	m_pendingCloses += closes;
}

void InjectorDeadtimeAutotune::injectionClosed() {
	chibios_rt::CriticalSectionLocker lock;
	if (m_pendingCloses) {
		m_pendingCloses--;
	}
}

#endif
