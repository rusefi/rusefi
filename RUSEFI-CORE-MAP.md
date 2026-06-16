# RUSEFI-CORE-MAP

Mapa do fluxo de sinal **borda do Hall → faísca/injeção** no rusEFI.
Escopo restrito a `trigger/`, `engine_cycle/`, `algo/` (fuel/advance/airmass/ignition)
e `math/` (speed_density, engine_math).

> Convenção: toda afirmação tem referência `arquivo:linha`. As equações são
> transcritas com os nomes de variável reais do código. O que não foi encontrado
> dentro do escopo está marcado como **NÃO LOCALIZADO**.

---

## 1. SINAL DE ROTAÇÃO

### 1.1 Onde a borda do sensor de crank entra

A borda física entra por uma callback de interrupção EXTI (PAL/ChibiOS), fora do
escopo de `controllers/` mas é a origem do sinal:

- ISR de hardware: `firmware/hw_layer/digital_input/trigger/trigger_input_exti.cpp:27`
  função `shaft_callback(void *arg, efitick_t stamp)`.
  - Lê o nível atual do pino: `bool rise = (palReadLine(pal_line) == PAL_HIGH);`
    (`trigger_input_exti.cpp:31`).
  - Encaminha para o core: `hwHandleShaftSignal(index, rise, stamp);`
    (`trigger_input_exti.cpp:36`).

Ponto de entrada no escopo (`controllers/trigger/`):

- `hwHandleShaftSignal(int signalIndex, bool isRising, efitick_t timestamp)`
  em `firmware/controllers/trigger/trigger_central.cpp:481`. Filtra ruído/self-stim
  (`trigger_central.cpp:488`) e chama `handleShaftSignal(...)` (`trigger_central.cpp:493`).
- `handleShaftSignal(...)` (função livre) em `trigger_central.cpp:497`. Converte a
  borda física em `trigger_event_e` respeitando os flags de inversão
  (`trigger_central.cpp:505-513`) e chama o método de instância:
  `getTriggerCentral()->handleShaftSignal(signal, timestamp);` (`trigger_central.cpp:552`).
- `TriggerCentral::handleShaftSignal(trigger_event_e signal, efitick_t timestamp)`
  em `trigger_central.cpp:811` — o coração do processamento por dente.

### 1.2 Como o decoder identifica o dente e detecta o sync do 60-2 (lógica da falha)

O decode é feito por `TriggerDecoderBase::decodeTriggerEvent(...)` em
`firmware/controllers/trigger/trigger_decoder.cpp:464`, invocado a partir de
`TriggerCentral::handleShaftSignal` em `trigger_central.cpp:852`.

**Medição da largura do dente (tempo entre bordas):**

- `efidur_t currentDurationLong = isFirstEvent ? 0 : (nowNt - toothed_previous_time);`
  (`trigger_decoder.cpp:508`)
- `toothDurations[0] = currentDurationLong > 10 * NT_PER_SECOND ? 10 * NT_PER_SECOND : currentDurationLong;`
  (`trigger_decoder.cpp:514-515`)

**Razão de gap (a chave para detectar a falha do 60-2):**

- `triggerSyncGapRatio = (float)toothDurations[0] / toothDurations[1];`
  (`trigger_decoder.cpp:547`)
- `isSynchronizationPoint = isSyncPoint(triggerShape, triggerConfiguration.TriggerType.type);`
  (`trigger_decoder.cpp:553`)

**A lógica da falha está em `TriggerDecoderBase::isSyncPoint(...)`
(`trigger_decoder.cpp:705`).** Para uma roda 60-2 o dente faltante produz um gap
~2x maior que o dente normal; isso é testado comparando a duração atual contra a
anterior dentro de uma janela `[from, to]` (transformada para multiplicação em vez
de divisão):

```c
// trigger_decoder.cpp:745-747
bool isGapCondition =
      (toothDurations[i] > toothDurations[i + 1] * from
    && toothDurations[i] < toothDurations[i + 1] * to);
```
(loop sobre `i` de 0 a `triggerShape.gapTrackingLength`, `trigger_decoder.cpp:732-752`;
`from`/`to` vêm de `synchronizationRatioFrom[i]`/`synchronizationRatioTo[i]`,
`trigger_decoder.cpp:733-734`). Se **todos** os gaps rastreados estão na janela,
retorna `true` (`trigger_decoder.cpp:754`) — esse é o ponto de sincronização.

