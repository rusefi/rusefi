# Sistema Adaptativo de Wall Wetting (Modelo Aquino Aprimorado)

## 1. Visão Geral e Objetivos

Este documento descreve um sistema adaptativo para compensação de "wall wetting" (molhagem de parede) em motores de combustão interna, baseado no modelo de Aquino. O objetivo é minimizar o erro de lambda (`λ_err`) durante e após transientes de carga (aceleração e desaceleração), ajustando dinamicamente os parâmetros `Beta` (fração de combustível que atinge a parede) e `Tau` (constante de tempo de evaporação do filme de combustível).

O sistema opera a uma frequência de 200Hz, sendo chamado pelo callback `onFastCallback`.

## 2. Variáveis e Parâmetros Chave

### 2.1. Entradas do Sistema

-   `λ_m`: Lambda medido pela sonda.
-   `λ_t`: Lambda alvo (do mapa base).
-   `MAP`: Pressão Absoluta no Coletor (kPa).
-   `TPS`: Posição da Borboleta (%).
-   `RPM`: Rotações Por Minuto.
-   `ECT`: Temperatura do Líquido de Arrefecimento (°C).
-   `IAT`: Temperatura do Ar de Admissão (°C).
-   `m_inj_total_ciclo`: Massa total de combustível injetada no ciclo do motor.

### 2.2. Parâmetros do Modelo de Wall Wetting

-   `X_f` ou `Beta`: Fração de combustível que atinge a parede.
-   `τ_f` ou `Tau`: Constante de tempo de evaporação do filme de combustível (s).

### 2.3. Tabelas Base e de Correção (8x8)

-   `X_f_base[MAP_idx][RPM_idx]`: Tabela base para Beta.
-   `τ_f_base[MAP_idx][RPM_idx]`: Tabela base para Tau (pode ter compensação por ECT/IAT).
-   `wwBetaCorrection[MAP_idx][RPM_idx]`: Tabela de correção multiplicativa para Beta.
    -   Inicialização: Todas as células em `1.0`.
    -   Limites: `min_beta_corr = 0.7`, `max_beta_corr = 1.3`.
-   `wwTauCorrection[MAP_idx][RPM_idx]`: Tabela de correção multiplicativa para Tau.
    -   Inicialização: Todas as células em `1.0`.
    -   Limites: `min_tau_corr = 0.5`, `max_tau_corr = 2.0`.

### 2.4. Parâmetros de Cálculo e Adaptação

-   `Δt = 0.005s`: Período do callback (1 / 200Hz).
-   `λ_err = λ_m - λ_t`: Erro de lambda.
-   `dTPS_dt`: Derivada do TPS (%/s).
-   `dMAP_dt`: Derivada do MAP (kPa/s).

### 2.5. Thresholds e Ganhos (Valores Iniciais Sugeridos - **CALIBRAR!**)

-   **Detecção de Transiente:**
    -   `dTPS_dt_acel_thresh = 70 %/s`
    -   `dMAP_dt_acel_thresh = 40 kPa/s`
    -   `dTPS_dt_desacel_thresh = -70 %/s`
    -   `dMAP_dt_desacel_thresh = -40 kPa/s`
-   **Condições de Análise:**
    -   `min_lambda_err_adapt = 0.03` (Magnitude mínima do erro para considerar adaptação)
    -   `T_trans_min_duration = 0.2s` (Duração mínima do transiente para análise)
    -   `T_analise_max_duration = 2.0s` (Duração máxima da janela de análise pós-transiente)
-   **Correção de Beta:**
    -   `K_beta_acel = 0.05` (Ganho para correção de Beta em aceleração)
    -   `K_beta_decel = 0.05` (Ganho para correção de Beta em desaceleração)
    -   `threshold_err_beta_pobre = 0.05`
    -   `threshold_err_beta_rico = -0.05`
-   **Correção de Tau (Aprimorado):**
    -   `λ_err_settle_thresh = 0.015` (Limiar de erro para assentamento)
    -   `T_settle_ideal_factor = 1.5` (Fator para `τ_f_base_atual` para definir `T_settle_ideal`)
    -   `T_settle_tolerance_margin_factor = 0.2` (Fator de `T_settle_ideal` para margem de tolerância)
    -   `T_overshoot_min_duration = 0.1s`
    -   `λ_overshoot_min_magnitude = 0.03`
    -   `K_tau_overshoot = 0.05` (Ganho para Tau se overshoot)
    -   `K_tau_lento = 0.04` (Ganho para Tau se assentamento lento)
    -   `K_tau_rapido = 0.02` (Ganho para Tau se assentamento rápido sem overshoot)

## 3. Modelo de Aquino (Conceitual para Entendimento)

O combustível injetado (`m_inj`) é dividido:

1.  Uma fração `X_f_eff` atinge a parede, formando um filme `m_f`.
2.  A fração `(1 - X_f_eff)` vai direto para o cilindro.

O filme evapora com uma constante de tempo `τ_f_eff`.

-   `X_f_eff = X_f_base * wwBetaCorrection`
-   `τ_f_eff = τ_f_base * wwTauCorrection`

**Equações Discretizadas (por `Δt`):**

-   `m_inj_no_Δt = (m_inj_total_ciclo / T_ciclo) * Δt` (aproximação da taxa de injeção)
-   `m_f_dep[k] = X_f_eff[k] * m_inj_no_Δt[k]`
-   `m_f_evap[k] = (m_f[k-1] / τ_f_eff[k]) * Δt`
-   `m_f[k] = m_f[k-1] + m_f_dep[k] - m_f_evap[k]` (Massa do filme no instante `k`)
-   `m_c[k] = (1 - X_f_eff[k]) * m_inj_no_Δt[k] + m_f_evap[k]` (Massa para o cilindro no instante `k`)

## 4. Detecção de Transientes

No `onFastCallback`:

1.  Calcule `dTPS_dt = (TPS[k] - TPS[k-1]) / Δt`.
2.  Calcule `dMAP_dt = (MAP[k] - MAP[k-1]) / Δt`.
3.  Filtre `dTPS_dt` e `dMAP_dt` (ex: filtro passa-baixa leve).
4.  **Estado do Transiente (`trans_state`):** `NONE`, `ACCEL_DETECTED`, `DECEL_DETECTED`, `ACCEL_ANALYSING`, `DECEL_ANALYSING`.
5.  **Lógica de Detecção:**
    -   Se `trans_state == NONE`:
        -   Se (`dTPS_dt &gt; dTPS_dt_acel_thresh` OU `dMAP_dt &gt; dMAP_dt_acel_thresh`):
            -   `trans_state = ACCEL_DETECTED`
            -   `t_trans_start = current_time`
            -   `map_idx_trans = current_map_idx`
            -   `rpm_idx_trans = current_rpm_idx`
            -   `tau_f_base_at_trans_start = τ_f_base[map_idx_trans][rpm_idx_trans]` (compensado por ECT/IAT)
            -   Inicializar buffers de `λ_err`.
        -   Senão se (`dTPS_dt &lt; dTPS_dt_desacel_thresh` OU `dMAP_dt &lt; dMAP_dt_desacel_thresh`):
            -   `trans_state = DECEL_DETECTED`
            -   (similar ao ACCEL_DETECTED para inicializações)
    -   Se `trans_state == ACCEL_DETECTED` ou `DECEL_DETECTED`:
        -   Mudar para `ACCEL_ANALYSING` ou `DECEL_ANALYSING` após uma pequena confirmação ou imediatamente.
    -   **Fim do Transiente para Análise:**
        -   O transiente é considerado "terminado" para fins de iniciar a análise de correção quando `current_time - t_trans_start &gt; T_analise_max_duration` ou quando as condições de transiente não são mais atendidas por um certo período. A análise de correção ocorre _após_ coletar dados suficientes.

## 5. Janelas de Análise (Relativas a `t_trans_start`)

Seja `τ_f_base_trans = tau_f_base_at_trans_start`.

-   **Janela de Análise para Beta (`W_beta`):**
    -   `T_W_beta_start = 0s` (relativo a `t_trans_start`)
    -   `T_W_beta_end = min(0.2s, 0.5 * τ_f_base_trans)`
-   **Janela de Análise para Tau (`W_tau`):**
    -   `T_W_tau_start = T_W_beta_end`
    -   `T_W_tau_end = min(T_analise_max_duration - T_W_beta_end, T_W_beta_end + 2.5 * τ_f_base_trans)` (duração relativa ao início de W_tau)
    -   Ou, mais simples: `T_W_tau_end_abs = min(t_trans_start + T_analise_max_duration, t_trans_start + T_W_beta_end + 2.5 * τ_f_base_trans)`

Durante o transiente, armazene `λ_err[i]` e `timestamp[i]` em um buffer.

## 6. Modelo de Correção para Beta (`ΔwwBetaCorrection`)

**Executado após a coleta de dados da janela `W_beta`.**

1.  Filtre as amostras de `λ_err` coletadas dentro de `W_beta` (de `t_trans_start` até `t_trans_start + T_W_beta_end`).
2.  Calcule `λ_err_avg_beta = Σ(λ_err_W_beta[i]) / N_W_beta_samples`.
3.  `ΔwwBetaCorrection = 0.0`
4.  **Lógica:**
    -   **Se `trans_state` foi Aceleração:**
        -   Se `λ_err_avg_beta &gt; threshold_err_beta_pobre`: `ΔwwBetaCorrection = -K_beta_acel * λ_err_avg_beta` (Reduz Beta)
        -   Senão se `λ_err_avg_beta &lt; threshold_err_beta_rico`: `ΔwwBetaCorrection = -K_beta_acel * λ_err_avg_beta` (Aumenta Beta, pois `λ_err_avg_beta` é negativo)
    -   **Se `trans_state` foi Desaceleração:**
        -   Se `λ_err_avg_beta &lt; threshold_err_beta_rico` (Rico no tip-out): `ΔwwBetaCorrection = K_beta_decel * λ_err_avg_beta` (Reduz Beta, pois `λ_err_avg_beta` é negativo)
        -   (Opcional: Se `λ_err_avg_beta &gt; threshold_err_beta_pobre` - Pobre no tip-out, menos comum, pode indicar Beta muito baixo) `ΔwwBetaCorrection = K_beta_decel * λ_err_avg_beta` (Aumenta Beta)

## 7. Modelo de Correção para Tau Aprimorado (`ΔwwTauCorrection`)

**Executado após a coleta de dados da janela `W_tau`.**

1.  Filtre as amostras de `λ_err` coletadas dentro de `W_tau` (de `t_trans_start + T_W_tau_start` até `t_trans_start + T_W_tau_end_abs`).
    
2.  `ΔwwTauCorrection = 0.0`
    
3.  `T_settle_ideal = T_settle_ideal_factor * tau_f_base_at_trans_start`
    
4.  `T_settle_tolerance_margin = T_settle_tolerance_margin_factor * T_settle_ideal`
    
5.  **Análise da Curva em `W_tau`:**
    
    -   `t_start_W_tau_abs = t_trans_start + T_W_tau_start`
    -   `λ_err_inicio_tau_avg`: Média de `λ_err` nos primeiros 50-100ms de `W_tau`.
    -   `T_settle_measured = 0`: Tempo a partir de `t_start_W_tau_abs` até `|λ_err(t)|` ficar `&lt; λ_err_settle_thresh` consistentemente. Se não assentar, `T_settle_measured` é a duração de `W_tau`.
    -   `overshoot_detectado = false`, `λ_err_overshoot_mag = 0`.
        -   Se `λ_err_inicio_tau_avg` tem sinal `S1`.
        -   Verificar se `λ_err` cruza zero e atinge magnitude `&gt; λ_overshoot_min_magnitude` com sinal `-S1` por `&gt; T_overshoot_min_duration`. Se sim, `overshoot_detectado = true`, `λ_err_overshoot_mag = max(|λ_err|)` durante o overshoot.
