#include "pch.h"
#include "closed_loop_idle.h"
#include "engine_math.h"
#include "efitime.h"
#include "tunerstudio.h"
#include "event_queue.h"
#include "efi_gpio.h"
#include "engine.h"

LongTermIdleTrim::LongTermIdleTrim() {
    // Verificar CRC ou inicializar a tabela apenas uma vez
    if (!ltitTableInitialized) {
        copyTable(ltitTableHelper, config->ltitTable, 1);
        acTrim = (float)config->ltitAcTrim / 10.0f;
        fan1Trim = (float)config->ltitFan1Trim / 10.0f;
        fan2Trim = (float)config->ltitFan2Trim / 10.0f;
        ltitTableInitialized = true;
    }
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

void LongTermIdleTrim::update(float rpm, float clt, bool acActive, bool fan1Active, bool fan2Active, float idleIntegral) {
    if (!engineConfiguration->ltitEnabled)
        return;
        
    (void)acActive; (void)fan1Active; (void)fan2Active;

	if(!ltitTableInitialized){
		return;
	}

    // Encontrar índices dos bins
    int i = 0, j = 0;
    for (int idx = 0; idx < 16; idx++) {
        if (clt < config->cltIdleCorrBins[idx]) { i = idx > 0 ? idx - 1 : 0; break; }
    }
    for (int idx = 0; idx < 16; idx++) {
        if (rpm < config->rpmIdleCorrBins[idx]) { j = idx > 0 ? idx - 1 : 0; break; }
    }
    // Correção multiplicativa baseada no termo integral do PID
    float correction = idleIntegral * (float)engineConfiguration->ltitCorrectionRate * 0.01f;
    ltitTableHelper[i][j] *= (1.0f + correction);
    // Clamping configurável
    float clampMin = engineConfiguration->ltitClampMin > 0 ? engineConfiguration->ltitClampMin : 50.0f;
    float clampMax = engineConfiguration->ltitClampMax > 0 ? engineConfiguration->ltitClampMax : 150.0f;
    if (ltitTableHelper[i][j] > clampMax) ltitTableHelper[i][j] = clampMax;
    if (ltitTableHelper[i][j] < clampMin) ltitTableHelper[i][j] = clampMin;
    // Remover trims AC/Fan com timers
    //acTrim, fan1Trim, fan2Trim podem ser ajustados diretamente se necessário
    // Após atualização da tabela, marcar aprendizado pendente
    updatedLtit = true;
    smoothLtitTable(engineConfiguration->ltitSmoothingIntensity);
}

void LongTermIdleTrim::onIgnitionStateChanged(bool ignitionOn) {
    if (!ignitionOn && updatedLtit) {
        // Salvar na memória persistente imediatamente quando desligar a ignição
        copyTable(config->ltitTable, ltitTableHelper);
        config->ltitAcTrim = (int16_t)(acTrim * 10.0f);
        config->ltitFan1Trim = (int16_t)(fan1Trim * 10.0f);
        config->ltitFan2Trim = (int16_t)(fan2Trim * 10.0f);
        setNeedToWriteConfiguration();
        
        updatedLtit = false;
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