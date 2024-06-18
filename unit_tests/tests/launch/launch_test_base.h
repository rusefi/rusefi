//
// Created by kifir on 6/12/24.
//

#pragma once

#include "util/test_base.h"

class LaunchTestConfig {
public:
    std::optional<bool> getLaunchControlEnabled() const { return m_launchControlEnabled; }

    std::optional<int> getLaunchRpm() const { return m_launchRpm; }
    std::optional<int> getLaunchRpmWindow() const { return m_launchRpmWindow; }
    std::optional<int> getLaunchCorrectionsEndRpm() const { return m_launchCorrectionsEndRpm; }

    std::optional<bool> getIgnitionRetardEnable() const { return m_ignitionRetardEnable; }
    std::optional<float> getIgnitionRetard() const { return m_ignitionRetard; }
    std::optional<bool> getSmoothRetardMode() const { return m_smoothRetardMode; }

    std::optional<bool> getEnableIgnitionCut() const { return m_enableIgnitionCut; }
    std::optional<int> getInitialIgnitionCut() const { return m_initialIgnitionCut; }
    std::optional<int> getFinalIgnitionCutBeforeLaunch() const { return m_finalIgnitionCutBeforeLaunch; }

    bool getSatisfyActivationSwithSpeedAndTpsConditions() const {
        return m_satisfyActivationSwithSpeedAndTpsConditions;
    }
protected:
    void setLaunchControlEnabled(const std::optional<bool> value) { m_launchControlEnabled = value; }

    void setLaunchRpm(const std::optional<int> value) { m_launchRpm = value; }
    void setLaunchRpmWindow(const std::optional<int> value) { m_launchRpmWindow = value; }
    void setLaunchCorrectionsEndRpm(const std::optional<int> value) { m_launchCorrectionsEndRpm = value; }

    void setIgnitionRetardEnable(const std::optional<bool> value) { m_ignitionRetardEnable = value; }
    void setIgnitionRetard(const std::optional<float> value) { m_ignitionRetard = value; }
    void setSmoothRetardMode(const std::optional<bool> value) { m_smoothRetardMode = value; }

    void setEnableIgnitionCut(const std::optional<bool> value) { m_enableIgnitionCut = value; }
    void setInitialIgnitionCut(const std::optional<int> value) { m_initialIgnitionCut = value; }
    void setFinalIgnitionCutBeforeLaunch(const std::optional<int> value) { m_finalIgnitionCutBeforeLaunch = value; }

    void setSatisfyActivationSwithSpeedAndTpsConditions(const bool value) {
        m_satisfyActivationSwithSpeedAndTpsConditions = value;
    }
private:
    std::optional<bool> m_launchControlEnabled;

    std::optional<int> m_launchRpm;
    std::optional<int> m_launchRpmWindow;
    std::optional<int> m_launchCorrectionsEndRpm;

    std::optional<bool> m_ignitionRetardEnable;
    std::optional<float> m_ignitionRetard;
    std::optional<bool> m_smoothRetardMode;

    std::optional<bool> m_enableIgnitionCut;
    std::optional<int> m_initialIgnitionCut;
    std::optional<int> m_finalIgnitionCutBeforeLaunch;

    bool m_satisfyActivationSwithSpeedAndTpsConditions { false };
};

class LaunchTestBase : public TestBase {
protected:
    void setUpTestConfig(const LaunchTestConfig& config);

    void updateRpm(const int rpm);
private:
    void configureLaunchControlEnabled(std::optional<bool> launchControlEnabled);

    void configureLaunchRpm(std::optional<int> launchRpm);
    void configureLaunchRpmWindow(std::optional<int> launchRpmWindow);
    void configureLaunchCorrectionsEndRpm(std::optional<int> launchCorrectionsEndRpm);

    void configureIgnitionRetardEnable(std::optional<bool> ignitionRetardEnable);
    void configureIgnitionRetard(std::optional<float> ignitionRetard);
    void configureSmoothRetardMode(std::optional<bool> smoothRetardMode);

    void configureEnableIgnitionCut(std::optional<bool> enableIgnitionCut);
    void configureInitialIgnitionCutPercent(std::optional<int> initialIgnitionCut);
    void configureFinalIgnitionCutPercentBeforeLaunch(std::optional<int> finalIgnitionCutBeforeLaunch);

    void configureSatisfiedActivationSwithSpeedAndTpsConditions();
};