**Validação de "dente perdido" / sync incorreto** (em `decodeTriggerEvent`):
ao atingir o ponto de sync, conta-se o número de eventos desde o último sync; se
estava sincronizado mas o número está errado (`isDecodingError`), perde-se o sync:

```c
// trigger_decoder.cpp:631-644
if (wasSynchronized && isDecodingError) {
    setTriggerErrorState();
    onNotEnoughTeeth(currentCycle.current_index, triggerShape.getSize());
    setShaftSynchronized(false);   // não estamos mais sincronizados
    onTriggerError();
    printGaps("newerr", triggerConfiguration, triggerShape);
} else {
    setShaftSynchronized(true);    // primeiro sync OU sem erro: sincronizado!
}
```

Também há proteção de "dentes demais": se passou do índice esperado sem sync,
`setShaftSynchronized(false)` e retorna `unexpected` (`trigger_decoder.cpp:668-682`).

Retorno: `TriggerDecodeResult{ currentCycle.current_index }` se sincronizado,
caso contrário `unexpected` (`trigger_decoder.cpp:698-702`).

### 1.3 Como o RPM é calculado a partir do tempo entre dentes

Há **dois** caminhos de RPM (confirmado no comentário de `rpm_calculator.cpp:9-10`).

**(a) RPM de ciclo** — uma vez por revolução, em `rpmShaftPositionCallback(...)`
(`firmware/controllers/engine_cycle/rpm_calculator.cpp:289`), somente quando
`trgEventIndex == 0` (`rpm_calculator.cpp:296`):

```c
// rpm_calculator.cpp:304
float periodSeconds = engine->rpmCalculator.lastTdcTimer.getElapsedSecondsAndReset(nowNt);
...
// rpm_calculator.cpp:320-321
int mult = (int)getEngineCycle(getEngineRotationState()->getOperationMode()) / 360;
float rpm = 60 * mult / periodSeconds;
```
(o `mult` compensa que o evento de sinal já está ajustado "por ciclo de motor";
ver comentário `rpm_calculator.cpp:307-313`).

**(b) RPM instantâneo** — por dente, em
`InstantRpmCalculator::calculateInstantRpm(...)`
(`firmware/controllers/trigger/instant_rpm_calculator.cpp:62`). Mede o tempo entre
o dente atual e um dente ~90° atrás:

```c
// instant_rpm_calculator.cpp:85-87
angle_t previousAngle = currentAngle - 90;
wrapAngle(previousAngle, "prevAngle", ObdCode::CUSTOM_ERR_TRIGGER_ANGLE_RANGE);
int prevIndex = triggerShape.findAngleIndex(triggerFormDetails, previousAngle);
...
// instant_rpm_calculator.cpp:113
uint32_t time = nowNt32 - time90ago;
...
// instant_rpm_calculator.cpp:120  (equação real do RPM instantâneo)
float instantRpm = (60000000.0 / 360 * US_TO_NT_MULTIPLIER) * angleDiff / time;
```

A escolha entre (a) e (b) ocorre em `rpmShaftPositionCallback`: `alwaysInstantRpm`
usa o instantâneo (`rpm_calculator.cpp:347-348`); durante "spinning up" também
(`rpm_calculator.cpp:349-350`); caso contrário usa o de ciclo via `setRpmValue`
(`rpm_calculator.cpp:326`).

**Derivada `oneDegreeUs`** (tempo de 1° de virabrequim) é recalculada a cada novo
RPM em `RpmCalculator::assignRpmValue(...)`:

```c
// rpm_calculator.cpp:158
oneDegreeUs = getOneDegreeTimeUs(floatRpmValue);
```
com a macro em `firmware/controllers/math/engine_math.h:27`:
```c
#define getOneDegreeTimeUs(rpm) (1000000.0f * 60 / 360 / (rpm))
```
(e a versão em ms em `engine_math.h:21`:
`#define getOneDegreeTimeMs(rpm) (1000.0f * 60 / 360 / (rpm))`).

### 1.4 Como o ângulo de virabrequim instantâneo é derivado

Duas peças:

1. **Fase do último dente decodificado** (espaço-motor), gravada em
   `TriggerCentral::handleShaftSignal`:
```c
// trigger_central.cpp:874
auto currentPhaseFromSyncPoint = getTriggerCentral()->triggerFormDetails.eventAngles[triggerIndexForListeners];
// trigger_central.cpp:877
currentEngineDecodedPhase = wrapAngleMethod(currentPhaseFromSyncPoint - tdcPosition(), "currentEnginePhase", ObdCode::CUSTOM_ERR_6555);
```
   e `m_lastToothPhaseFromSyncPoint = currentPhaseFromSyncPoint;` sob lock
   (`trigger_central.cpp:886`), junto com `m_lastToothTimer.reset(timestamp)`
   (`trigger_central.cpp:885`).

2. **Interpolação no tempo** entre dentes — ângulo instantâneo a qualquer momento
   `nowNt`, em `TriggerCentral::getCurrentEnginePhase(...)`:
```c
// trigger_central.cpp:82-86
elapsed = m_lastToothTimer.getElapsedUs(nowNt);
toothPhase = m_lastToothPhaseFromSyncPoint;
...
return toothPhase + elapsed / oneDegreeUs;
```
   Ou seja: `fase = fase_do_último_dente + tempo_decorrido / tempo_por_grau`.

O próximo ângulo de dente esperado (usado para agendamento e rejeição de dentes
espúrios) é calculado em `TriggerCentral::findNextTriggerToothAngle(...)`
(`trigger_central.cpp:786`) e publicado como `expectedNextPhase`
(`trigger_central.cpp:903-907`).

---

## 2. INJEÇÃO

### 2.1 A equação real de massa de combustível

Entrada principal: `getInjectionMass(float rpm)` em
`firmware/controllers/algo/fuel_math.cpp:312`.

```c
// fuel_math.cpp:316-328
float baseFuelMass = getBaseFuelMass(rpm);
bool isCranking = engine->rpmCalculator.isCranking();
float cycleFuelMass = getCycleFuelMass(isCranking, baseFuelMass);
...
float durationMultiplier = getInjectionModeDurationMultiplier();
float injectionFuelMass = cycleFuelMass * durationMultiplier;
...
// fuel_math.cpp:370
return injectionFuelMass + tpsFuelMass;
```

**Massa base por cilindro** — `getBaseFuelMass(float rpm)` (`fuel_math.cpp:183`):
```c
// fuel_math.cpp:190
auto airmass = model->getAirmass(rpm, true);
...
// fuel_math.cpp:205
float baseFuelMass = engine->fuelComputer.getCycleFuel(airmass.CylinderAirmass, rpm, airmass.EngineLoadPercent);
```

**Conversão ar → combustível** — `FuelComputerBase::getCycleFuel(...)`
(`firmware/controllers/algo/fuel/fuel_computer.cpp:13`):
```c
// fuel_computer.cpp:16-25
float stoich = getStoichiometricRatio();
float lambda = getTargetLambda(rpm, load);
float afr = stoich * lambda;
...
return airmass / afr;        // massa de combustível = massa de ar / AFR-alvo
```

**Correções de regime** — `getCycleFuelMass` (`fuel_math.cpp:300`) escolhe entre
cranking e running. Em running (`getRunningFuel`, `fuel_math.cpp:111`):
```c
// fuel_math.cpp:123
float correction = baroCorrection * iatCorrection * cltCorrection * postCrankingFuelCorrection;
...
// fuel_math.cpp:140
float runningFuel = baseFuel * correction;
```

### 2.2 Modelo de massa de ar usado em speed-density

`SpeedDensityAirmass::getAirmass(...)` em
`firmware/controllers/algo/airmass/speed_density_airmass.cpp:14`:
```c
// speed_density_airmass.cpp:18-26
float tChargeK = engine->engineState.sd.tChargeK;
...
float ve = getVe(rpm, map, postState);
float airMass = getAirmassImpl(ve, map, tChargeK);
```

