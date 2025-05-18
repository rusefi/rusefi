#include "pch.h"
#include "closed_loop_idle.h"
#include "engine_math.h"
#include "efitime.h"
#include "tunerstudio.h"
#include "event_queue.h"
#include "efi_gpio.h"
#include "engine.h"

LongTermIdleTrim::LongTermIdleTrim() {
    // Inicializa a partir dos valores persistidos, se disponíveis
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            if (config->ltitTable[i][j] > 0) {
                ltitTableHelper[i][j] = (float)config->ltitTable[i][j];
            } else {
                ltitTableHelper[i][j] = 100.0f;
            }
        }
    }
    acTrim = (float)config->ltitAcTrim / 10.0f;
    fan1Trim = (float)config->ltitFan1Trim / 10.0f;
    fan2Trim = (float)config->ltitFan2Trim / 10.0f;
    emaError = 0;
    lastUpdateTime = 0;
}

float LongTermIdleTrim::getLtitFactor(float rpm, float clt) const {
    // Interpolação 3D:
    return interpolate3d(ltitTableHelper, config->cltIdleCorrBins, clt, config->rpmIdleCorrBins, rpm) * 0.01f;
}
float LongTermIdleTrim::getLtitAcTrim() const { return acTrim; }
float LongTermIdleTrim::getLtitFan1Trim() const { return fan1Trim; }
float LongTermIdleTrim::getLtitFan2Trim() const { return fan2Trim; }

void LongTermIdleTrim::update(float rpm, float clt, bool acActive, bool fan1Active, bool fan2Active) {
    // Critério de idle estável
    float targetRpm = interpolate2d(clt, config->cltIdleCorrBins, config->rpmIdleCorrBins);
    if (fabsf(rpm - targetRpm) > engineConfiguration->ltitStableRpmThreshold)
        return;
    static uint32_t stableStart = 0;
    uint32_t now = getTimeNowS();
    if (stableStart == 0)
        stableStart = now;
    if ((now - stableStart) < engineConfiguration->ltitStableTime)
        return;
    // Filtro EMA do erro (exemplo: erro = rpm - target)
    float error = rpm - targetRpm;
    float alpha = (float)engineConfiguration->ltitEmaAlpha / 255.0f;
    emaError = alpha * error + (1.0f - alpha) * emaError;
    // Encontrar índices dos bins
    int i = 0, j = 0;
    for (int idx = 0; idx < 16; idx++) {
        if (clt < config->cltIdleCorrBins[idx]) { i = idx > 0 ? idx - 1 : 0; break; }
    }
    for (int idx = 0; idx < 16; idx++) {
        if (rpm < config->rpmIdleCorrBins[idx]) { j = idx > 0 ? idx - 1 : 0; break; }
    }
    // Correção multiplicativa
    float correction = -emaError * (float)engineConfiguration->ltitCorrectionRate * 0.01f;
    ltitTableHelper[i][j] *= (1.0f + correction);
    // Clamping configurável
    float clampMin = engineConfiguration->ltitClampMin > 0 ? engineConfiguration->ltitClampMin : 50.0f;
    float clampMax = engineConfiguration->ltitClampMax > 0 ? engineConfiguration->ltitClampMax : 150.0f;
    if (ltitTableHelper[i][j] > clampMax) ltitTableHelper[i][j] = clampMax;
    if (ltitTableHelper[i][j] < clampMin) ltitTableHelper[i][j] = clampMin;

    // --- Lógica de trims AC/Fan: só aprende se RPM estável com carga ativa por tempo mínimo ---
    static uint32_t acStableStart = 0, fan1StableStart = 0, fan2StableStart = 0;
    static bool acWasActive = false, fan1WasActive = false, fan2WasActive = false;
    // AC
    if (acActive) {
        if (!acWasActive) acStableStart = now;
        if ((now - acStableStart) >= engineConfiguration->ltitStableTime && fabsf(rpm - targetRpm) <= engineConfiguration->ltitStableRpmThreshold) {
            acTrim += correction;
        }
        acWasActive = true;
    } else {
        acWasActive = false;
    }
    // Fan1
    if (fan1Active) {
        if (!fan1WasActive) fan1StableStart = now;
        if ((now - fan1StableStart) >= engineConfiguration->ltitStableTime && fabsf(rpm - targetRpm) <= engineConfiguration->ltitStableRpmThreshold) {
            fan1Trim += correction;
        }
        fan1WasActive = true;
    } else {
        fan1WasActive = false;
    }
    // Fan2
    if (fan2Active) {
        if (!fan2WasActive) fan2StableStart = now;
        if ((now - fan2StableStart) >= engineConfiguration->ltitStableTime && fabsf(rpm - targetRpm) <= engineConfiguration->ltitStableRpmThreshold) {
            fan2Trim += correction;
        }
        fan2WasActive = true;
    } else {
        fan2WasActive = false;
    }

    // --- NOVA LÓGICA DE SALVAMENTO PADRÃO ---
    static bool ignitionOffSaveDelayActive = false;
    static uint32_t ignitionOffTimestamp = 0;
    static bool pendingSave = false;
    // Estado de ignição: true = ON, false = OFF
    bool ignitionState = engine->module<IgnitionController>()->secondsSinceIgnVoltage() > 1.0f;
    if (!ignitionState) {
        if (!ignitionOffSaveDelayActive) {
            ignitionOffTimestamp = now;
            ignitionOffSaveDelayActive = true;
            pendingSave = true;
        }
        if (ignitionOffSaveDelayActive && (now - ignitionOffTimestamp) >= engineConfiguration->ltitIgnitionOffSaveDelay) {
            if (pendingSave) {
                // Copiar para config e marcar para salvar
                for (int x = 0; x < 16; x++)
                    for (int y = 0; y < 16; y++)
                        config->ltitTable[x][y] = (uint16_t)ltitTableHelper[x][y];
                config->ltitAcTrim = (int16_t)(acTrim * 10.0f);
                config->ltitFan1Trim = (int16_t)(fan1Trim * 10.0f);
                config->ltitFan2Trim = (int16_t)(fan2Trim * 10.0f);
                // Suavização regional após salvamento
                smoothLtitTable(engineConfiguration->ltitSmoothingIntensity);
                setNeedToWriteConfiguration();
                pendingSave = false;
            }
            ignitionOffSaveDelayActive = false;
        }
    } else {
        ignitionOffSaveDelayActive = false;
        pendingSave = false;
    }
}

// Suavização regional da tabela LTIT
void LongTermIdleTrim::smoothLtitTable(float intensity) {
    float temp[16][16];
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            float sum = ltitTableHelper[i][j];
            int count = 1;
            if (i > 0)   { sum += ltitTableHelper[i-1][j]; count++; }
            if (i < 15)  { sum += ltitTableHelper[i+1][j]; count++; }
            if (j > 0)   { sum += ltitTableHelper[i][j-1]; count++; }
            if (j < 15)  { sum += ltitTableHelper[i][j+1]; count++; }
            float avg = sum / count;
            temp[i][j] = ltitTableHelper[i][j] * (1.0f - intensity) + avg * intensity;
        }
    }
    for (int i = 0; i < 16; i++)
        for (int j = 0; j < 16; j++)
            ltitTableHelper[i][j] = temp[i][j];
} 