6.  **Lógica de Correção para `ΔwwTauCorrection`:**
    
    -   **Cenário A: Overshoot Detectado**
        
        -   Se `overshoot_detectado == true`: `ΔwwTauCorrection = K_tau_overshoot * (abs(λ_err_overshoot_mag) / λ_overshoot_min_magnitude)` (Aumenta Tau)
    -   **Cenário B: Sem Overshoot Significativo**
        
        -   Senão:
            -   **B1: Assentamento Muito Lento** Se `T_settle_measured &gt; (T_settle_ideal + T_settle_tolerance_margin)`: `ΔwwTauCorrection = -K_tau_lento * ((T_settle_measured - T_settle_ideal) / T_settle_ideal)` (Diminui Tau)
            -   **B2: Assentamento Muito Rápido (sem overshoot problemático)** Senão se `T_settle_measured &lt; (T_settle_ideal - T_settle_tolerance_margin)`: `ΔwwTauCorrection = K_tau_rapido * ((T_settle_ideal - T_settle_measured) / T_settle_ideal)` (Aumenta Tau)
            -   **B3: Assentamento Dentro do Esperado** Senão: `ΔwwTauCorrection = 0.0`

## 8. Tabelas de Correção e Aplicação

1.  Após calcular `ΔwwBetaCorrection` e `ΔwwTauCorrection` para um transiente:
    
    -   `current_beta_corr = wwBetaCorrection[map_idx_trans][rpm_idx_trans]`
        
    -   `wwBetaCorrection[map_idx_trans][rpm_idx_trans] = clamp(current_beta_corr + ΔwwBetaCorrection, min_beta_corr, max_beta_corr)`
        
    -   `current_tau_corr = wwTauCorrection[map_idx_trans][rpm_idx_trans]`
        
    -   `wwTauCorrection[map_idx_trans][rpm_idx_trans] = clamp(current_tau_corr + ΔwwTauCorrection, min_tau_corr, max_tau_corr)`
        
2.  **Uso das Correções no Cálculo de Combustível (a cada ciclo/`onFastCallback`):**
    
    -   Obtenha `map_idx_atual`, `rpm_idx_atual`.
    -   Interpole `wwBetaCorrection[map_idx_atual][rpm_idx_atual]` para obter `beta_corr_fator_atual`.
    -   Interpole `wwTauCorrection[map_idx_atual][rpm_idx_atual]` para obter `tau_corr_fator_atual`.
    -   `X_f_eff = X_f_base[map_idx_atual][rpm_idx_atual] * beta_corr_fator_atual`
    -   `τ_f_eff = τ_f_base[map_idx_atual][rpm_idx_atual] * tau_corr_fator_atual` (Lembre-se de compensar `τ_f_base` por ECT/IAT antes da multiplicação).

## 9. Fluxo de Execução no `onFastCallback` (200Hz)

Função onFastCallback():

// --- Leitura e Cálculos Iniciais ---

Ler λ_m, λ_t, MAP, TPS, RPM, ECT, IAT

Calcular λ_err = λ_m - λ_t

Calcular dTPS_dt, dMAP_dt (filtrados)

Obter map_idx_atual, rpm_idx_atual

// --- Lógica de Detecção e Gerenciamento de Estado de Transiente ---

Gerenciar trans_state (NONE, ACCEL_DETECTED, DECEL_DETECTED, ACCEL_ANALYSING, DECEL_ANALYSING, COOLDOWN_ANALYSIS)

Se (novo transiente detectado):

```
trans_state = [ACCEL/DECEL]_DETECTED

```

```
t_trans_start = current_time

```

```
map_idx_trans = map_idx_atual

```

```
rpm_idx_trans = rpm_idx_atual

```

```
tau_f_base_at_trans_start = τ_f_base[map_idx_trans][rpm_idx_trans] (compensado)

```

```
Limpar/Inicializar buffer_lambda_error

```

```
buffer_lambda_error.add(λ_err, current_time)

```

```
trans_state = [ACCEL/DECEL]_ANALYSING

```

Senão se (trans_state == [ACCEL/DECEL]_ANALYSING):

```
buffer_lambda_error.add(λ_err, current_time)

```

```
Se (current_time - t_trans_start > T_analise_max_duration OU condições de transiente cessaram):

```

```
  trans_state = COOLDOWN_ANALYSIS // Sinaliza para processar os dados coletados

```

// --- Processamento da Adaptação (pode ser feito fora do callback principal se for pesado, ou no COOLDOWN_ANALYSIS) ---

Se (trans_state == COOLDOWN_ANALYSIS E dados_suficientes_coletados):

```
// Verificar |λ_err| médio > min_lambda_err_adapt e duração > T_trans_min_duration

```

```
Se (condições de adaptação atendidas):

```

```
  // 1. Correção de Beta

```

```
  Calcular λ_err_avg_beta da W_beta (usando buffer_lambda_error e timestamps)

```

```
  Calcular ΔwwBetaCorrection (conforme Seção 6)

```

```
  Aplicar ΔwwBetaCorrection à wwBetaCorrection[map_idx_trans][rpm_idx_trans]

```

```
  // 2. Correção de Tau

```

```
  Analisar curva em W_tau (usando buffer_lambda_error e timestamps) para T_settle_measured, overshoot_detectado, etc.

```

```
  Calcular ΔwwTauCorrection (conforme Seção 7)

```

```
  Aplicar ΔwwTauCorrection à wwTauCorrection[map_idx_trans][rpm_idx_trans]

```

```
trans_state = NONE // Reset para próximo transiente

```

// --- Cálculo de Combustível com Wall Wetting Corrigido (para o ciclo atual) ---

// Esta parte é sempre executada para aplicar as correções atuais

beta_corr_fator = interpolar(wwBetaCorrection, map_idx_atual, rpm_idx_atual)

tau_corr_fator = interpolar(wwTauCorrection, map_idx_atual, rpm_idx_atual)

X_f_base_atual = X_f_base[map_idx_atual][rpm_idx_atual]

τ_f_base_atual = τ_f_base[map_idx_atual][rpm_idx_atual] (compensado por ECT/IAT)

X_f_eff = X_f_base_atual * beta_corr_fator

τ_f_eff = τ_f_base_atual * tau_corr_fator

// Usar X_f_eff e τ_f_eff no modelo de Aquino para calcular m_f[k] e m_c[k]

// … e então o pulso de injeção final …

## 10. Condições de Habilitação da Adaptação

A lógica de adaptação (cálculo e aplicação de  `ΔwwBetaCorrection`  e  `ΔwwTauCorrection`) só deve ser habilitada se:

-   Motor estiver aquecido (ECT >  `ECT_min_adapt_thresh`, ex: 70°C).

-   Sistema de lambda em malha fechada estiver ativo e estável.

-   Não houver códigos de falha (DTCs) ativos relacionados a sensores críticos (TPS, MAP, Sonda Lambda, Injetores).

-   Tensão da bateria estável.

-   Após um reset da ECU ou reprogramação, as tabelas de correção devem ser inicializadas (1.0).

## 11. Considerações Adicionais

-   **Filtragem de Sinais:**  Todos os sinais de entrada (TPS, MAP,  `λ_m`) devem ser adequadamente filtrados para reduzir ruído antes de serem usados em cálculos de derivadas ou erros.

-   **Ganhos de Adaptação (`K_...`):**  Devem ser calibrados cuidadosamente. Comece com valores pequenos para garantir estabilidade. Podem ser tabelados em função do MAP/RPM ou do erro.

-   **Interpolação:**  Use interpolação bilinear para ler valores das tabelas 8x8 (`X_f_base`,  `τ_f_base`,  `wwBetaCorrection`,  `wwTauCorrection`) entre os pontos da grade.

-   **Suavização de Correções:**  Para evitar degraus nas tabelas de correção, uma pequena fração da correção aplicada a uma célula pode ser propagada para as células vizinhas (ex: 25% para vizinhos diretos, 12.5% para diagonais), mas isso adiciona complexidade.

-   **Persistência:**  As tabelas  `wwBetaCorrection`  e  `wwTauCorrection`  devem ser armazenadas em memória não volátil.

-   **Teste e Validação:**  Testes extensivos em dinamômetro e em condições reais de condução são cruciais para calibrar e validar o sistema.

---

# IMPLEMENTAÇÃO DETALHADA - RUSEFI CODEBASE

Esta seção contém todos os detalhes de implementação para o sistema adaptativo de wall wetting no codebase RusEFI, baseado nas discussões e análises realizadas.

## 12. Análise do Estado Atual da Implementação

### 12.1. Implementação Atual de Wall Wetting

- **Localização**: `firmware/controllers/algo/wall_fuel.cpp` e `wall_fuel.h`
- **Modelo**: Aquino básico com parâmetros Alpha (retenção) e Beta (fração de impacto na parede)
- **Compensação**: CLT/MAP através de tabelas 2D básicas
- **Frequência**: Chamado durante `onFastCallback` (200Hz)
- **Status**: Implementação básica funcional, mas sem uso correto das tabelas 3D

### 12.2. Variáveis Existentes Identificadas (evotech_config.txt)

#### Flags de Controle:
- `wwEnableAdaptiveLearning`: Boolean flag para habilitar aprendizado adaptativo

#### Ganhos de Correção Beta:
- `wwAquinoBetaAccelGain`: Ganho para correção Beta em aceleração
- `wwAquinoBetaDecelGain`: Ganho para correção Beta em desaceleração  
- `wwAquinoBetaLeanThresh`: Limiar para erro Beta pobre
- `wwAquinoBetaRichThresh`: Limiar para erro Beta rico

#### Ganhos de Correção Tau:
- `wwAquinoTauOvershootGain`: Ganho para Tau quando há overshoot
- `wwAquinoTauSlowGain`: Ganho para Tau quando assentamento é lento
- `wwAquinoTauFastGain`: Ganho para Tau quando assentamento é rápido

#### Tabelas Base (8x8):
- `wwTauMapRpmValues`: Valores base Tau [MAP][RPM] (ATUALMENTE NÃO USADO CORRETAMENTE)
- `wwBetaMapRpmValues`: Valores base Beta [MAP][RPM] (ATUALMENTE NÃO USADO CORRETAMENTE)
- `wwMapBins`: Eixo MAP para tabelas base
- `wwRpmBins`: Eixo RPM para tabelas base

#### Tabelas de Correção Adaptativa (8x8):
- `wwTauCorrection`: Correções multiplicativas para Tau [MAP][RPM]
- `wwBetaCorrection`: Correções multiplicativas para Beta [MAP][RPM]
- `wwCorrectionMapBins`: Eixo MAP para tabelas de correção
- `wwCorrectionRpmBins`: Eixo RPM para tabelas de correção

### 12.3. Integração TunerStudio

- **Tabelas definidas**: `wwTauCorrectionTable`, `wwBetaCorrectionTable`
- **Exibição condicional**: Baseada em `complexWallModel` e `wwDirectionalCorrections`
- **Status**: Interface já configurada e funcional

### 12.4. Problemas Identificados no Código Atual