Núcleo (lei dos gases ideais) em
`firmware/controllers/algo/airmass/speed_density_base.cpp`:
```c
// speed_density_base.cpp:20
#define AIR_R 0.28705f          // R/massa_molar_ar = 8.31/28.97 [J*K/g]

// speed_density_base.cpp:22-24
mass_t idealGasLaw(float volume, float pressure, float temperature) {
    return volume * pressure / (AIR_R * temperature);
}

// speed_density_base.cpp:26-28
/*static*/ mass_t SpeedDensityBase::getAirmassImpl(float ve, float manifoldPressure, float temperature) {
    mass_t cycleAir = ve * idealGasLaw(engineConfiguration->displacement, manifoldPressure, temperature);
    return cycleAir / engineConfiguration->cylindersCount;   // massa de ar por cilindro
}
```
Ou seja: `m_ar_cyl = VE * (displacement * MAP) / (0.28705 * T_carga) / nCyl`.

O `MAP` usado vem de `SpeedDensityAirmass::getMap(...)`
(`speed_density_airmass.cpp:152`), com sensor real ou tabela de estimativa de
fallback (`speed_density_airmass.cpp:142-157`).

### 2.3 Como o evento de abertura/fechamento do bico é agendado por ÂNGULO

**Massa e duração são pré-calculadas** em `EngineState::periodicFastCallback()`
(`firmware/controllers/algo/engine2.cpp:154`):
```c
// engine2.cpp:194  (massa total, antes de trims)
float untrimmedInjectionMass = getInjectionMass(rpm) * engine->engineState.lua.fuelMult + engine->engineState.lua.fuelAdd;
// engine2.cpp:208  (duração só para fins de agendamento)
engine->engineState.injectionDuration = engine->module<InjectorModelPrimary>()->getInjectionDuration(stage1InjectionMass);
// engine2.cpp:214
injectionOffset = getInjectionOffset(rpm, fuelLoad);
// engine2.cpp:251  (massa final por cilindro, com trims)
engine->engineState.injectionMass[cylinderIndex] = untrimmedInjectionMass * bankTrim * cylinderTrim * knockTrim;
```

**Massa → duração** em `InjectorModelBase::getBaseDurationImpl(...)`
(`firmware/controllers/algo/fuel/injector_model.cpp:250`):
```c
// injector_model.cpp:251
floatms_t baseDuration = fuelMassGram / m_massFlowRate * 1000;
```
com adição do deadtime do injetor: `return baseDuration + m_deadtime;`
(`injector_model.cpp:201` e `:226`).

**Ângulo de abertura** em `InjectionEvent::computeInjectionAngle()`
(`firmware/controllers/engine_cycle/fuel_schedule.cpp:78`):
```c
// fuel_schedule.cpp:93  (correção para modo Start/Center/End)
angle_t injectionDurationAngle = getInjectionAngleCorrection(fuelMs, oneDegreeUs);
// fuel_schedule.cpp:96-102
floatus_t injectionOffset = getEngineState()->injectionOffset;
...
angle_t openingAngle = injectionOffset - injectionDurationAngle;
// fuel_schedule.cpp:109  (corrige para a ordem de injeção)
openingAngle += getPerCylinderFiringOrderOffset(ownIndex, cylinderNumber);
```
(a conversão duração→ângulo em `getInjectionAngleCorrection`:
`injectionDurationAngle = MS2US(fuelMs) / oneDegreeUs;`, `fuel_schedule.cpp:63`).

**Agendamento por ângulo** em `InjectionEvent::onTriggerTooth(...)`
(`firmware/controllers/engine_cycle/main_trigger_callback.cpp:78`), chamado a cada
dente via `handleFuel` → `mainTriggerCallback` (`main_trigger_callback.cpp:303`):
```c
// main_trigger_callback.cpp:79-82  (só age se o ângulo cai antes do próximo dente)
auto eventAngle = injectionStartAngle;
if (!isPhaseInRange(eventAngle, currentPhase, nextPhase)) {
    return;
}
...
// main_trigger_callback.cpp:93  (massa do cilindro correto)
auto injectionMassGrams = getEngineState()->injectionMass[this->cylinderNumber];
// main_trigger_callback.cpp:98  (wall wetting na MASSA, não na duração)
injectionMassGrams = wallFuel.adjust(injectionMassGrams);
...
// main_trigger_callback.cpp:199-201  (ângulo até a abertura, a partir de agora)
float angleFromNow = eventAngle - currentPhase;
if (angleFromNow < 0) {
    angleFromNow += getEngineState()->engineCycle;
}
// main_trigger_callback.cpp:205  (ABRE: agendado por ÂNGULO)
efitick_t startTime = scheduleByAngle(nullptr, nowNt, angleFromNow, startAction);
// main_trigger_callback.cpp:208-209  (FECHA: tempo = abertura + duração, por TEMPO)
efitick_t turnOffTimeStage1 = startTime + US2NT((int)durationUsStage1);
getScheduler()->schedule("inj", nullptr, turnOffTimeStage1, endActionStage1);
```
Resumo: **abertura é por ângulo**; **fechamento é por tempo** (instante de abertura
+ duração da injeção em µs). A pin é aberta em `turnInjectionPinHigh`
(`fuel_schedule.cpp:11`) e fechada em `turnInjectionPinLow` (`main_trigger_callback.cpp:55`).

