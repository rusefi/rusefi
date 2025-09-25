package com.rusefi.maintenance.migration.migrators;

import com.devexperts.logging.Logging;
import com.opensr5.ini.field.IniField;
import com.opensr5.ini.field.EnumIniField;
import com.rusefi.maintenance.migration.TuneMigrationContext;
import com.rusefi.tune.xml.Constant;

import java.util.Optional;

import static com.devexperts.logging.Logging.getLogging;

public enum BooleanIniFieldMigrator implements TuneMigrator {
    INSTANCE;

    private static final Logging log = getLogging(BooleanIniFieldMigrator.class);

    public static final String LAUNCH_CONTROL_FIELD_NAME = "launchControlEnabled";

    private static final BooleanField[] fieldsToMigrate = {
        // List all the boolean fields to migrate here, you should also update the tests!!
        new BooleanField(LAUNCH_CONTROL_FIELD_NAME, "enabled", "disabled"),
        new BooleanField("isInjectionEnabled", "enabled", "disabled"),
        new BooleanField("isIgnitionEnabled", "enabled", "disabled"),
        new BooleanField("isCylinderCleanupEnabled", "enabled", "disabled"),
        new BooleanField("isMapAveragingEnabled", "enabled", "disabled"),
        new BooleanField("isFasterEngineSpinUpEnabled", "enabled", "disabled"),
        new BooleanField("enableSoftwareKnock", "enabled", "disabled"),
        new BooleanField("fuelClosedLoopCorrectionEnabled", "enabled", "disabled"),
        new BooleanField("isSdCardEnabled", "enabled", "disabled"),
        new BooleanField("isBoostControlEnabled", "enabled", "disabled"),
        new BooleanField("useSeparateAdvanceForIdle", "enabled", "disabled"),
        new BooleanField("useSeparateVeForIdle", "enabled", "disabled"),
        new BooleanField("useSeparateIdleTablesForCrankingTaper", "enabled", "disabled"),
        new BooleanField("useIacTableForCoasting", "enabled", "disabled"),
        new BooleanField("nitrousControlEnabled", "enabled", "disabled"),
        new BooleanField("stftIgnoreErrorMagnitude", "yes", "no"),
        new BooleanField("enableLaunchRetard", "yes", "no"),
        new BooleanField("useIdleAdvanceWhileCoasting", "yes", "no"),
        new BooleanField("launchFuelCutEnable", "yes", "no"),
        new BooleanField("launchSparkCutEnable", "yes", "no"),
        new BooleanField("useAdvanceCorrectionsForCranking", "yes", "no"),
        new BooleanField("useInjectorFlowLinearizationTable", "yes", "no"),
        new BooleanField("multisparkEnable", "yes", "no"),
        new BooleanField("consumeObdSensors", "yes", "no"),
        new BooleanField("enableCanVss", "yes", "no"),
        new BooleanField("alphaNUseIat", "yes", "no"),
        new BooleanField("useRawOutputToDriveIdleStepper", "yes", "no"),
        new BooleanField("alwaysWriteSdCard", "yes", "no"),
        new BooleanField("useLinearCltSensor", "yes", "no"),
        new BooleanField("useLinearIatSensor", "yes", "no"),
        new BooleanField("enableOilPressureProtect", "yes", "no"),
        new BooleanField("limitTorqueReductionTime", "yes", "no"),
        new BooleanField("isWaveAnalyzerEnabled", "yes", "no"),
        new BooleanField("twoWireBatchIgnition", "yes", "no"),
        new BooleanField("useFixedBaroCorrFromMap", "yes", "no"),
        new BooleanField("useIacPidMultTable", "yes", "no"),
        new BooleanField("isPhaseSyncRequiredForIgnition", "yes", "no"),
        new BooleanField("stepper_dc_use_two_wires", "yes", "no"),
        new BooleanField("useHardSkipInTraction", "yes", "no"),
        new BooleanField("useAuxSpeedForSlipRatio", "yes", "no"),
        new BooleanField("useVssAsSecondWheelSpeed", "yes", "no"),
        new BooleanField("enableAemXSeriesEgt", "yes", "no"),
        new BooleanField("enableKnockSpectrogramFilter", "yes", "no"),
        new BooleanField("stepperDcInvertedPins", "yes", "no"),
        new BooleanField("canOpenBLT", "yes", "no"),
        new BooleanField("can2OpenBLT", "yes", "no"),
        new BooleanField("boardUseCanTerminator", "yes", "no"),
        new BooleanField("can1ListenMode", "yes", "no"),
        new BooleanField("can2ListenMode", "yes", "no"),
        new BooleanField("enableRemap", "yes", "no"),
        new BooleanField("ltitEnabled", "yes", "no"),
        new BooleanField("verboseVVTDecoding", "enabled", "disabled"),
        new BooleanField("tcuEnabled", "enabled", "disabled"),
        new BooleanField("torqueReductionEnabled", "enabled", "disabled"),
        new BooleanField("launchSmoothRetard", "enabled", "disabled"),
        new BooleanField("useBiQuadOnAuxSpeedSensors", "enabled", "disabled"),
        new BooleanField("kLineDoHondaSend", "yes", "no"),
        new BooleanField("isForcedInduction", "yes", "no"),
        new BooleanField("useFordRedundantTps", "yes", "no"),
        new BooleanField("enableKline", "enabled", "disabled"),
        new BooleanField("overrideTriggerGaps", "yes", "no"),
        new BooleanField("enableFan1WithAc", "yes", "no"),
        new BooleanField("enableFan2WithAc", "yes", "no"),
        new BooleanField("enableTrailingSparks", "yes", "no"),
        new BooleanField("etb_use_two_wires", "yes", "no"),
        new BooleanField("isDoubleSolenoidIdle", "yes", "no"),
        new BooleanField("useEeprom", "yes", "no"),
        new BooleanField("useCicPidForIdle", "Cic", "Industrial"),
        new BooleanField("displayLogicLevelsInEngineSniffer", "yes", "no"),
        new BooleanField("usescriptTableForCanSniffingFiltering", "yes", "no"),
        new BooleanField("useFordRedundantPps", "yes", "no"),
        new BooleanField("cltSensorPulldown", "yes", "no"),
        new BooleanField("iatSensorPulldown", "yes", "no"),
        new BooleanField("allowIdenticalPps", "yes", "no"),
        new BooleanField("overrideVvtTriggerGaps", "yes", "no"),
        new BooleanField("useSpiImu", "yes", "no"),
        new BooleanField("enableStagedInjection", "enabled", "disabled"),
        new BooleanField("is_enabled_spi_1", "yes", "no"),
        new BooleanField("is_enabled_spi_2", "yes", "no"),
        new BooleanField("is_enabled_spi_3", "yes", "no"),
        new BooleanField("is_enabled_spi_4", "yes", "no"),
        new BooleanField("is_enabled_spi_5", "yes", "no"),
        new BooleanField("is_enabled_spi_6", "yes", "no"),
        new BooleanField("rethrowHardFault", "yes", "no"),
        new BooleanField("verboseQuad", "yes", "no"),
        new BooleanField("verboseTLE8888", "yes", "no"),
        new BooleanField("enableVerboseCanTx", "yes", "no"),
        new BooleanField("measureMapOnlyInOneCylinder", "yes", "no"),
        new BooleanField("stepperForceParkingEveryRestart", "yes", "no"),
        new BooleanField("useNoiselessTriggerDecoder", "yes", "no"),
        new BooleanField("useIdleTimingPidControl", "yes", "no"),
        new BooleanField("verboseKLine", "yes", "no"),
        new BooleanField("idleIncrementalPidCic", "yes", "no"),
        new BooleanField("enableAemXSeries", "yes", "no"),
        new BooleanField("modeledFlowIdle", "yes", "no"),
    };