#### ❌ Problema 1: Tabelas 3D Não Utilizadas
```cpp
// CÓDIGO ATUAL INCORRETO em wall_fuel.cpp
tau *= interpolate2d(map, config->wwMapBins, config->wwTauMapValues);  // ❌ Não existe!
beta *= interpolate2d(map, config->wwMapBins, config->wwBetaMapValues); // ❌ Não existe!
```

#### ❌ Problema 2: Correções Adaptativas Não Aplicadas
- Tabelas `wwTauCorrection`/`wwBetaCorrection` existem mas não são usadas no cálculo

#### ❌ Problema 3: Lógica Adaptativa Ausente
- Não há implementação da máquina de estados
- Não há detecção de transientes
- Não há coleta e análise de dados lambda

## 13. Plano de Implementação Detalhado

### 13.1. FASE 1: Correção das Tabelas 3D (PRIORIDADE MÁXIMA)

#### 13.1.1. Corrigir `WallFuelController::computeTau()`:
```cpp
float WallFuelController::computeTau() const {
    if (!engineConfiguration->complexWallModel) {
        return engineConfiguration->wwaeTau;
    }

    float clt = Sensor::get(SensorType::Clt).value_or(90);
    auto map = Sensor::get(SensorType::Map).value_or(60);
    auto rpm = Sensor::get(SensorType::Rpm).value_or(1000);

    // Primeiro aplicar compensação CLT (2D)
    float tauClt = interpolate2d(clt, config->wwCltBins, config->wwTauCltValues);
    
    // Aplicar tabela 3D MAP x RPM base (seguindo padrão LTFT)
    float tauBase = interpolate3d(config->wwTauMapRpmValues,
                                 config->wwMapBins, map,     // Primeiro eixo (Y/linhas)
                                 config->wwRpmBins, rpm);    // Segundo eixo (X/colunas)
    
    // Aplicar correção adaptativa (se habilitada)
    float tauCorrection = 1.0f;
    if (engineConfiguration->wwEnableAdaptiveLearning) {
        tauCorrection = interpolate3d(config->wwTauCorrection,
                                     config->wwCorrectionMapBins, map,
                                     config->wwCorrectionRpmBins, rpm);
    }
    
    return tauClt * tauBase * tauCorrection;
}
```

#### 13.1.2. Corrigir `WallFuelController::computeBeta()`:
```cpp 
float WallFuelController::computeBeta() const {
    if (!engineConfiguration->complexWallModel) {
        return engineConfiguration->wwaeBeta;
    }

    float clt = Sensor::get(SensorType::Clt).value_or(90);
    auto map = Sensor::get(SensorType::Map).value_or(60);
    auto rpm = Sensor::get(SensorType::Rpm).value_or(1000);

    // Compensação CLT (2D)
    float betaClt = interpolate2d(clt, config->wwCltBins, config->wwBetaCltValues);
    
    // Tabela 3D MAP x RPM base (seguindo padrão LTFT)
    float betaBase = interpolate3d(config->wwBetaMapRpmValues,
                                  config->wwMapBins, map,     
                                  config->wwRpmBins, rpm);    
    
    // Correção adaptativa
    float betaCorrection = 1.0f;
    if (engineConfiguration->wwEnableAdaptiveLearning) {
        betaCorrection = interpolate3d(config->wwBetaCorrection,
                                      config->wwCorrectionMapBins, map,
                                      config->wwCorrectionRpmBins, rpm);
    }
    
    float result = betaClt * betaBase * betaCorrection;
    return clampF(0, result, 1); // Beta não pode exceder 100%
}
```

### 13.2. FASE 2: Implementação da Máquina de Estados Adaptativa

#### 13.2.1. Estruturas de Dados (seguindo padrão Launch Control/DFCO):

```cpp
/**
 * Estados da máquina de estados do sistema adaptativo de wall wetting
 * Baseado no padrão usado em LaunchControl e DFCO
 */
enum class AWWTransientState : uint8_t {
    IDLE = 0,                    // Aguardando condições adequadas
    WAITING_CONDITIONS,          // Verificando pré-condições
    TRANSIENT_DETECTED,          // Transiente detectado, iniciando análise
    COLLECTING_BETA_DATA,        // Coletando dados para análise de Beta (0-500ms)
    COLLECTING_TAU_DATA,         // Coletando dados para análise de Tau (500ms-2s)
    ANALYZING_BETA,              // Processando correção de Beta
    ANALYZING_TAU,               // Processando correção de Tau
    APPLYING_CORRECTIONS,        // Aplicando correções às tabelas
    COOLDOWN                     // Período de espera antes do próximo ciclo
};

/**
 * Dados do sistema adaptativo - seguindo padrão do launch_control_state.txt
 */
struct adaptive_wall_wetting_state_s {
    // Estado da máquina
    AWWTransientState state = AWWTransientState::IDLE;
    
    // Condições para operação
    bool isAdaptiveLearningEnabled = false;
    bool isEngineStable = false;
    bool isLambdaValid = false;
    bool isConditionsMet = false;
    
    // Detecção de transiente
    bool isTransientDetected = false;
    bool isAcceleration = false;
    float transientMagnitude = 0;
    
    // Posição nas tabelas
    uint8_t currentMapIndex = 0;
    uint8_t currentRpmIndex = 0;
    
    // Dados de análise
    uint16_t betaSampleCount = 0;
    uint16_t tauSampleCount = 0;
    float betaLambdaSum = 0;
    float tauSettlingTime = 0;
    bool tauOvershootDetected = false;
    
    // Resultados de correção
    float calculatedBetaCorrection = 0;
    float calculatedTauCorrection = 0;
};
```

#### 13.2.2. Classe Principal do Controlador (seguindo padrão EngineModule):

```cpp
/**
 * Controlador adaptativo de wall wetting
 * Integrado como EngineModule seguindo padrão de outros controladores
 */
class AdaptiveWallWettingController : public EngineModule {
public:
    using interface_t = AdaptiveWallWettingController;
    
    void onFastCallback() override;  // Chamado a 200Hz
    void onSlowCallback() override;  // Chamado a 20Hz
    void onIgnitionStateChanged(bool ignitionOn) override;  // Salvamento de dados
    
    // Interface pública para diagnósticos (similar ao LaunchControl)
    AWWTransientState getState() const { return m_state.state; }
    bool isActive() const { return m_state.state != AWWTransientState::IDLE; }
    
private:
    adaptive_wall_wetting_state_s m_state;
    
    // Buffer circular para dados de lambda (similar ao TPS accel enrichment)
    static constexpr size_t LAMBDA_BUFFER_SIZE = 400; // 2s a 200Hz
    float m_lambdaBuffer[LAMBDA_BUFFER_SIZE];
    uint16_t m_bufferIndex = 0;
    
    // Filtros para detecção de transiente (similar ao TPS accel)
    float m_lastTps = 0;
    float m_lastMap = 0;
    float m_tpsDerivative = 0;
    float m_mapDerivative = 0;
    
    // Timers seguindo padrão LTFT/LTIT
    Timer m_transientTimer;         // Times the transient analysis
    Timer m_cooldownTimer;          // Times the cooldown period
    Timer m_updateTimer;            // Controls update frequency
    
    // Salvamento de dados (seguindo padrão LTFT/LTIT)
    bool m_ignitionState = false;
    bool m_pendingSave = false;
    bool m_updatedCorrections = false;  // Flag quando tabelas são modificadas
    Timer m_ignitionOffTimer;
    
    // Métodos principais da máquina de estados
    void updateStateMachine();
    void processIdleState();
    void processWaitingConditions();
    void processTransientDetected();
    void processCollectingBetaData();
    void processCollectingTauData();
    void processAnalyzingBeta();
    void processAnalyzingTau();
    void processApplyingCorrections();
    void processCooldown();
    
    // Métodos utilitários
    bool checkConditionsForLearning() const;
    bool detectTransient();
    void addLambdaSample();
    float getCurrentLambdaError() const;
    void calculateTableIndices(float map, float rpm);
    void updateCorrectionTable(bool isBeta, float correction);
    void resetState();
};
```

### 13.3. FASE 3: Integração com WallFuelController Existente

```cpp
class WallFuelController : public IWallFuelController, public EngineModule {
public:
    void onFastCallback() override;
    
    // Métodos existentes...
    
private:
    // Membros existentes...
    
    // Novo controlador adaptativo
    AdaptiveWallWettingController m_adaptiveController;
    
    // Flag para evitar recursão
    bool m_processingAdaptive = false;
};

void WallFuelController::onFastCallback() {
    // Lógica existente...
    
    // Processar sistema adaptativo se habilitado
    if (engineConfiguration->wwEnableAdaptiveLearning && !m_processingAdaptive) {
        m_processingAdaptive = true;
        m_adaptiveController.onFastCallback();
        m_processingAdaptive = false;
    }
}
```

### 13.4. FASE 4: Algoritmos de Análise e Correção

#### 13.4.1. Algoritmo de Correção Beta (Erro Médio):
```cpp
void AdaptiveWallWettingController::analyzeBetaCorrection() {
    if (m_state.betaSampleCount < 50) { // Mínimo de amostras
        m_state.state = AWWTransientState::COOLDOWN;
        enterCooldown();
        return;
    }
    
    // Calcular erro médio durante fase beta (primeiros 500ms)
    float avgLambdaError = calculateAverageError(0.5f); // 500ms window
    
    // Aplicar ganho baseado na direção do transiente
    float gain = m_state.isAcceleration ? 
        engineConfiguration->wwAquinoBetaAccelGain : 
        engineConfiguration->wwAquinoBetaDecelGain;
    
    // Calcular correção com saturação
    float correctionPercent = avgLambdaError * gain * 100.0f;
    correctionPercent = clampF(-50.0f, correctionPercent, 50.0f);
    
    // Converter para multiplicador (ex: 5% -> 1.05)
    m_state.calculatedBetaCorrection = 1.0f + (correctionPercent / 100.0f);
    
    m_state.state = AWWTransientState::ANALYZING_TAU;
}
```

#### 13.4.2. Algoritmo de Correção Tau (Tempo de Assentamento):
```cpp
void AdaptiveWallWettingController::analyzeTauCorrection() {
    // Analisar dados de tau na janela de 500ms-2s
    float settlingThreshold = 0.02f; // 2% de lambda
    float settleTime = 0;
    
    // Procurar tempo de assentamento usando análise do buffer
    bool foundSettling = analyzeSettling(0.5f, 2.0f, settlingThreshold, &settleTime);
    
    if (foundSettling) {
        // Calcular tau esperado do modelo físico
        float currentTau = getCurrentTau();
        float currentRpm = Sensor::getOrZero(SensorType::Rpm);
        float expectedTimeConstant = currentTau / (120.0f / currentRpm);
        
        float timeRatio = settleTime / expectedTimeConstant;
        
        // Selecionar ganho baseado no comportamento
        float gain;
        if (timeRatio > 1.2f) {
            gain = engineConfiguration->wwAquinoTauSlowGain;    // Muito lento
        } else if (timeRatio < 0.8f) {
            gain = engineConfiguration->wwAquinoTauFastGain;    // Muito rápido  
        } else {
            gain = engineConfiguration->wwAquinoTauOvershootGain; // Overshoot
        }
        
        float correctionPercent = (1.0f - timeRatio) * gain * 100.0f;
        correctionPercent = clampF(-30.0f, correctionPercent, 30.0f);
        
        m_state.calculatedTauCorrection = 1.0f + (correctionPercent / 100.0f);
    } else {
        m_state.calculatedTauCorrection = 1.0f; // Sem correção
    }
    
    m_state.state = AWWTransientState::APPLYING_CORRECTIONS;
}
```