---

## 3. IGNIÇÃO

### 3.1 Como o avanço é buscado na tabela

Tabela base 3D (RPM × carga) em
`IgnitionState::getInterpolatedIgnitionAngle(...)`
(`firmware/controllers/algo/ignition/ignition_state.cpp:406`):
```c
// ignition_state.cpp:407-411
return interpolate3d(
    config->ignitionTable,
    config->ignitionLoadBins, ignitionLoad,
    config->ignitionRpmBins, rpm
);
```
Consumida por `getRunningAdvance(...)` (`ignition_state.cpp:37`):
```c
// ignition_state.cpp:46
float advanceAngle = IgnitionState::getInterpolatedIgnitionAngle(rpm, engineLoad);
```
(somam-se aqui correções: 2ª tabela/blend `:56-77`, traction `:86`, ALS `:96`,
ignBlends `:101-106`). O seletor cranking/running é `IgnitionState::getAdvance(...)`
(`ignition_state.cpp:243`); `getWrappedAdvance(...)` aplica mult/add do Lua e faz
`wrapAngle` (`ignition_state.cpp:282-285`). As correções IAT/CLT/PID/DFCO vêm de
`getAdvanceCorrections(...)` (`ignition_state.cpp:196-225`).

Resultado final por cilindro gravado em `EngineState::periodicFastCallback`:
```c
// engine2.cpp:224
float baseAdvance = engine->ignitionState.getWrappedAdvance(rpm, l_ignitionLoad);
// engine2.cpp:231
float correctedIgnitionAdvance = baseAdvance + corrections;   // knock + soft-limit
// engine2.cpp:260
timingAdvance[cylinderIndex] = cylinderIgnitionAdvance;
```

### 3.2 Como o dwell é calculado

`IgnitionState::getSparkDwell(float rpm, bool isCranking)`
(`firmware/controllers/algo/ignition/ignition_state.cpp:345`):
```c
// ignition_state.cpp:347-364
if (isCranking) {
    dwellMs = engineConfiguration->ignitionDwellForCrankingMs;
} else {
    baseDwell = interpolate2d(rpm, config->sparkDwellRpmBins, config->sparkDwellValues);
    dwellVoltageCorrection = interpolate2d(
        Sensor::getOrZero(SensorType::BatteryVoltage),
        config->dwellVoltageCorrVoltBins,
        config->dwellVoltageCorrValues
    );
    if (dwellVoltageCorrection < 0.1f) { dwellVoltageCorrection = 1; }
    dwellMs = baseDwell * dwellVoltageCorrection;
}
```

Conversão dwell tempo → ângulo em `IgnitionState::updateDwell(...)`
(`ignition_state.cpp:375`):
```c
// ignition_state.cpp:376-377
sparkDwell = getSparkDwell(rpm, isCranking);
dwellDurationAngle = std::isnan(rpm) ? NAN : getDwell() / getOneDegreeTimeMs(rpm);
```
(`updateDwell` é chamado de `EngineState::periodicFastCallback`, `engine2.cpp:176`).

### 3.3 Como o evento de dwell-start e de faísca é agendado por ângulo

