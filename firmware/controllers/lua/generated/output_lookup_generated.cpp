#include "pch.h"
float getOutputValueByName(const char *name) {
	if (strEqualCaseInsensitive(name, "sd_present"))
		return engine->outputChannels.sd_present;
	if (strEqualCaseInsensitive(name, "isIgnitionEnabledIndicator"))
		return engine->outputChannels.isIgnitionEnabledIndicator;
	if (strEqualCaseInsensitive(name, "isInjectionEnabledIndicator"))
		return engine->outputChannels.isInjectionEnabledIndicator;
	if (strEqualCaseInsensitive(name, "sd_logging_internal"))
		return engine->outputChannels.sd_logging_internal;
	if (strEqualCaseInsensitive(name, "isCylinderCleanupActivated"))
		return engine->outputChannels.isCylinderCleanupActivated;
	if (strEqualCaseInsensitive(name, "isFuelPumpOn"))
		return engine->outputChannels.isFuelPumpOn;
	if (strEqualCaseInsensitive(name, "isFanOn"))
		return engine->outputChannels.isFanOn;
	if (strEqualCaseInsensitive(name, "isO2HeaterOn"))
		return engine->outputChannels.isO2HeaterOn;
	if (strEqualCaseInsensitive(name, "checkEngine"))
		return engine->outputChannels.checkEngine;
	if (strEqualCaseInsensitive(name, "needBurn"))
		return engine->outputChannels.needBurn;
	if (strEqualCaseInsensitive(name, "sd_msd"))
		return engine->outputChannels.sd_msd;
	if (strEqualCaseInsensitive(name, "clutchUpState"))
		return engine->outputChannels.clutchUpState;
	if (strEqualCaseInsensitive(name, "clutchDownState"))
		return engine->outputChannels.clutchDownState;
	if (strEqualCaseInsensitive(name, "isFan2On"))
		return engine->outputChannels.isFan2On;
	if (strEqualCaseInsensitive(name, "alternatorOnOff"))
		return engine->outputChannels.alternatorOnOff;
	if (strEqualCaseInsensitive(name, "brakePedalState"))
		return engine->outputChannels.brakePedalState;
	if (strEqualCaseInsensitive(name, "toothLogReady"))
		return engine->outputChannels.toothLogReady;
	if (strEqualCaseInsensitive(name, "acSwitchState"))
		return engine->outputChannels.acSwitchState;
	if (strEqualCaseInsensitive(name, "isTpsError"))
		return engine->outputChannels.isTpsError;
	if (strEqualCaseInsensitive(name, "isCltError"))
		return engine->outputChannels.isCltError;
	if (strEqualCaseInsensitive(name, "isMapError"))
		return engine->outputChannels.isMapError;
	if (strEqualCaseInsensitive(name, "isIatError"))
		return engine->outputChannels.isIatError;
	if (strEqualCaseInsensitive(name, "acState"))
		return engine->outputChannels.acState;
	if (strEqualCaseInsensitive(name, "isTriggerError"))
		return engine->outputChannels.isTriggerError;
	if (strEqualCaseInsensitive(name, "hasCriticalError"))
		return engine->outputChannels.hasCriticalError;
	if (strEqualCaseInsensitive(name, "isWarnNow"))
		return engine->outputChannels.isWarnNow;
	if (strEqualCaseInsensitive(name, "isPedalError"))
		return engine->outputChannels.isPedalError;
	if (strEqualCaseInsensitive(name, "isKnockChipOk"))
		return engine->outputChannels.isKnockChipOk;
	if (strEqualCaseInsensitive(name, "launchTriggered"))
		return engine->outputChannels.launchTriggered;
	if (strEqualCaseInsensitive(name, "isTps2Error"))
		return engine->outputChannels.isTps2Error;
	if (strEqualCaseInsensitive(name, "isIdleClosedLoop"))
		return engine->outputChannels.isIdleClosedLoop;
	if (strEqualCaseInsensitive(name, "isIdleCoasting"))
		return engine->outputChannels.isIdleCoasting;
	if (strEqualCaseInsensitive(name, "RPMValue"))
		return engine->outputChannels.RPMValue;
	if (strEqualCaseInsensitive(name, "rpmAcceleration"))
		return engine->outputChannels.rpmAcceleration;
	if (strEqualCaseInsensitive(name, "speedToRpmRatio"))
		return engine->outputChannels.speedToRpmRatio;
	if (strEqualCaseInsensitive(name, "vehicleSpeedKph"))
		return engine->outputChannels.vehicleSpeedKph;
	if (strEqualCaseInsensitive(name, "internalMcuTemperature"))
		return engine->outputChannels.internalMcuTemperature;
	if (strEqualCaseInsensitive(name, "coolant"))
		return engine->outputChannels.coolant;
	if (strEqualCaseInsensitive(name, "intake"))
		return engine->outputChannels.intake;
	if (strEqualCaseInsensitive(name, "auxTemp1"))
		return engine->outputChannels.auxTemp1;
	if (strEqualCaseInsensitive(name, "auxTemp2"))
		return engine->outputChannels.auxTemp2;
	if (strEqualCaseInsensitive(name, "TPSValue"))
		return engine->outputChannels.TPSValue;
	if (strEqualCaseInsensitive(name, "throttlePedalPosition"))
		return engine->outputChannels.throttlePedalPosition;
	if (strEqualCaseInsensitive(name, "tpsADC"))
		return engine->outputChannels.tpsADC;
	if (strEqualCaseInsensitive(name, "MAFValue"))
		return engine->outputChannels.MAFValue;
	if (strEqualCaseInsensitive(name, "massAirFlowValue"))
		return engine->outputChannels.massAirFlowValue;
	if (strEqualCaseInsensitive(name, "MAPValue"))
		return engine->outputChannels.MAPValue;
	if (strEqualCaseInsensitive(name, "baroPressure"))
		return engine->outputChannels.baroPressure;
	if (strEqualCaseInsensitive(name, "lambdaValue"))
		return engine->outputChannels.lambdaValue;
	if (strEqualCaseInsensitive(name, "knockRetard"))
		return engine->outputChannels.knockRetard;
	if (strEqualCaseInsensitive(name, "idleCurrentPosition"))
		return engine->outputChannels.idleCurrentPosition;
	if (strEqualCaseInsensitive(name, "VBatt"))
		return engine->outputChannels.VBatt;
	if (strEqualCaseInsensitive(name, "oilPressure"))
		return engine->outputChannels.oilPressure;
	if (strEqualCaseInsensitive(name, "vvtPositionB1I"))
		return engine->outputChannels.vvtPositionB1I;
	if (strEqualCaseInsensitive(name, "chargeAirMass"))
		return engine->outputChannels.chargeAirMass;
	if (strEqualCaseInsensitive(name, "crankingFuelMs"))
		return engine->outputChannels.crankingFuelMs;
	if (strEqualCaseInsensitive(name, "currentTargetAfr"))
		return engine->outputChannels.currentTargetAfr;
	if (strEqualCaseInsensitive(name, "baseFuel"))
		return engine->outputChannels.baseFuel;
	if (strEqualCaseInsensitive(name, "fuelRunning"))
		return engine->outputChannels.fuelRunning;
	if (strEqualCaseInsensitive(name, "actualLastInjection"))
		return engine->outputChannels.actualLastInjection;
	if (strEqualCaseInsensitive(name, "injectorDutyCycle"))
		return engine->outputChannels.injectorDutyCycle;
	if (strEqualCaseInsensitive(name, "veValue"))
		return engine->outputChannels.veValue;
	if (strEqualCaseInsensitive(name, "injectionOffset"))
		return engine->outputChannels.injectionOffset;
	if (strEqualCaseInsensitive(name, "tCharge"))
		return engine->outputChannels.tCharge;
	if (strEqualCaseInsensitive(name, "injectorLagMs"))
		return engine->outputChannels.injectorLagMs;
	if (strEqualCaseInsensitive(name, "iatCorrection"))
		return engine->outputChannels.iatCorrection;
	if (strEqualCaseInsensitive(name, "cltCorrection"))
		return engine->outputChannels.cltCorrection;
	if (strEqualCaseInsensitive(name, "baroCorrection"))
		return engine->outputChannels.baroCorrection;
	if (strEqualCaseInsensitive(name, "currentEnginePhase"))
		return engine->outputChannels.currentEnginePhase;
	if (strEqualCaseInsensitive(name, "wallFuelAmount"))
		return engine->outputChannels.wallFuelAmount;
	if (strEqualCaseInsensitive(name, "wallFuelCorrection"))
		return engine->outputChannels.wallFuelCorrection;
	if (strEqualCaseInsensitive(name, "revolutionCounterSinceStart"))
		return engine->outputChannels.revolutionCounterSinceStart;
	if (strEqualCaseInsensitive(name, "deltaTps"))
		return engine->outputChannels.deltaTps;
	if (strEqualCaseInsensitive(name, "canReadCounter"))
		return engine->outputChannels.canReadCounter;
	if (strEqualCaseInsensitive(name, "tpsAccelFuel"))
		return engine->outputChannels.tpsAccelFuel;
	if (strEqualCaseInsensitive(name, "ignitionAdvance"))
		return engine->outputChannels.ignitionAdvance;
	if (strEqualCaseInsensitive(name, "sparkDwellValue"))
		return engine->outputChannels.sparkDwellValue;
	if (strEqualCaseInsensitive(name, "coilDutyCycle"))
		return engine->outputChannels.coilDutyCycle;
	if (strEqualCaseInsensitive(name, "idleAirValvePosition"))
		return engine->outputChannels.idleAirValvePosition;
	if (strEqualCaseInsensitive(name, "etbTarget"))
		return engine->outputChannels.etbTarget;
	if (strEqualCaseInsensitive(name, "etb1DutyCycle"))
		return engine->outputChannels.etb1DutyCycle;
	if (strEqualCaseInsensitive(name, "etb1Error"))
		return engine->outputChannels.etb1Error;
	if (strEqualCaseInsensitive(name, "fuelTankLevel"))
		return engine->outputChannels.fuelTankLevel;
	if (strEqualCaseInsensitive(name, "totalFuelConsumption"))
		return engine->outputChannels.totalFuelConsumption;
	if (strEqualCaseInsensitive(name, "fuelFlowRate"))
		return engine->outputChannels.fuelFlowRate;
	if (strEqualCaseInsensitive(name, "veTableYAxis"))
		return engine->outputChannels.veTableYAxis;
	if (strEqualCaseInsensitive(name, "afrTableYAxis"))
		return engine->outputChannels.afrTableYAxis;
	if (strEqualCaseInsensitive(name, "knockLevel"))
		return engine->outputChannels.knockLevel;
	if (strEqualCaseInsensitive(name, "seconds"))
		return engine->outputChannels.seconds;
	if (strEqualCaseInsensitive(name, "engineMode"))
		return engine->outputChannels.engineMode;
	if (strEqualCaseInsensitive(name, "firmwareVersion"))
		return engine->outputChannels.firmwareVersion;
	if (strEqualCaseInsensitive(name, "tsConfigVersion"))
		return engine->outputChannels.tsConfigVersion;
	if (strEqualCaseInsensitive(name, "calibrationValue"))
		return engine->outputChannels.calibrationValue;
	if (strEqualCaseInsensitive(name, "calibrationMode"))
		return engine->outputChannels.calibrationMode;
	if (strEqualCaseInsensitive(name, "idleTargetPosition"))
		return engine->outputChannels.idleTargetPosition;
	if (strEqualCaseInsensitive(name, "fuelingLoad"))
		return engine->outputChannels.fuelingLoad;
	if (strEqualCaseInsensitive(name, "ignitionLoad"))
		return engine->outputChannels.ignitionLoad;
	if (strEqualCaseInsensitive(name, "engineMakeCodeNameCrc16"))
		return engine->outputChannels.engineMakeCodeNameCrc16;
	if (strEqualCaseInsensitive(name, "totalTriggerErrorCounter"))
		return engine->outputChannels.totalTriggerErrorCounter;
	if (strEqualCaseInsensitive(name, "orderingErrorCounter"))
		return engine->outputChannels.orderingErrorCounter;
	if (strEqualCaseInsensitive(name, "warningCounter"))
		return engine->outputChannels.warningCounter;
	if (strEqualCaseInsensitive(name, "lastErrorCode"))
		return engine->outputChannels.lastErrorCode;
	if (strEqualCaseInsensitive(name, "debugFloatField1"))
		return engine->outputChannels.debugFloatField1;
	if (strEqualCaseInsensitive(name, "debugFloatField2"))
		return engine->outputChannels.debugFloatField2;
	if (strEqualCaseInsensitive(name, "debugFloatField3"))
		return engine->outputChannels.debugFloatField3;
	if (strEqualCaseInsensitive(name, "debugFloatField4"))
		return engine->outputChannels.debugFloatField4;
	if (strEqualCaseInsensitive(name, "debugFloatField5"))
		return engine->outputChannels.debugFloatField5;
	if (strEqualCaseInsensitive(name, "debugFloatField6"))
		return engine->outputChannels.debugFloatField6;
	if (strEqualCaseInsensitive(name, "debugFloatField7"))
		return engine->outputChannels.debugFloatField7;
	if (strEqualCaseInsensitive(name, "debugIntField1"))
		return engine->outputChannels.debugIntField1;
	if (strEqualCaseInsensitive(name, "debugIntField2"))
		return engine->outputChannels.debugIntField2;
	if (strEqualCaseInsensitive(name, "debugIntField3"))
		return engine->outputChannels.debugIntField3;
	if (strEqualCaseInsensitive(name, "debugIntField4"))
		return engine->outputChannels.debugIntField4;
	if (strEqualCaseInsensitive(name, "debugIntField5"))
		return engine->outputChannels.debugIntField5;
	if (strEqualCaseInsensitive(name, "accelerationX"))
		return engine->outputChannels.accelerationX;
	if (strEqualCaseInsensitive(name, "accelerationY"))
		return engine->outputChannels.accelerationY;
	if (strEqualCaseInsensitive(name, "TPS2Value"))
		return engine->outputChannels.TPS2Value;
	if (strEqualCaseInsensitive(name, "rawTps1Primary"))
		return engine->outputChannels.rawTps1Primary;
	if (strEqualCaseInsensitive(name, "rawPpsPrimary"))
		return engine->outputChannels.rawPpsPrimary;
	if (strEqualCaseInsensitive(name, "rawClt"))
		return engine->outputChannels.rawClt;
	if (strEqualCaseInsensitive(name, "rawIat"))
		return engine->outputChannels.rawIat;
	if (strEqualCaseInsensitive(name, "rawOilPressure"))
		return engine->outputChannels.rawOilPressure;
	if (strEqualCaseInsensitive(name, "tuneCrc16"))
		return engine->outputChannels.tuneCrc16;
	if (strEqualCaseInsensitive(name, "fuelClosedLoopBinIdx"))
		return engine->outputChannels.fuelClosedLoopBinIdx;
	if (strEqualCaseInsensitive(name, "tcuCurrentGear"))
		return engine->outputChannels.tcuCurrentGear;
	if (strEqualCaseInsensitive(name, "rawPpsSecondary"))
		return engine->outputChannels.rawPpsSecondary;
	if (strEqualCaseInsensitive(name, "tcuDesiredGear"))
		return engine->outputChannels.tcuDesiredGear;
	if (strEqualCaseInsensitive(name, "flexPercent"))
		return engine->outputChannels.flexPercent;
	if (strEqualCaseInsensitive(name, "rawIdlePositionSensor"))
		return engine->outputChannels.rawIdlePositionSensor;
	if (strEqualCaseInsensitive(name, "rawWastegatePosition"))
		return engine->outputChannels.rawWastegatePosition;
	if (strEqualCaseInsensitive(name, "wastegatePositionSensor"))
		return engine->outputChannels.wastegatePositionSensor;
	if (strEqualCaseInsensitive(name, "idlePositionSensor"))
		return engine->outputChannels.idlePositionSensor;
	if (strEqualCaseInsensitive(name, "rawLowFuelPressure"))
		return engine->outputChannels.rawLowFuelPressure;
	if (strEqualCaseInsensitive(name, "rawHighFuelPressure"))
		return engine->outputChannels.rawHighFuelPressure;
	if (strEqualCaseInsensitive(name, "lowFuelPressure"))
		return engine->outputChannels.lowFuelPressure;
	if (strEqualCaseInsensitive(name, "highFuelPressure"))
		return engine->outputChannels.highFuelPressure;
	if (strEqualCaseInsensitive(name, "targetLambda"))
		return engine->outputChannels.targetLambda;
	if (strEqualCaseInsensitive(name, "AFRValue"))
		return engine->outputChannels.AFRValue;
	if (strEqualCaseInsensitive(name, "VssAcceleration"))
		return engine->outputChannels.VssAcceleration;
	if (strEqualCaseInsensitive(name, "lambdaValue2"))
		return engine->outputChannels.lambdaValue2;
	if (strEqualCaseInsensitive(name, "AFRValue2"))
		return engine->outputChannels.AFRValue2;
	if (strEqualCaseInsensitive(name, "vvtPositionB1E"))
		return engine->outputChannels.vvtPositionB1E;
	if (strEqualCaseInsensitive(name, "vvtPositionB2I"))
		return engine->outputChannels.vvtPositionB2I;
	if (strEqualCaseInsensitive(name, "vvtPositionB2E"))
		return engine->outputChannels.vvtPositionB2E;
	if (strEqualCaseInsensitive(name, "rawTps1Secondary"))
		return engine->outputChannels.rawTps1Secondary;
	if (strEqualCaseInsensitive(name, "rawTps2Primary"))
		return engine->outputChannels.rawTps2Primary;
	if (strEqualCaseInsensitive(name, "rawTps2Secondary"))
		return engine->outputChannels.rawTps2Secondary;
	if (strEqualCaseInsensitive(name, "knockCount"))
		return engine->outputChannels.knockCount;
	if (strEqualCaseInsensitive(name, "accelerationZ"))
		return engine->outputChannels.accelerationZ;
	if (strEqualCaseInsensitive(name, "accelerationRoll"))
		return engine->outputChannels.accelerationRoll;
	if (strEqualCaseInsensitive(name, "accelerationYaw"))
		return engine->outputChannels.accelerationYaw;
	if (strEqualCaseInsensitive(name, "turboSpeed"))
		return engine->outputChannels.turboSpeed;
	if (strEqualCaseInsensitive(name, "launchSpeedCondition"))
		return engine->outputChannels.launchSpeedCondition;
	if (strEqualCaseInsensitive(name, "launchRpmCondition"))
		return engine->outputChannels.launchRpmCondition;
	if (strEqualCaseInsensitive(name, "launchTpsCondition"))
		return engine->outputChannels.launchTpsCondition;
	if (strEqualCaseInsensitive(name, "launchActivateSwitchCondition"))
		return engine->outputChannels.launchActivateSwitchCondition;
	if (strEqualCaseInsensitive(name, "launchIsLaunchCondition"))
		return engine->outputChannels.launchIsLaunchCondition;
	if (strEqualCaseInsensitive(name, "launchCombinedConditions"))
		return engine->outputChannels.launchCombinedConditions;
	if (strEqualCaseInsensitive(name, "launchActivatePinState"))
		return engine->outputChannels.launchActivatePinState;
	if (strEqualCaseInsensitive(name, "TEMPLOG_MAP_AT_CYCLE_COUNT"))
		return engine->outputChannels.TEMPLOG_MAP_AT_CYCLE_COUNT;
	if (strEqualCaseInsensitive(name, "TEMPLOG_MAP_AT_SPECIAL_POINT"))
		return engine->outputChannels.TEMPLOG_MAP_AT_SPECIAL_POINT;
	if (strEqualCaseInsensitive(name, "TEMPLOG_MAP_AT_DIFF"))
		return engine->outputChannels.TEMPLOG_MAP_AT_DIFF;
	if (strEqualCaseInsensitive(name, "vvtSyncGapRatio"))
		return engine->outputChannels.vvtSyncGapRatio;
	if (strEqualCaseInsensitive(name, "vvtCurrentPosition"))
		return engine->outputChannels.vvtCurrentPosition;
	if (strEqualCaseInsensitive(name, "triggerSyncGapRatio"))
		return engine->outputChannels.triggerSyncGapRatio;
	if (strEqualCaseInsensitive(name, "triggerStateIndex"))
		return engine->outputChannels.triggerStateIndex;
	if (strEqualCaseInsensitive(name, "vvtCounter"))
		return engine->outputChannels.vvtCounter;
	if (strEqualCaseInsensitive(name, "vvtSyncCounter"))
		return engine->outputChannels.vvtSyncCounter;
	if (strEqualCaseInsensitive(name, "vvtStateIndex"))
		return engine->outputChannels.vvtStateIndex;
	if (strEqualCaseInsensitive(name, "TEMPLOG_MAP_INSTANT_AVERAGE"))
		return engine->outputChannels.TEMPLOG_MAP_INSTANT_AVERAGE;
	if (strEqualCaseInsensitive(name, "fallbackMap"))
		return engine->outputChannels.fallbackMap;
	if (strEqualCaseInsensitive(name, "TEMPLOG_map_peak"))
		return engine->outputChannels.TEMPLOG_map_peak;
	if (strEqualCaseInsensitive(name, "TEMPLOG_map_length"))
		return engine->outputChannels.TEMPLOG_map_length;
	if (strEqualCaseInsensitive(name, "TEMPLOG_mapIndex"))
		return engine->outputChannels.TEMPLOG_mapIndex;
	if (strEqualCaseInsensitive(name, "timingCltCorrection"))
		return engine->outputChannels.timingCltCorrection;
	if (strEqualCaseInsensitive(name, "timingIatCorrection"))
		return engine->outputChannels.timingIatCorrection;
	if (strEqualCaseInsensitive(name, "timingPidCorrection"))
		return engine->outputChannels.timingPidCorrection;
	if (strEqualCaseInsensitive(name, "instantMAPValue"))
		return engine->outputChannels.instantMAPValue;
	if (strEqualCaseInsensitive(name, "mostRecentTimeBetweenSparkEvents"))
		return engine->outputChannels.mostRecentTimeBetweenSparkEvents;
	if (strEqualCaseInsensitive(name, "mostRecentTimeBetweenIgnitionEvents"))
		return engine->outputChannels.mostRecentTimeBetweenIgnitionEvents;
	if (strEqualCaseInsensitive(name, "maxLockedDuration"))
		return engine->outputChannels.maxLockedDuration;
	if (strEqualCaseInsensitive(name, "maxTriggerReentrant"))
		return engine->outputChannels.maxTriggerReentrant;
	if (strEqualCaseInsensitive(name, "canWriteOk"))
		return engine->outputChannels.canWriteOk;
	if (strEqualCaseInsensitive(name, "canWriteNotOk"))
		return engine->outputChannels.canWriteNotOk;
	if (strEqualCaseInsensitive(name, "triggerPrimaryFall"))
		return engine->outputChannels.triggerPrimaryFall;
	if (strEqualCaseInsensitive(name, "triggerPrimaryRise"))
		return engine->outputChannels.triggerPrimaryRise;
	if (strEqualCaseInsensitive(name, "triggerSecondaryFall"))
		return engine->outputChannels.triggerSecondaryFall;
	if (strEqualCaseInsensitive(name, "triggerSecondaryRise"))
		return engine->outputChannels.triggerSecondaryRise;
	if (strEqualCaseInsensitive(name, "triggerVvtFall"))
		return engine->outputChannels.triggerVvtFall;
	if (strEqualCaseInsensitive(name, "triggerVvtRise"))
		return engine->outputChannels.triggerVvtRise;
	if (strEqualCaseInsensitive(name, "startStopStateToggleCounter"))
		return engine->outputChannels.startStopStateToggleCounter;
	if (strEqualCaseInsensitive(name, "starterState"))
		return engine->outputChannels.starterState;
	if (strEqualCaseInsensitive(name, "starterRelayDisable"))
		return engine->outputChannels.starterRelayDisable;
	if (strEqualCaseInsensitive(name, "multiSparkCounter"))
		return engine->outputChannels.multiSparkCounter;
	if (strEqualCaseInsensitive(name, "idleState"))
		return engine->outputChannels.idleState;
	if (strEqualCaseInsensitive(name, "injectorFlowPressureRatio"))
		return engine->outputChannels.injectorFlowPressureRatio;
	if (strEqualCaseInsensitive(name, "injectorFlowPressureDelta"))
		return engine->outputChannels.injectorFlowPressureDelta;
	if (strEqualCaseInsensitive(name, "etbFeedForward"))
		return engine->outputChannels.etbFeedForward;
	if (strEqualCaseInsensitive(name, "etbIntegralError"))
		return engine->outputChannels.etbIntegralError;
	if (strEqualCaseInsensitive(name, "etbCurrentTarget"))
		return engine->outputChannels.etbCurrentTarget;
	if (strEqualCaseInsensitive(name, "alternatorStatus.iTerm"))
		return engine->outputChannels.alternatorStatus.iTerm;
	if (strEqualCaseInsensitive(name, "alternatorStatus.dTerm"))
		return engine->outputChannels.alternatorStatus.dTerm;
	if (strEqualCaseInsensitive(name, "alternatorStatus.output"))
		return engine->outputChannels.alternatorStatus.output;
	if (strEqualCaseInsensitive(name, "alternatorStatus.error"))
		return engine->outputChannels.alternatorStatus.error;
	if (strEqualCaseInsensitive(name, "alternatorStatus.resetCounter"))
		return engine->outputChannels.alternatorStatus.resetCounter;
	if (strEqualCaseInsensitive(name, "idleStatus.iTerm"))
		return engine->outputChannels.idleStatus.iTerm;
	if (strEqualCaseInsensitive(name, "idleStatus.dTerm"))
		return engine->outputChannels.idleStatus.dTerm;
	if (strEqualCaseInsensitive(name, "idleStatus.output"))
		return engine->outputChannels.idleStatus.output;
	if (strEqualCaseInsensitive(name, "idleStatus.error"))
		return engine->outputChannels.idleStatus.error;
	if (strEqualCaseInsensitive(name, "idleStatus.resetCounter"))
		return engine->outputChannels.idleStatus.resetCounter;
	if (strEqualCaseInsensitive(name, "etbStatus.iTerm"))
		return engine->outputChannels.etbStatus.iTerm;
	if (strEqualCaseInsensitive(name, "etbStatus.dTerm"))
		return engine->outputChannels.etbStatus.dTerm;
	if (strEqualCaseInsensitive(name, "etbStatus.output"))
		return engine->outputChannels.etbStatus.output;
	if (strEqualCaseInsensitive(name, "etbStatus.error"))
		return engine->outputChannels.etbStatus.error;
	if (strEqualCaseInsensitive(name, "etbStatus.resetCounter"))
		return engine->outputChannels.etbStatus.resetCounter;
	return EFI_ERROR_CODE;
}