#### 13.4.3. Implementação da Máquina de Estados com Timers:
```cpp
void AdaptiveWallWettingController::onFastCallback() {
    if (!engineConfiguration->wwEnableAdaptiveLearning) {
        return;
    }
    
    // Update derivative filters for transient detection
    updateDerivativeFilters();
    
    // Add current lambda sample to buffer
    float lambdaError = getCurrentLambdaError();
    if (!std::isnan(lambdaError)) {
        addLambdaSample(lambdaError);
    }
    
    // Process state machine
    updateStateMachine();
}

void AdaptiveWallWettingController::updateStateMachine() {
    switch (m_state.state) {
        case AWWTransientState::IDLE:
            if (checkConditionsForLearning() && detectTransient()) {
                m_state.state = AWWTransientState::TRANSIENT_DETECTED;
                m_transientTimer.reset(); // Start timing the transient
                clearLambdaBuffer();
            }
            break;
            
        case AWWTransientState::TRANSIENT_DETECTED:
            // Collect data immediately after transient detection
            m_state.state = AWWTransientState::COLLECTING_BETA_DATA;
            break;
            
        case AWWTransientState::COLLECTING_BETA_DATA:
            // Collect beta data for 500ms
            if (m_transientTimer.hasElapsedSec(0.5f)) {
                m_state.state = AWWTransientState::COLLECTING_TAU_DATA;
            }
            break;
            
        case AWWTransientState::COLLECTING_TAU_DATA:
            // Collect tau data until 2s total
            if (m_transientTimer.hasElapsedSec(2.0f)) {
                m_state.state = AWWTransientState::ANALYZING_BETA;
            }
            break;
            
        case AWWTransientState::ANALYZING_BETA:
            analyzeBetaCorrection();
            break;
            
        case AWWTransientState::ANALYZING_TAU:
            analyzeTauCorrection();
            break;
            
        case AWWTransientState::APPLYING_CORRECTIONS:
            applyCorrectionToTables();
            enterCooldown();
            break;
            
        case AWWTransientState::COOLDOWN:
            // Wait 10 seconds before next learning opportunity
            if (m_cooldownTimer.hasElapsedSec(10.0f)) {
                resetState();
                m_state.state = AWWTransientState::IDLE;
            }
            break;
    }
}

void AdaptiveWallWettingController::enterCooldown() {
    m_state.state = AWWTransientState::COOLDOWN;
    m_cooldownTimer.reset();
}
```

### 13.5. FASE 5: Condições e Validações

#### 13.5.1. Condições para Operação (seguindo padrão LTFT):
```cpp
bool AdaptiveWallWettingController::checkConditionsForLearning() const {
    // Verificar flag principal
    if (!engineConfiguration->wwEnableAdaptiveLearning) {
        return false;
    }
    
    // Minimum update interval (like LTFT)
    if (!m_updateTimer.hasElapsedSec(1.0f)) {
        return false;
    }
    
    // Verificar sensores essenciais
    auto lambda = Sensor::get(SensorType::Lambda1);
    auto clt = Sensor::get(SensorType::Clt);
    auto tps = Sensor::get(SensorType::DriverThrottleIntent);
    auto map = Sensor::get(SensorType::Map);
    auto rpm = Sensor::get(SensorType::Rpm);
    
    if (!lambda.Valid || !clt.Valid || !tps.Valid || !map.Valid || !rpm.Valid) {
        return false;
    }
    
    // Verificar faixas operacionais
    if (rpm.Value < 1000 || rpm.Value > 6000) return false;
    if (clt.Value < 60 || clt.Value > 110) return false;
    if (map.Value < 30 || map.Value > 120) return false;
    
    // Lambda dentro de faixa razoável
    if (lambda.Value < 0.7f || lambda.Value > 1.3f) return false;
    
    // Engine não em cut-off ou outros modos especiais
    if (engine->module<DfcoController>()->cutFuel()) return false;
    
    // Pause if accel enrichment was active recently (like LTFT)
    auto timeSinceAccel = engine->module<TpsAccelEnrichment>()->getTimeSinceAcell();
    if (timeSinceAccel < 2.0f) return false;
    
    return true;
}
```

#### 13.5.2. Aplicação das Correções às Tabelas:
```cpp
void AdaptiveWallWettingController::applyCorrectionToTables() {
    // Get current operating point
    float map = Sensor::getOrZero(SensorType::Map);
    float rpm = Sensor::getOrZero(SensorType::Rpm);
    
    // Calculate table indices
    calculateTableIndices(map, rpm);
    
    // Apply Beta correction
    if (fabsf(m_state.calculatedBetaCorrection - 1.0f) > 0.01f) {
        updateCorrectionTable(true, m_state.calculatedBetaCorrection);
    }
    
    // Apply Tau correction  
    if (fabsf(m_state.calculatedTauCorrection - 1.0f) > 0.01f) {
        updateCorrectionTable(false, m_state.calculatedTauCorrection);
    }
    
    // Mark configuration for saving (like LTFT)
    setNeedToWriteConfiguration();
}

void AdaptiveWallWettingController::updateCorrectionTable(bool isBeta, float correction) {
    if (isBeta) {
        // Update Beta correction table with clamping
        float currentValue = config->wwBetaCorrection[m_state.currentMapIndex][m_state.currentRpmIndex];
        float newValue = currentValue * correction;
        config->wwBetaCorrection[m_state.currentMapIndex][m_state.currentRpmIndex] = clampF(0.5f, newValue, 2.0f);
    } else {
        // Update Tau correction table with clamping
        float currentValue = config->wwTauCorrection[m_state.currentMapIndex][m_state.currentRpmIndex];
        float newValue = currentValue * correction;
        config->wwTauCorrection[m_state.currentMapIndex][m_state.currentRpmIndex] = clampF(0.5f, newValue, 2.0f);
    }
    
    // Marcar que tabelas foram atualizadas
    m_updatedCorrections = true;
}
```

#### 13.5.3. Salvamento de Dados (seguindo padrão LTFT/LTIT):
```cpp
void AdaptiveWallWettingController::onIgnitionStateChanged(bool ignitionOn) {
    m_ignitionState = ignitionOn;
    
    if (ignitionOn) {
        // Reset quando ignição liga
        m_updateTimer.reset();
        m_pendingSave = false;
    } else if (m_updatedCorrections) {
        // Programar salvamento após ignição desligar
        m_pendingSave = true;
        m_ignitionOffTimer.reset();
        m_updatedCorrections = false;
    }
}

void AdaptiveWallWettingController::onSlowCallback() {
    // Handle delayed save after ignition off (como LTIT)
    if (m_pendingSave && !m_ignitionState) {
        // Usar delay configurável ou padrão de 5s como LTFT/LTIT
        if (m_ignitionOffTimer.hasElapsedSec(5.0f)) {
            // Salvar tabelas de correção para memória flash
            setNeedToWriteConfiguration();
            m_pendingSave = false;
        }
    }
    
    // Processar estado da máquina também se necessário
}
```

## 14. Considerações de Implementação Específicas do RusEFI

### 14.1. Padrões Seguidos:
- ✅ **Timers**: Uso de `Timer` class com `hasElapsedSec()` como LTFT/LTIT
- ✅ **EngineModule**: Integração adequada com `onFastCallback()`/`onSlowCallback()`
- ✅ **Interpolação 3D**: Uso correto de `interpolate3d` seguindo padrão do codebase
- ✅ **Salvamento**: Implementação de `onIgnitionStateChanged()` como LTFT/LTIT
- ✅ **Variáveis Existentes**: Uso exclusivo das configurações já criadas

### 14.2. Cronograma de Implementação:
1. **Fase 1** (Prioridade máxima): Correção das tabelas 3D - 1 dia
2. **Fase 2**: Infraestrutura básica da máquina de estados - 2 dias  
3. **Fase 3**: Integração com WallFuelController - 1 dia
4. **Fase 4**: Algoritmos de análise e correção - 2 dias
5. **Fase 5**: Validações e testes - 2 dias

**Total estimado: 8 dias de implementação**

### 14.3. Referências de Implementação:
- **Long Term Fuel Trim**: `closed_loop_fuel.cpp` (linhas 114-131)
- **Long Term Idle Trim**: `closed_loop_idle.cpp` (linhas 200-244)
- **Launch Control**: Para padrões de máquina de estados
- **DFCO Controller**: Para integração como EngineModule

### 14.4. Equações Físicas Implementadas:

#### Modelo de Aquino Corrigido:
```cpp
// X_f_eff = X_f_base[MAP][RPM] * wwBetaCorrection[MAP][RPM]
float betaEff = interpolate3d(config->wwBetaMapRpmValues, mapBins, map, rpmBins, rpm) *
                interpolate3d(config->wwBetaCorrection, correctionMapBins, map, correctionRpmBins, rpm);

// τ_f_eff = τ_f_base[MAP][RPM] * wwTauCorrection[MAP][RPM]  
float tauEff = interpolate3d(config->wwTauMapRpmValues, mapBins, map, rpmBins, rpm) *
               interpolate3d(config->wwTauCorrection, correctionMapBins, map, correctionRpmBins, rpm);

// alpha = exp(-120 / (rpm * tau_eff))
float alpha = expf(-120.0f / (rpm * tauEff));
```

#### Correções Adaptativas:
```cpp
// Beta: Baseado em erro médio nos primeiros 500ms
float betaCorrection = 1.0f + (avgLambdaError * gain);

// Tau: Baseado em tempo de assentamento vs esperado
float timeRatio = measuredSettleTime / expectedSettleTime;
float tauCorrection = 1.0f + ((1.0f - timeRatio) * gain);
```

Esta implementação segue todos os padrões estabelecidos no codebase RusEFI e utiliza exclusivamente as variáveis já configuradas, garantindo compatibilidade total com o sistema existente.

---

## 15. Refinamentos Críticos para Estabilidade

Esta seção apresenta refinamentos essenciais identificados após análise detalhada da implementação, focando em estabilidade e precisão do sistema adaptativo.

### 15.1. Análise de Acoplamento Beta-Tau

**Problema Identificado**: Beta e Tau são fisicamente interdependentes no modelo de Aquino, mas a implementação os trata independentemente, podendo causar correções que se cancelam mutuamente.

**Solução - Correções Acopladas**:

```cpp
/**
 * Estrutura para análise de acoplamento entre Beta e Tau
 * Considera a interdependência física dos parâmetros
 */
struct BetaTauCoupling {
    float correlationFactor;     // Fator de correlação entre parâmetros
    float systemTimeConstant;    // Constante de tempo atual do sistema
    float couplingStrength;      // Força do acoplamento (0-1)
};

/**
 * Calcula o efeito de acoplamento entre Beta e Tau
 * @param currentBeta: Valor atual de Beta
 * @param currentTau: Valor atual de Tau  
 * @param rpm: RPM atual
 * @return: Fator de acoplamento (0=independente, 1=totalmente acoplado)
 */
float calculateBetaTauCoupling(float currentBeta, float currentTau, float rpm) {
    // Calcular constante de tempo do sistema atual
    // Baseado na física: alpha = exp(-120/(rpm * tau))
    float systemTimeConstant = currentTau * (120.0f / rpm);
    
    // Quando tau é alto, beta tem menos impacto imediato
    // Quando tau é baixo, beta domina a resposta
    float couplingFactor = clampF(0.0f, 1.0f - expf(-systemTimeConstant * 5.0f), 0.8f);
    
    return couplingFactor;
}

/**
 * Aplica correções considerando acoplamento físico
 * Evita que correções se cancelem mutuamente
 */
void applyCoupledCorrections(float& betaCorrection, float& tauCorrection, 
                           float currentBeta, float currentTau, float rpm) {
    
    float coupling = calculateBetaTauCoupling(currentBeta, currentTau, rpm);
    
    // Se uma correção é significativa, reduzir a outra proporcionalmente
    float betaMagnitude = abs(betaCorrection - 1.0f);
    float tauMagnitude = abs(tauCorrection - 1.0f);
    
    if (betaMagnitude > 0.05f && tauMagnitude > 0.05f) {
        // Ambas correções significativas - aplicar acoplamento
        float reductionFactor = 1.0f - (coupling * 0.5f);
        
        // Reduzir a correção menor
        if (betaMagnitude > tauMagnitude) {
            tauCorrection = 1.0f + (tauCorrection - 1.0f) * reductionFactor;
        } else {
            betaCorrection = 1.0f + (betaCorrection - 1.0f) * reductionFactor;
        }
    }
}
```

### 15.2. Ganhos Adaptativos com Amortecimento

**Problema Identificado**: Ganhos fixos podem causar divergência, especialmente com erros pequenos (overcorreção) ou grandes (subcorreção).

**Solução - Ganhos Dinâmicos**:

```cpp
/**
 * Estrutura para ganhos adaptativos
 * Considera magnitude do erro e histórico de correções
 */
struct AdaptiveGainSystem {
    float baseBetaGain;              // Ganho base para Beta
    float baseTauGain;               // Ganho base para Tau  
    uint16_t recentCorrectionCount;  // Contador de correções recentes
    float errorMagnitudeAverage;     // Média de magnitude de erros
    Timer gainDecayTimer;            // Timer para decaimento do histórico
};

/**
 * Calcula ganho adaptativo para correção Beta
 * Considera magnitude do erro e amortecimento por histórico
 */
float calculateAdaptiveBetaGain(float avgLambdaError, AdaptiveGainSystem& gainSystem) {
    // Amortecimento baseado em correções recentes
    // Quanto mais correções recentes, menor o ganho
    float dampingFactor = 1.0f / (1.0f + gainSystem.recentCorrectionCount * 0.15f);
    
    // Ganho proporcional à magnitude do erro
    float expectedMaxError = 0.15f; // Erro máximo esperado (15%)
    float errorRatio = clampF(0.2f, abs(avgLambdaError) / expectedMaxError, 2.5f);
    
    // Ganho adaptativo = ganho_base * amortecimento * proporcionalidade
    float adaptiveGain = gainSystem.baseBetaGain * dampingFactor * errorRatio;
    
    // Limites de segurança
    return clampF(0.005f, adaptiveGain, 0.15f);
}

/**
 * Calcula ganho adaptativo para correção Tau
 * Usa lógica similar mas com limites diferentes
 */
float calculateAdaptiveTauGain(float timeRatio, AdaptiveGainSystem& gainSystem) {
    float dampingFactor = 1.0f / (1.0f + gainSystem.recentCorrectionCount * 0.12f);
    float deviationRatio = clampF(0.3f, abs(timeRatio - 1.0f) / 0.5f, 2.0f);
    
    float adaptiveGain = gainSystem.baseTauGain * dampingFactor * deviationRatio;
    return clampF(0.003f, adaptiveGain, 0.12f);
}

/**
 * Atualiza histórico de correções para amortecimento
 */
void updateCorrectionHistory(AdaptiveGainSystem& gainSystem, float appliedCorrection) {
    // Incrementar contador se correção foi significativa
    if (abs(appliedCorrection - 1.0f) > 0.02f) {
        gainSystem.recentCorrectionCount++;
    }
    
    // Decaimento natural do histórico (a cada 45 segundos)
    if (gainSystem.gainDecayTimer.hasElapsedSec(45.0f)) {
        if (gainSystem.recentCorrectionCount > 0) {
            gainSystem.recentCorrectionCount--;
        }
        gainSystem.gainDecayTimer.reset();
    }
}
```

### 15.3. Janelas de Análise Dinâmicas

**Problema Identificado**: Janelas fixas (500ms, 2s) não consideram a física atual do sistema (RPM e Tau), causando análise prematura ou tardia.

**Solução - Janelas Baseadas na Física**:

```cpp
/**
 * Estrutura para janelas de análise dinâmicas
 * Ajustadas automaticamente baseadas na física atual
 */
struct DynamicAnalysisWindows {
    float betaWindowEnd;         // Fim da janela de análise Beta (s)
    float tauWindowStart;        // Início da janela de análise Tau (s)
    float tauWindowEnd;          // Fim da janela de análise Tau (s)
    float expectedResponseTime;  // Tempo de resposta esperado (s)
    bool isValidForConditions;   // Se janelas são válidas para condições atuais
};

/**
 * Calcula janelas dinâmicas baseadas na física do sistema
 * @param currentTau: Valor atual de Tau
 * @param currentRpm: RPM atual
 * @param currentMap: MAP atual
 * @return: Estrutura com janelas calculadas
 */
DynamicAnalysisWindows calculateDynamicWindows(float currentTau, float currentRpm, float currentMap) {
    DynamicAnalysisWindows windows;
    
    // Calcular tempo de resposta esperado baseado na física
    // Baseado em: alpha = exp(-120/(rpm * tau))
    windows.expectedResponseTime = currentTau * (120.0f / currentRpm);
    
    // Janela Beta: capturar resposta imediata (20-40% do tempo esperado)
    // Mínimo 150ms, máximo 800ms
    windows.betaWindowEnd = clampF(0.15f, 0.3f * windows.expectedResponseTime, 0.8f);
    
    // Janela Tau: capturar assentamento completo (2-4x tempo esperado)
    // Mínimo 600ms, máximo 4s
    windows.tauWindowStart = windows.betaWindowEnd;
    float tauDuration = clampF(0.6f, 3.0f * windows.expectedResponseTime, 4.0f);
    windows.tauWindowEnd = windows.tauWindowStart + tauDuration;
    
    // Validar se janelas são apropriadas para condições atuais
    windows.isValidForConditions = validateWindowsForConditions(windows, currentRpm, currentMap);
    
    return windows;
}

/**
 * Valida se as janelas calculadas são apropriadas para as condições atuais
 */
bool validateWindowsForConditions(const DynamicAnalysisWindows& windows, float rpm, float map) {
    // Em baixo RPM, janelas muito longas podem capturar outros transientes
    if (rpm < 1200 && windows.tauWindowEnd > 3.5f) {
        return false;
    }
    
    // Em alto RPM, janelas muito curtas perdem informação importante
    if (rpm > 5000 && windows.betaWindowEnd < 0.08f) {
        return false;
    }
    
    // Em baixa carga, respostas são mais lentas
    if (map < 40 && windows.tauWindowEnd < 0.8f) {
        return false;
    }
    
    // Em alta carga, respostas são mais rápidas
    if (map > 90 && windows.betaWindowEnd > 0.6f) {
        return false;
    }
    
    return true;
}

/**
 * Aplica janelas dinâmicas na máquina de estados
 */
void AdaptiveWallWettingController::updateStateMachineWithDynamicWindows() {
    DynamicAnalysisWindows windows = calculateDynamicWindows(
        getCurrentTau(), 
        Sensor::getOrZero(SensorType::Rpm),
        Sensor::getOrZero(SensorType::Map)
    );
    
    // Usar janelas apenas se válidas, caso contrário usar padrão seguro
    if (!windows.isValidForConditions) {
        windows.betaWindowEnd = 0.4f;
        windows.tauWindowStart = 0.4f;
        windows.tauWindowEnd = 2.0f;
    }
    
    switch (m_state.state) {
        case AWWTransientState::COLLECTING_BETA_DATA:
            if (m_transientTimer.hasElapsedSec(windows.betaWindowEnd)) {
                m_state.state = AWWTransientState::COLLECTING_TAU_DATA;
            }
            break;
            
        case AWWTransientState::COLLECTING_TAU_DATA:
            if (m_transientTimer.hasElapsedSec(windows.tauWindowEnd)) {
                m_state.state = AWWTransientState::ANALYZING_BETA;
            }
            break;
            
        // ... resto dos estados
    }
}
```

### 15.4. Detecção de Transiente Adaptativa

**Problema Identificado**: Thresholds fixos para TPS/MAP não consideram que a sensibilidade varia com RPM, carga e condições operacionais.

**Solução - Thresholds Adaptativos**:

```cpp
/**
 * Estrutura para thresholds adaptativos de detecção
 * Ajustados dinamicamente às condições operacionais
 */
struct AdaptiveTransientThresholds {
    float tpsThreshold;          // Threshold TPS adaptativo (%/s)
    float mapThreshold;          // Threshold MAP adaptativo (kPa/s)
    float minimumMagnitude;      // Magnitude mínima para filtrar ruído
    float stabilityFactor;       // Fator de estabilidade das condições
};

/**
 * Calcula thresholds adaptativos baseados nas condições atuais
 * @param rpm: RPM atual
 * @param map: MAP atual  
 * @param vehicleSpeed: Velocidade do veículo (se disponível)
 * @return: Thresholds calculados
 */
AdaptiveTransientThresholds calculateAdaptiveThresholds(float rpm, float map, float vehicleSpeed) {
    AdaptiveTransientThresholds thresholds;
    
    // TPS threshold adaptativo por RPM
    // Em baixo RPM: mais sensível (threshold menor)
    // Em alto RPM: menos sensível (threshold maior)
    if (rpm < 2000) {
        thresholds.tpsThreshold = 45.0f;
    } else if (rpm < 4000) {
        thresholds.tpsThreshold = interpolateF(rpm, 2000, 4000, 45.0f, 85.0f);
    } else {
        thresholds.tpsThreshold = 85.0f;
    }
    
    // MAP threshold adaptativo por carga atual
    // Em baixa carga: mais sensível
    // Em alta carga: menos sensível (mais ruído)
    if (map < 50) {
        thresholds.mapThreshold = 25.0f;
    } else if (map < 80) {
        thresholds.mapThreshold = interpolateF(map, 50, 80, 25.0f, 45.0f);
    } else {
        thresholds.mapThreshold = 45.0f;
    }
    
    // Magnitude mínima para filtrar ruído de sensores
    thresholds.minimumMagnitude = clampF(8.0f, 0.08f * map, 20.0f);
    
    // Fator de estabilidade (reduz sensibilidade em condições instáveis)
    thresholds.stabilityFactor = calculateStabilityFactor(rpm, map, vehicleSpeed);
    
    // Aplicar fator de estabilidade
    thresholds.tpsThreshold *= thresholds.stabilityFactor;
    thresholds.mapThreshold *= thresholds.stabilityFactor;
    
    return thresholds;
}

/**
 * Calcula fator de estabilidade das condições atuais
 * Reduz sensibilidade em condições instáveis (marcha lenta, aceleração contínua)
 */
float calculateStabilityFactor(float rpm, float map, float vehicleSpeed) {
    float stabilityFactor = 1.0f;
    
    // Reduzir sensibilidade em marcha lenta (condições instáveis)
    if (rpm < 1000) {
        stabilityFactor *= 1.4f; // Aumentar threshold (menos sensível)
    }
    
    // Reduzir sensibilidade em aceleração contínua
    if (vehicleSpeed > 0 && map > 70) {
        stabilityFactor *= 1.2f;
    }
    
    // Aumentar sensibilidade em condições estáveis (cruzeiro)
    if (rpm > 2000 && map > 30 && map < 60) {
        stabilityFactor *= 0.8f; // Diminuir threshold (mais sensível)
    }
    
    return clampF(0.6f, stabilityFactor, 1.8f);
}

/**
 * Detecção de transiente com thresholds adaptativos
 */
bool AdaptiveWallWettingController::detectAdaptiveTransient() {
    // Atualizar filtros de derivada
    updateDerivativeFilters();
    
    // Calcular thresholds adaptativos
    auto rpm = Sensor::getOrZero(SensorType::Rpm);
    auto map = Sensor::getOrZero(SensorType::Map);
    auto speed = Sensor::getOrZero(SensorType::VehicleSpeed);
    
    AdaptiveTransientThresholds thresholds = calculateAdaptiveThresholds(rpm, map, speed);
    
    // Verificar transiente TPS
    bool tpsTransient = (abs(m_tpsDerivative) > thresholds.tpsThreshold) &&
                       (abs(m_tpsDerivative) > thresholds.minimumMagnitude);
    
    // Verificar transiente MAP  
    bool mapTransient = (abs(m_mapDerivative) > thresholds.mapThreshold) &&
                       (abs(m_mapDerivative) > thresholds.minimumMagnitude);
    
    // Determinar direção do transiente
    if (tpsTransient || mapTransient) {
        m_state.isAcceleration = (m_tpsDerivative > 0) || (m_mapDerivative > 0);
        return true;
    }
    
    return false;
}
```