**Cálculo dos ângulos por cilindro** em `prepareCylinderIgnitionSchedule(...)`
(`firmware/controllers/engine_cycle/spark_logic.cpp:85`):
```c
// spark_logic.cpp:102
angle_t finalIgnitionTiming = getEngineState()->timingAdvance[coilIndex];
...
// spark_logic.cpp:123-127  (avanço BTDC vira ângulo ATDC + offset de ordem)
angle_t sparkAngle =
    - finalIgnitionTiming
    + getPerCylinderFiringOrderOffset(event->cylinderIndex, coilIndex);
wrapAngle(sparkAngle, "findAngle#2", ObdCode::CUSTOM_ERR_6550);   // :130
event->sparkAngle = sparkAngle;                                   // :131
...
// spark_logic.cpp:157  (início do dwell = faísca menos a duração angular do dwell)
angle_t dwellStartAngle = sparkAngle - dwellAngleDuration;
event->dwellAngle = dwellStartAngle;                              // :162
```

**Disparo por dente** em `onTriggerEventSparkLogic(...)` (`spark_logic.cpp:556`),
chamado por `mainTriggerCallback` (`main_trigger_callback.cpp:311`). Só agenda
quando o ângulo de início de dwell cai na janela do dente atual:
```c
// spark_logic.cpp:627
if (!isOddCylWastedEvent && !isPhaseInRange(dwellAngle, currentPhase, nextPhase)) {
    continue;
}
...
// spark_logic.cpp:670
scheduleSparkEvent(limitedSpark, event, rpm, dwellMs, dwellAngle, sparkAngle, edgeTimestamp, currentPhase, nextPhase);
```

**Agendamento concreto** em `scheduleSparkEvent(...)` (`spark_logic.cpp:379`):
```c
// spark_logic.cpp:383-386  (offset angular do início de carga)
float angleOffset = dwellAngle - currentPhase;
if (angleOffset < 0) {
    angleOffset += engine->engineState.engineCycle;
}
...
// spark_logic.cpp:423  (INÍCIO DO DWELL: agendado por ÂNGULO)
chargeTime = scheduleByAngle(&event->dwellStartTimer, edgeTimestamp, angleOffset,
                             action_s::make<turnSparkPinHighStartCharging>( event ));
...
// spark_logic.cpp:461-465  (FAÍSCA: agendada por ângulo OU enfileirada por dente)
bool isTimeScheduled = engine->module<TriggerScheduler>()->scheduleOrQueue(
    "spark",
    &event->sparkEvent, edgeTimestamp, sparkAngle,
    action_s::make<fireSparkAndPrepareNextSchedule>( event ),
    currentPhase, nextPhase);
```
- A bobina é carregada (pino alto) em `turnSparkPinHighStartCharging`
  (`spark_logic.cpp:332`) → `startDwellByTurningSparkPinHigh` → `output->setHigh()`
  (`spark_logic.cpp:328`).
- A faísca dispara (pino baixo) em `fireSparkAndPrepareNextSchedule`
  (`spark_logic.cpp:209`) → `fireSparkBySettingPinLow` → `output->setLow()`
  (`spark_logic.cpp:51`). Esse mesmo handler **replaneja o próximo ciclo** chamando
  `prepareCylinderIgnitionSchedule` (`spark_logic.cpp:280`) — cada `IgnitionEvent`
  se reagenda sozinho (comentário `spark_logic.cpp:206-207`).
- Proteção contra over-dwell se a faísca foi só enfileirada: agenda fogo forçado a
  1.5× o dwell nominal (`spark_logic.cpp:477-493`).

### 3.4 Como ângulo-alvo vira tempo (a ponte com o executor/timer)

A ponte ângulo→tempo é `scheduleByAngle(...)` em
`firmware/controllers/engine_cycle/rpm_calculator.cpp:417`:
```c
// rpm_calculator.cpp:418-422
float delayUs = engine->rpmCalculator.oneDegreeUs * angle;
efitick_t actionTimeNt = sumTickAndFloat(nowNt, USF2NT(delayUs));
engine->scheduler.schedule("angle", timer, actionTimeNt, action);
return actionTimeNt;
```
Ou seja, o atraso em µs é `oneDegreeUs * angle` (e `oneDegreeUs` vem de
`getOneDegreeTimeUs(rpm)`, ver §1.3), convertido em ticks e entregue ao
`engine->scheduler`. Esta mesma função é usada tanto pela injeção (§2.3,
`main_trigger_callback.cpp:205`) quanto pela ignição (§3.3,
`spark_logic.cpp:423`). O fechamento do injetor e o disparo "asap" de multispark
usam agendamento puramente por tempo (`main_trigger_callback.cpp:209`,
`spark_logic.cpp:264-265`).