    @Override
    public void migrateTune(final TuneMigrationContext context) {
        for (BooleanField field : fieldsToMigrate) {

            final Optional<IniField> prevBooleanIniField = context.getPrevIniFile().findIniField(field.getFieldName());
            final Optional<IniField> updatedBooleanIniField = context.getUpdatedIniFile().findIniField(field.getFieldName());

            if (!prevBooleanIniField.isPresent() || !updatedBooleanIniField.isPresent()) {
                continue;
            }

            final Constant prevBooleanConst = context.getPrevTune().getConstantsAsMap().get(field.getFieldName());
            final Constant updatedBooleanConst = context.getUpdatedTune().getConstantsAsMap().get(field.getFieldName());

            // used to check if the migration is possible/valid:
            final String prevBooleanValue = prevBooleanConst.getValue();
            final String updatedBooleanFalse = ((EnumIniField) updatedBooleanIniField.get()).getEnums().get(0);

            if ((null == prevBooleanConst) || (null == updatedBooleanConst)) {
                continue;
            }

            Optional<Boolean> shouldMigrate = field.shouldMigrateField(prevBooleanValue, updatedBooleanFalse);

            // check if migration of this value is possible/valid
            if (shouldMigrate.isPresent()) {
                if(shouldMigrate.get()) {
                    final Constant migratedBooleanConst = prevBooleanConst.cloneWithValue(
                        String.valueOf(field.migrateField(prevBooleanValue))
                    );
                    context.addMigration(field.getFieldName(), migratedBooleanConst);
                }
            } else {
                log.warn(String.format("Invalid boolean field value on oldTune `%s` neither true/false or `%s`/`%s` :(", field.getFieldName(), field.getTrueValue(), field.getFalseValue()));
            }

        }
    }
}