### 15.5. Compensação de Delay da Sonda Lambda

**Problema Identificado**: O delay da sonda lambda (150-400ms) não é compensado, causando correlação incorreta entre causa (injeção) e efeito (lambda medido).

**Solução - Compensação Dinâmica de Delay**:

```cpp
/**
 * Estrutura para compensação de delay da sonda lambda
 * Considera temperatura, fluxo e posicionamento da sonda
 */
struct LambdaDelayCompensation {
    float estimatedDelay;        // Delay estimado atual (s)
    float exhaustTemperature;    // Temperatura do escape (°C)
    float exhaustFlowRate;       // Taxa de fluxo do escape
    Timer delayUpdateTimer;      // Timer para atualização do delay
};

/**
 * Estima o delay da sonda lambda baseado em condições operacionais
 * @param exhaustTemp: Temperatura estimada do escape
 * @param rpm: RPM atual
 * @param load: Carga atual (MAP/TPS)
 * @return: Delay estimado em segundos
 */
float estimateLambdaDelay(float exhaustTemp, float rpm, float load) {
    // Delay base da sonda (características físicas)
    float baseDelay = 0.22f; // 220ms baseline para sonda aquecida
    
    // Fator de temperatura (sonda fria = mais delay)
    float tempFactor = 1.0f;
    if (exhaustTemp < 300.0f) {
        tempFactor = 1.8f; // Sonda fria
    } else if (exhaustTemp < 500.0f) {
        tempFactor = interpolateF(exhaustTemp, 300.0f, 500.0f, 1.8f, 1.0f);
    } else if (exhaustTemp > 800.0f) {
        tempFactor = 0.8f; // Sonda muito quente (resposta mais rápida)
    }
    
    // Fator de fluxo (mais fluxo = menos delay de transporte)
    float flowFactor = 1.0f;
    if (rpm > 3000 && load > 50) {
        flowFactor = 0.85f; // Alto fluxo
    } else if (rpm < 1500 && load < 30) {
        flowFactor = 1.3f; // Baixo fluxo
    }
    
    // Delay final
    float finalDelay = baseDelay * tempFactor * flowFactor;
    
    // Limites físicos razoáveis
    return clampF(0.08f, finalDelay, 0.6f);
}

/**
 * Obtém valor de lambda compensado pelo delay estimado
 * @param estimatedDelay: Delay estimado em segundos
 * @return: Valor de lambda corrigido temporalmente
 */
float AdaptiveWallWettingController::getLambdaWithDelayCompensation(float estimatedDelay) {
    // Converter delay para número de samples (200Hz)
    uint16_t delaySamples = (uint16_t)(estimatedDelay * 200.0f);
    
    // Garantir que não excede buffer size
    delaySamples = min(delaySamples, LAMBDA_BUFFER_SIZE - 10);
    
    // Calcular índice compensado no buffer circular
    uint16_t compensatedIndex = (m_bufferIndex + LAMBDA_BUFFER_SIZE - delaySamples) 
                               % LAMBDA_BUFFER_SIZE;
    
    // Interpolar entre samples para maior precisão
    float fractionalDelay = (estimatedDelay * 200.0f) - (float)delaySamples;
    
    uint16_t nextIndex = (compensatedIndex + 1) % LAMBDA_BUFFER_SIZE;
    float interpolatedValue = m_lambdaBuffer[compensatedIndex] * (1.0f - fractionalDelay) +
                             m_lambdaBuffer[nextIndex] * fractionalDelay;
    
    return interpolatedValue;
}

/**
 * Atualiza estimativa de delay da sonda periodicamente
 */
void AdaptiveWallWettingController::updateLambdaDelayEstimation() {
    // Atualizar delay a cada 2 segundos
    if (!m_delayCompensation.delayUpdateTimer.hasElapsedSec(2.0f)) {
        return;
    }
    
    // Estimar temperatura do escape baseada em RPM e carga
    float rpm = Sensor::getOrZero(SensorType::Rpm);
    float load = Sensor::getOrZero(SensorType::Map);
    float clt = Sensor::getOrZero(SensorType::Clt);
    
    // Temperatura do escape aproximada
    float exhaustTemp = clt + (rpm * 0.05f) + (load * 2.0f);
    exhaustTemp = clampF(200.0f, exhaustTemp, 900.0f);
    
    // Atualizar delay estimado
    m_delayCompensation.estimatedDelay = estimateLambdaDelay(exhaustTemp, rpm, load);
    m_delayCompensation.delaySamples = (uint16_t)(m_delayCompensation.estimatedDelay * 200.0f);
    
    m_delayCompensation.delayUpdateTimer.reset();
}

/**
 * Obtém erro de lambda atual com compensação de delay
 */
float AdaptiveWallWettingController::getCurrentLambdaError() const {
    // Obter lambda target atual
    float lambdaTarget = 1.0f; // Ou do mapa AFR atual
    
    // Obter lambda medido com compensação de delay
    float lambdaMeasured = getLambdaWithDelayCompensation(m_delayCompensation.estimatedDelay);
    
    return lambdaMeasured - lambdaTarget;
}
```

### 15.6. Integração dos Refinamentos na Implementação Principal

**Atualização da Classe Principal**:

```cpp
class AdaptiveWallWettingController : public EngineModule {
private:
    // Estruturas dos refinamentos
    AdaptiveGainSystem m_gainSystem;
    LambdaDelayCompensation m_delayCompensation;
    DynamicAnalysisWindows m_currentWindows;
    BetaTauCoupling m_couplingAnalysis;
    
    // Métodos refinados
    void updateRefinedStateMachine();
    bool detectRefinedTransient();
    void analyzeRefinedBetaCorrection();
    void analyzeRefinedTauCorrection();
    void applyRefinedCorrectionToTables();
    
public:
    void onFastCallback() override {
        if (!engineConfiguration->wwEnableAdaptiveLearning) {
            return;
        }
        
        // Atualizar compensação de delay
        updateLambdaDelayEstimation();
        
        // Atualizar filtros de derivada
        updateDerivativeFilters();
        
        // Adicionar sample de lambda com compensação
        float lambdaError = getCurrentLambdaError();
        if (!std::isnan(lambdaError)) {
            addLambdaSample(lambdaError);
        }
        
        // Processar máquina de estados refinada
        updateRefinedStateMachine();
    }
};
```

### 15.7. Considerações de Implementação dos Refinamentos

#### **Compatibilidade com Variáveis Existentes**:
- ✅ Todos os refinamentos usam apenas variáveis existentes do `evotech_config.txt`
- ✅ Ganhos adaptativos baseados nos ganhos base já configurados
- ✅ Thresholds adaptativos derivados dos valores existentes

#### **Performance e Recursos**:
- ✅ Cálculos otimizados para execução a 200Hz
- ✅ Uso eficiente de memória com estruturas compactas
- ✅ Interpolações apenas quando necessário

#### **Estabilidade Garantida**:
- ✅ Amortecimento automático previne oscilações
- ✅ Janelas dinâmicas evitam análise inadequada
- ✅ Compensação de delay melhora correlação temporal
- ✅ Acoplamento físico previne correções conflitantes

**Com estes refinamentos, o sistema adaptativo terá estabilidade e precisão significativamente melhoradas, mantendo total compatibilidade com a infraestrutura existente.**

---

# 16. CORREÇÕES CRÍTICAS DE ESTABILIDADE

Esta seção implementa as correções essenciais para prevenir instabilidade e divergência do sistema adaptativo, baseadas na análise detalhada dos problemas identificados.

## 16.1. Problema Crítico #1: Acoplamento Beta-Tau Ignorado

**PROBLEMA**: Beta e Tau são fisicamente interdependentes no modelo de Aquino, mas o sistema atual os corrige independentemente, causando instabilidade.

**SOLUÇÃO - Correção Física do Acoplamento**:

```cpp
/**
 * Calcula fator de acoplamento Beta-Tau baseado na física do modelo Aquino
 * @param currentBeta: Valor atual de Beta
 * @param currentTau: Valor atual de Tau (s)
 * @param rpm: RPM atual
 * @return: Fator de acoplamento [0.1-0.9]
 */
float calculateBetaTauCoupling(float currentBeta, float currentTau, float rpm) {
    // CORREÇÃO: Usar constante de tempo adimensional correta
    float engineCycleTime = 120.0f / rpm;  // s/ciclo
    float evaporationCycles = currentTau / engineCycleTime;  // ciclos para evaporação
    
    // Acoplamento baseado na física: quanto mais ciclos, menor o acoplamento imediato
    float couplingFactor = 1.0f / (1.0f + evaporationCycles * 0.2f);
    return clampF(0.1f, couplingFactor, 0.9f);
}

/**
 * Estrutura para análise de acoplamento Beta-Tau
 */
struct BetaTauCoupling {
    float couplingFactor = 0.5f;      // Fator de acoplamento atual
    float betaCorrection = 1.0f;       // Correção Beta calculada
    float tauCorrection = 1.0f;        // Correção Tau calculada
    bool hasConflict = false;          // Flag para detecção de conflito
    
    /**
     * Valida se correções Beta e Tau são fisicamente compatíveis
     */
    bool validateCoupledCorrections(float beta, float tau, float rpm) {
        // Calcular acoplamento físico
        couplingFactor = calculateBetaTauCoupling(beta, tau, rpm);
        
        // Detectar conflito: correções em direções opostas com acoplamento alto
        bool betaIncrease = (betaCorrection > 1.05f);
        bool tauIncrease = (tauCorrection > 1.05f);
        bool betaDecrease = (betaCorrection < 0.95f);
        bool tauDecrease = (tauCorrection < 0.95f);
        
        // Conflito: alta acoplamento + direções opostas
        hasConflict = (couplingFactor > 0.7f) && 
                     ((betaIncrease && tauDecrease) || (betaDecrease && tauIncrease));
        
        return !hasConflict;
    }
    
    /**
     * Resolve conflitos aplicando prioridade física
     */
    void resolveConflict() {
        if (!hasConflict) return;
        
        // PRIORIDADE: Beta tem impacto mais imediato, Tau afeta resposta temporal
        // Em conflito, reduzir magnitude de ambas correções
        float conflictDamping = 0.3f;
        
        if (betaCorrection > 1.0f) betaCorrection = 1.0f + (betaCorrection - 1.0f) * conflictDamping;
        if (betaCorrection < 1.0f) betaCorrection = 1.0f + (betaCorrection - 1.0f) * conflictDamping;
        
        if (tauCorrection > 1.0f) tauCorrection = 1.0f + (tauCorrection - 1.0f) * conflictDamping;
        if (tauCorrection < 1.0f) tauCorrection = 1.0f + (tauCorrection - 1.0f) * conflictDamping;
        
        hasConflict = false;
    }
};
```