---

## DIAGRAMA DE FLUXO (borda do Hall → faísca)

```
[Borda física no pino de crank]
        │  EXTI ISR
        ▼
shaft_callback()                          hw_layer/.../trigger_input_exti.cpp:27
  palReadLine() → rise                                                      :31
        │ hwHandleShaftSignal(index, rise, stamp)                           :36
        ▼
hwHandleShaftSignal()                     trigger/trigger_central.cpp:481
        │ handleShaftSignal() [livre]      → trigger_event_e               :497-513
        ▼
TriggerCentral::handleShaftSignal()       trigger/trigger_central.cpp:811
        │
        ├─► TriggerDecoderBase::decodeTriggerEvent()   trigger_decoder.cpp:464
        │       toothDurations[0] = nowNt - toothed_previous_time          :514
        │       triggerSyncGapRatio = toothDurations[0]/toothDurations[1]  :547
        │       isSyncPoint(): gap em [from,to]  → SYNC do 60-2            :705,745
        │       setShaftSynchronized()                                     :636,643
        │
        ├─► currentEngineDecodedPhase = eventAngles[idx] - tdcPosition()   :874-877
        │   (ângulo instantâneo: getCurrentEnginePhase = fase + Δt/oneDegreeUs :86)
        │
        ├─► rpmShaftPositionCallback()              rpm_calculator.cpp:289
        │       rpm = 60 * mult / periodSeconds      (ciclo)               :321
        │       instantRpm = (60e6/360*US_TO_NT) * angleDiff/time          instant_rpm_calculator.cpp:120
        │       oneDegreeUs = getOneDegreeTimeUs(rpm)                      rpm_calculator.cpp:158
        │
        ▼
mainTriggerCallback()                     engine_cycle/main_trigger_callback.cpp:264
        │
   ┌────┴───────────────────────────────────────────┐
   ▼                                                 ▼
handleFuel() :303                          onTriggerEventSparkLogic() :311
   │ FuelSchedule::onTriggerTooth                    │ (spark_logic.cpp:556)
   ▼                                                 ▼
InjectionEvent::onTriggerTooth()           prepareCylinderIgnitionSchedule()
   main_trigger_callback.cpp:78               spark_logic.cpp:85
   │ massa: injectionMass[cyl] (engine2.cpp:251)     │ sparkAngle = -timing + offset :123
   │   ← getInjectionMass (fuel_math.cpp:312)        │ dwellStartAngle = sparkAngle
   │   ← airmass/AFR (fuel_computer.cpp:25,           │                   - dwellAngleDuration :157
   │      speed_density_base.cpp:27 idealGasLaw)     │   ← dwell: getSparkDwell (ignition_state.cpp:352)
   │ duração: mass/m_massFlowRate*1000+deadtime      │   ← avanço: ignitionTable interpolate3d :407
   │   (injector_model.cpp:251)                      ▼
   │ angleFromNow = eventAngle - currentPhase :199   scheduleSparkEvent() spark_logic.cpp:379
   ▼                                                 │ angleOffset = dwellAngle - currentPhase :383
scheduleByAngle(open) :205 ───┐                      ▼
schedule(close, +durUs) :209  │            scheduleByAngle(dwell start) :423 ──┐
                              │            scheduleOrQueue(spark fire)  :461   │
                              ▼                                                ▼
                    ╔══════════════════════════════════════════════════════════╗
                    ║ scheduleByAngle()  rpm_calculator.cpp:417                  ║
                    ║   delayUs = oneDegreeUs * angle          (ÂNGULO→TEMPO)    ║
                    ║   actionTimeNt = nowNt + USF2NT(delayUs)                   ║
                    ║   engine->scheduler.schedule(...)                          ║
                    ╚══════════════════════════════════════════════════════════╝
                              │                                    │
                              ▼                                    ▼
                 turnInjectionPinHigh/Low            turnSparkPinHighStartCharging :332 (dwell)
                 (fuel_schedule.cpp:11 /              fireSparkAndPrepareNextSchedule :209
                  main_trigger_callback.cpp:55)          → output->setLow()  ⚡ FAÍSCA  :51
```
