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
    // Clamping
    if (ltitTableHelper[i][j] > 150.0f) ltitTableHelper[i][j] = 150.0f;
    if (ltitTableHelper[i][j] < 50.0f) ltitTableHelper[i][j] = 50.0f;
    // Trims adaptativos
    if (acActive) acTrim += correction;
    if (fan1Active) fan1Trim += correction;
    if (fan2Active) fan2Trim += correction;
    // Persistência: exemplo simples (pode ser expandido)
    static uint32_t lastSave = 0;
    if (now - lastSave > engineConfiguration->ltitIgnitionOffSaveDelay) {
        // Copiar para config e marcar para salvar
        for (int x = 0; x < 16; x++)
            for (int y = 0; y < 16; y++)
                config->ltitTable[x][y] = (uint16_t)ltitTableHelper[x][y];
        config->ltitAcTrim = (int16_t)(acTrim * 10.0f);
        config->ltitFan1Trim = (int16_t)(fan1Trim * 10.0f);
        config->ltitFan2Trim = (int16_t)(fan2Trim * 10.0f);
        setNeedToWriteConfiguration();
        lastSave = now;
    }
} 