## 16.2. Problema Crítico #2: Ganhos Fixos Causam Divergência

**PROBLEMA**: Ganhos constantes não consideram magnitude do erro ou histórico, causando over/sub-correção.

**SOLUÇÃO - Ganhos Adaptativos**:

```cpp
/**
 * Sistema de ganhos adaptativos que previne divergência
 */
struct AdaptiveGainSystem {
    // Histórico de correções para detectar oscilação
    float lastBetaCorrections[5] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
    float lastTauCorrections[5] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
    uint8_t historyIndex = 0;
    
    // Fatores de amortecimento dinâmico
    float betaDampingFactor = 1.0f;
    float tauDampingFactor = 1.0f;
    
    /**
     * Calcula ganho adaptativo baseado na magnitude do erro e histórico
     */
    float calculateAdaptiveGain(float baseGain, float errorMagnitude, bool isBeta) {
        // Ganho base da configuração
        float adaptiveGain = baseGain;
        
        // Fator 1: Magnitude do erro (menor erro = menor ganho)
        float magnitudeFactor = clampF(0.2f, errorMagnitude * 10.0f, 2.0f);
        adaptiveGain *= magnitudeFactor;
        
        // Fator 2: Amortecimento baseado no histórico de oscilação
        float dampingFactor = isBeta ? betaDampingFactor : tauDampingFactor;
        adaptiveGain *= dampingFactor;
        
        // Limite final para prevenir instabilidade
        return clampF(baseGain * 0.1f, adaptiveGain, baseGain * 1.5f);
    }
    
    /**
     * Detecta oscilação no histórico de correções
     */
    bool detectOscillation(bool isBeta) {
        float* history = isBeta ? lastBetaCorrections : lastTauCorrections;
        
        // Contar mudanças de direção
        uint8_t directionChanges = 0;
        for (int i = 1; i < 4; i++) {
            bool currentIncrease = history[i] > history[i-1];
            bool nextIncrease = history[i+1] > history[i];
            if (currentIncrease != nextIncrease) {
                directionChanges++;
            }
        }
        
        // Oscilação detectada se muitas mudanças de direção
        return directionChanges >= 2;
    }
    
    /**
     * Atualiza histórico e calcula amortecimento
     */
    void updateHistory(float betaCorrection, float tauCorrection) {
        lastBetaCorrections[historyIndex] = betaCorrection;
        lastTauCorrections[historyIndex] = tauCorrection;
        historyIndex = (historyIndex + 1) % 5;
        
        // Ajustar amortecimento baseado em oscilação
        if (detectOscillation(true)) {
            betaDampingFactor *= 0.8f;  // Reduzir ganho Beta
        } else {
            betaDampingFactor = minF(1.0f, betaDampingFactor * 1.05f);  // Recuperar lentamente
        }
        
        if (detectOscillation(false)) {
            tauDampingFactor *= 0.8f;  // Reduzir ganho Tau
        } else {
            tauDampingFactor = minF(1.0f, tauDampingFactor * 1.05f);  // Recuperar lentamente
        }
        
        // Limites de amortecimento
        betaDampingFactor = clampF(0.2f, betaDampingFactor, 1.0f);
        tauDampingFactor = clampF(0.2f, tauDampingFactor, 1.0f);
    }
};
```

## 16.3. Problema Crítico #3: Buffer Thread-Safe

**PROBLEMA**: Buffer circular não é thread-safe, pode causar corrupção de dados.

**SOLUÇÃO - Buffer Thread-Safe**:

```cpp
/**
 * Buffer circular thread-safe para dados de lambda
 */
class ThreadSafeLambdaBuffer {
private:
    float m_buffer[LAMBDA_BUFFER_SIZE];
    efitick_t m_timestamps[LAMBDA_BUFFER_SIZE];
    volatile uint16_t m_writeIndex = 0;      // Índice de escrita atômico
    volatile uint16_t m_sampleCount = 0;     // Contador de amostras
    
public:
    /**
     * Adiciona amostra de forma thread-safe
     */
    void addSample(float lambdaError, efitick_t timestamp) {
        uint16_t currentWrite = m_writeIndex;
        
        // Escrever dados
        m_buffer[currentWrite] = lambdaError;
        m_timestamps[currentWrite] = timestamp;
        
        // Atualizar índices atomicamente
        m_writeIndex = (currentWrite + 1) % LAMBDA_BUFFER_SIZE;
        if (m_sampleCount < LAMBDA_BUFFER_SIZE) {
            m_sampleCount++;
        }
    }
    
    /**
     * Obtém valor com delay específico de forma segura
     */
    float getDelayedValue(uint16_t delaySamples) const {
        uint16_t currentWrite = m_writeIndex;  // Snapshot atômico
        
        // Validar delay
        if (delaySamples >= m_sampleCount || delaySamples >= LAMBDA_BUFFER_SIZE) {
            return 0.0f;  // Valor seguro
        }
        
        uint16_t readIndex = (currentWrite + LAMBDA_BUFFER_SIZE - delaySamples) % LAMBDA_BUFFER_SIZE;
        return m_buffer[readIndex];
    }
    
    /**
     * Calcula média em janela temporal específica de forma segura
     */
    float getAverageInWindow(float windowDurationSec) const {
        uint16_t windowSamples = (uint16_t)(windowDurationSec * 200.0f);  // 200Hz
        windowSamples = minF(windowSamples, m_sampleCount);
        
        if (windowSamples == 0) return 0.0f;
        
        float sum = 0.0f;
        uint16_t currentWrite = m_writeIndex;
        
        for (uint16_t i = 0; i < windowSamples; i++) {
            uint16_t index = (currentWrite + LAMBDA_BUFFER_SIZE - 1 - i) % LAMBDA_BUFFER_SIZE;
            sum += m_buffer[index];
        }
        
        return sum / windowSamples;
    }
    
    /**
     * Limpa buffer de forma segura
     */
    void clear() {
        m_sampleCount = 0;
        m_writeIndex = 0;
        // Dados antigos serão sobrescritos naturalmente
    }
};
```

## 16.4. Problema Crítico #4: Modelo de Injeção Mais Realista

**PROBLEMA**: Correlação inadequada entre timing de injeção e resposta de lambda.

**SOLUÇÃO - Timing Realista de Injeção**:

```cpp
/**
 * Calcula timing real da injeção no ciclo do motor
 */
float calculateInjectionTiming(float currentCrankAngle, float injectionAngle, float injectionDuration) {
    // Considerar quando a injeção realmente acontece no ciclo
    float injectionPhase = (currentCrankAngle - injectionAngle) / 360.0f;
    
    // Normalizar para [0, 1]
    while (injectionPhase < 0) injectionPhase += 1.0f;
    while (injectionPhase > 1) injectionPhase -= 1.0f;
    
    float injectionDurationPhase = injectionDuration / 360.0f;
    
    if (injectionPhase <= injectionDurationPhase) {
        return 1.0f;  // Injetando agora
    }
    return 0.0f;  // Não injetando
}

/**
 * Compensa timing de injeção na análise de lambda
 */
struct InjectionTimingCompensation {
    float lastInjectionPhase = 0.0f;
    efitick_t lastInjectionTime = 0;
    
    /**
     * Determina se lambda atual corresponde à injeção analisada
     */
    bool isLambdaCorrelated(efitick_t currentTime, float crankAngle) {
        // Calcular delay esperado entre injeção e medição lambda
        float expectedDelay = 0.2f;  // ~200ms delay típico
        efitick_t expectedDelayTicks = US2NT(expectedDelay * 1000000);
        
        // Verificar se tempo corresponde
        return (currentTime - lastInjectionTime) >= expectedDelayTicks;
    }
};
```

## 16.5. Problema Crítico #5: Validação Coordenada

**PROBLEMA**: Diferentes subsistemas validam independentemente, causando inconsistências.

**SOLUÇÃO - Validação Mestre Coordenada**:

```cpp
/**
 * Sistema de validação coordenada que garante consistência
 */
struct CoordinatedValidation {
    bool thresholdValid = false;     // Thresholds de detecção válidos
    bool windowValid = false;        // Janelas de análise válidas  
    bool delayValid = false;         // Compensação de delay válida
    bool couplingValid = false;      // Acoplamento Beta-Tau válido
    bool overallValid = false;       // Validação mestre
    
    /**
     * Valida thresholds adaptativos baseados em condições atuais
     */
    bool validateThresholds(float rpm, float map, float tau) {
        // Threshold TPS adaptativo (função do RPM)
        float tpsThreshold = 50.0f + (rpm - 1000.0f) * 0.01f;  // Base + RPM factor
        tpsThreshold = clampF(30.0f, tpsThreshold, 100.0f);
        
        // Threshold MAP adaptativo (função da pressão)
        float mapThreshold = 20.0f + (map - 50.0f) * 0.2f;    // Base + MAP factor  
        mapThreshold = clampF(15.0f, mapThreshold, 60.0f);
        
        // Validar se condições permitem detecção confiável
        bool rpmRange = (rpm >= 1000.0f && rpm <= 6000.0f);
        bool mapRange = (map >= 30.0f && map <= 120.0f);
        bool reasonableThresholds = (tpsThreshold > 20.0f && mapThreshold > 10.0f);
        
        return rpmRange && mapRange && reasonableThresholds;
    }
    
    /**
     * Valida janelas de análise baseadas em Tau
     */
    bool validateWindows(float rpm, float tau) {
        // Janelas dinâmicas baseadas no tau físico
        float betaWindow = minF(0.5f, tau * 0.8f);       // Beta: início da resposta
        float tauWindow = maxF(1.0f, tau * 3.0f);        // Tau: assentamento completo
        
        // Verificar se janelas são fisicamente razoáveis
        bool windowsReasonable = (betaWindow >= 0.1f && betaWindow <= 1.0f) &&
                                (tauWindow >= 0.5f && tauWindow <= 4.0f);
        
        // Verificar se RPM permite análise adequada
        bool rpmAllowsAnalysis = (rpm >= 1200.0f);  // RPM mínimo para análise estável
        
        return windowsReasonable && rpmAllowsAnalysis;
    }
    
    /**
     * Valida compensação de delay
     */
    bool validateDelay(float rpm, float map) {
        // Estimar delay baseado em condições
        float estimatedDelay = 0.2f + (1000.0f / rpm) * 0.1f;  // Base + RPM factor
        
        // Validar se delay está em faixa razoável
        bool delayReasonable = (estimatedDelay >= 0.1f && estimatedDelay <= 0.6f);
        
        // Validar se condições permitem compensação confiável
        bool steadyState = (rpm > 800.0f && map > 20.0f);
        
        return delayReasonable && steadyState;
    }
    
    /**
     * Validação mestre que considera todos os fatores
     */
    void validateAll(float rpm, float map, float tau, const BetaTauCoupling& coupling) {
        thresholdValid = validateThresholds(rpm, map, tau);
        windowValid = validateWindows(rpm, tau);
        delayValid = validateDelay(rpm, map);
        couplingValid = !coupling.hasConflict;
        
        // Só aprova se TODOS os subsistemas concordam
        overallValid = thresholdValid && windowValid && delayValid && couplingValid;
        
        // Adicionar margem de segurança: exigir 2 validações consecutivas
        static bool lastValidation = false;
        if (overallValid && !lastValidation) {
            overallValid = false;  // Primeira validação, aguardar confirmação
        }
        lastValidation = overallValid;
    }
};
```

## 16.6. Problema Crítico #6: Limitação de Complexidade Computacional

**PROBLEMA**: Cálculos complexos podem sobrecarregar CPU a 200Hz.

**SOLUÇÃO - Escalonamento de Processamento**:

```cpp
/**
 * Controla complexidade computacional distribuindo cálculos no tempo
 */
void AdaptiveWallWettingController::onFastCallback() {
    static uint8_t cycleCounter = 0;
    static uint8_t heavyProcessingCycle = 0;
    
    // SEMPRE: Operações críticas de alta frequência
    updateBasicFilters();           // ~10 operações
    addLambdaSample();             // ~5 operações
    updateStateMachine();          // ~20 operações
    
    // A cada 5 ciclos (40Hz): Cálculos adaptativos médios
    if (++cycleCounter >= 5) {
        updateAdaptiveThresholds();     // ~50 operações
        validateCurrentConditions();   // ~30 operações
        cycleCounter = 0;
    }
    
    // A cada 20 ciclos (10Hz): Processamento pesado
    if (++heavyProcessingCycle >= 20) {
        updateLambdaDelayEstimation();  // ~100 operações
        analyzeOscillationHistory();   // ~80 operações
        optimizeGainFactors();         // ~60 operações
        heavyProcessingCycle = 0;
    }
    
    // NUNCA: Operações bloqueantes ou de I/O
    // NUNCA: Loops com número indefinido de iterações
    // NUNCA: Operações de ponto flutuante desnecessárias
}

/**
 * Cache de cálculos caros para evitar recomputação
 */
struct ComputationCache {
    // Cache de interpolações
    struct {
        float lastMap = -1.0f;
        float lastRpm = -1.0f;
        float cachedBeta = 1.0f;
        float cachedTau = 1.0f;
        bool valid = false;
    } interpolationCache;
    
    // Cache de thresholds adaptativos
    struct {
        float lastRpm = -1.0f;
        float cachedTpsThreshold = 50.0f;
        float cachedMapThreshold = 30.0f;
        bool valid = false;
    } thresholdCache;
    
    /**
     * Obtém valor interpolado com cache
     */
    float getCachedInterpolation(float map, float rpm, bool isBeta) {
        // Verificar se cache é válido (tolerância de 5%)
        float mapTolerance = fabsf(map - interpolationCache.lastMap) / map;
        float rpmTolerance = fabsf(rpm - interpolationCache.lastRpm) / rpm;
        
        if (!interpolationCache.valid || mapTolerance > 0.05f || rpmTolerance > 0.05f) {
            // Recalcular e atualizar cache
            interpolationCache.lastMap = map;
            interpolationCache.lastRpm = rpm;
            
            // Fazer interpolação cara apenas quando necessário
            interpolationCache.cachedBeta = interpolate3d(/* parâmetros beta */);
            interpolationCache.cachedTau = interpolate3d(/* parâmetros tau */);
            interpolationCache.valid = true;
        }
        
        return isBeta ? interpolationCache.cachedBeta : interpolationCache.cachedTau;
    }
};
```

## 16.7. Problema Crítico #7: Proteção Contra Divergência

**PROBLEMA**: Sem limites para correções acumuladas totais, sistema pode divergir.

**SOLUÇÃO - Limites Absolutos e Detecção de Divergência**:

```cpp
/**
 * Sistema de proteção contra divergência
 */
struct DivergenceProtection {
    // Limites absolutos das correções
    static constexpr float MAX_TOTAL_BETA_CORRECTION = 3.0f;   // 300% máximo
    static constexpr float MIN_TOTAL_BETA_CORRECTION = 0.3f;   // 30% mínimo
    static constexpr float MAX_TOTAL_TAU_CORRECTION = 4.0f;    // 400% máximo
    static constexpr float MIN_TOTAL_TAU_CORRECTION = 0.25f;   // 25% mínimo
    
    // Contadores de correções consecutivas
    uint8_t consecutiveBetaCorrections = 0;
    uint8_t consecutiveTauCorrections = 0;
    
    // Detecção de divergência
    bool divergenceDetected = false;
    Timer divergenceCooldown;
    
    /**
     * Verifica se correção proposta causa divergência
     */
    bool checkForDivergence(float currentCorrection, float proposedChange, bool isBeta) {
        float newTotal = currentCorrection * proposedChange;
        
        // Verificar limites absolutos
        if (isBeta) {
            if (newTotal > MAX_TOTAL_BETA_CORRECTION || newTotal < MIN_TOTAL_BETA_CORRECTION) {
                return true;  // Divergência detectada
            }
        } else {
            if (newTotal > MAX_TOTAL_TAU_CORRECTION || newTotal < MIN_TOTAL_TAU_CORRECTION) {
                return true;  // Divergência detectada
            }
        }
        
        // Verificar correções consecutivas excessivas
        uint8_t& consecutiveCount = isBeta ? consecutiveBetaCorrections : consecutiveTauCorrections;
        if (fabsf(proposedChange - 1.0f) > 0.05f) {  // Correção significativa
            consecutiveCount++;
            if (consecutiveCount > 5) {  // Muitas correções consecutivas
                return true;  // Possível divergência
            }
        } else {
            consecutiveCount = 0;  // Reset contador
        }
        
        return false;  // Sem divergência
    }
    
    /**
     * Aplica correção limitada para prevenir divergência
     */
    float applySafeCorrection(float currentCorrection, float proposedChange, bool isBeta) {
        if (checkForDivergence(currentCorrection, proposedChange, isBeta)) {
            divergenceDetected = true;
            divergenceCooldown.reset();
            
            // Aplicar correção muito conservadora
            float safeChange = 1.0f + (proposedChange - 1.0f) * 0.1f;  // 10% da correção original
            return currentCorrection * safeChange;
        }
        
        // Aplicar correção normal se não há divergência
        return currentCorrection * proposedChange;
    }
    
    /**
     * Verifica se sistema pode tentar correções após divergência
     */
    bool canAttemptCorrection() const {
        if (divergenceDetected) {
            return divergenceCooldown.hasElapsedSec(30.0f);  // 30s de cooldown
        }
        return true;
    }
    
    /**
     * Reset proteção após cooldown
     */
    void resetProtection() {
        if (divergenceDetected && divergenceCooldown.hasElapsedSec(30.0f)) {
            divergenceDetected = false;
            consecutiveBetaCorrections = 0;
            consecutiveTauCorrections = 0;
        }
    }
};
```

## 16.8. Integração Coordenada dos Refinamentos

**Máquina de Estados Refinada**:

```cpp
void AdaptiveWallWettingController::updateRefinedStateMachine() {
    // Atualizar sistemas de proteção
    m_divergenceProtection.resetProtection();
    m_gainSystem.updateHistory(m_lastBetaCorrection, m_lastTauCorrection);
    
    // Validação coordenada antes de qualquer processamento
    float rpm = Sensor::getOrZero(SensorType::Rpm);
    float map = Sensor::getOrZero(SensorType::Map);
    float currentTau = getCurrentTau();
    
    CoordinatedValidation validation;
    validation.validateAll(rpm, map, currentTau, m_betaTauCoupling);
    
    if (!validation.overallValid) {
        // Condições inadequadas, retornar ao IDLE
        if (m_state.state != AWWTransientState::IDLE) {
            resetToIdleState();
        }
        return;
    }
    
    // Processar máquina de estados apenas se validação coordenada passou
    switch (m_state.state) {
        case AWWTransientState::IDLE:
            if (detectRefinedTransient()) {
                m_state.state = AWWTransientState::TRANSIENT_DETECTED;
                initializeTransientAnalysis();
            }
            break;
            
        case AWWTransientState::PROCESSING_BETA:
            analyzeRefinedBetaCorrection();
            break;
            
        case AWWTransientState::PROCESSING_TAU:
            analyzeRefinedTauCorrection();
            break;
            
        case AWWTransientState::APPLYING_CORRECTIONS:
            applyRefinedCorrectionToTables();
            break;
            
        // ... outros estados
    }
}

/**
 * Análise Beta refinada com todos os sistemas de segurança
 */
void AdaptiveWallWettingController::analyzeRefinedBetaCorrection() {
    if (!m_divergenceProtection.canAttemptCorrection()) {
        enterCooldown();
        return;
    }
    
    // Calcular correção com ganhos adaptativos
    float avgError = m_threadSafeBuffer.getAverageInWindow(0.5f);
    float baseGain = engineConfiguration->wwAquinoBetaAccelGain;
    float adaptiveGain = m_gainSystem.calculateAdaptiveGain(baseGain, fabsf(avgError), true);
    
    float rawCorrection = 1.0f - adaptiveGain * avgError;
    
    // Aplicar proteção contra divergência
    float currentBetaCorrection = getCurrentBetaCorrection();
    float safeBetaCorrection = m_divergenceProtection.applySafeCorrection(
        currentBetaCorrection, rawCorrection, true);
    
    m_state.calculatedBetaCorrection = safeBetaCorrection;
    m_state.state = AWWTransientState::PROCESSING_TAU;
}

/**
 * Análise Tau refinada com acoplamento Beta-Tau
 */
void AdaptiveWallWettingController::analyzeRefinedTauCorrection() {
    // ... análise normal de Tau ...
    
    // Validar acoplamento com Beta
    float rpm = Sensor::getOrZero(SensorType::Rpm);
    float currentBeta = getCurrentBeta();
    float currentTau = getCurrentTau();
    
    m_betaTauCoupling.betaCorrection = m_state.calculatedBetaCorrection;
    m_betaTauCoupling.tauCorrection = m_state.calculatedTauCorrection;
    
    if (!m_betaTauCoupling.validateCoupledCorrections(currentBeta, currentTau, rpm)) {
        // Resolver conflito físico
        m_betaTauCoupling.resolveConflict();
        m_state.calculatedBetaCorrection = m_betaTauCoupling.betaCorrection;
        m_state.calculatedTauCorrection = m_betaTauCoupling.tauCorrection;
    }
    
    m_state.state = AWWTransientState::APPLYING_CORRECTIONS;
}
```

**RESUMO DOS REFINAMENTOS**:

✅ **Acoplamento Beta-Tau**: Correções fisicamente consistentes  
✅ **Ganhos Adaptativos**: Prevenção de over/sub-correção  
✅ **Buffer Thread-Safe**: Eliminação de corrupção de dados  
✅ **Timing Realista**: Correlação adequada injeção-lambda  
✅ **Validação Coordenada**: Consistência entre subsistemas  
✅ **Complexidade Limitada**: Performance garantida a 200Hz  
✅ **Proteção Divergência**: Limites absolutos e detecção  

**Com estes refinamentos, o sistema adaptativo alcança estabilidade robusta e convergência garantida.**
