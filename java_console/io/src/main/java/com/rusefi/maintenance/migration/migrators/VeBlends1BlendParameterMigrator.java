package com.rusefi.maintenance.migration.migrators;

import com.opensr5.ini.field.EnumIniField;
import com.opensr5.ini.field.IniField;
import com.rusefi.maintenance.migration.TuneMigrationContext;
import com.rusefi.tune.xml.Constant;

import java.util.Optional;

public enum VeBlends1BlendParameterMigrator implements TuneMigrator {
    INSTANCE;

    public static final String VE_BLENDS_1_BLEND_PARAMETER_FIELD_NAME = "veBlends1_blendParameter";

    public static final String ZERO_BLEND_PARAMETER_VALUE = "\"Zero\"";

    @Override
    public void migrateTune(final TuneMigrationContext context) {
        final Optional<IniField> prevVeBlends1BlendParameterIniField = context.getPrevIniFile().findIniField(
            VE_BLENDS_1_BLEND_PARAMETER_FIELD_NAME
        );
        final Optional<IniField> updatedVeBlends1BlendParameterIniField = context.getUpdatedIniFile().findIniField(
            VE_BLENDS_1_BLEND_PARAMETER_FIELD_NAME
        );
        if (!prevVeBlends1BlendParameterIniField.isPresent() || !updatedVeBlends1BlendParameterIniField.isPresent()) {
            return;
        }
        if (!(prevVeBlends1BlendParameterIniField.get() instanceof EnumIniField)
                || !(updatedVeBlends1BlendParameterIniField.get() instanceof EnumIniField)
        ) {
            return;
        }

        final Constant prevVeBlends1BlendParameterConst = context.getPrevTune().getConstantsAsMap().get(
            VE_BLENDS_1_BLEND_PARAMETER_FIELD_NAME
        );
        final Constant updatedVeBlends1BlendParameterConst = context.getUpdatedTune().getConstantsAsMap().get(
            VE_BLENDS_1_BLEND_PARAMETER_FIELD_NAME
        );
        if ((null == prevVeBlends1BlendParameterConst) || (null == updatedVeBlends1BlendParameterConst)) {
            return;
        }
        final String prevVeBlends1BlendParameterValue = prevVeBlends1BlendParameterConst.getValue();
        final String updatedVeBlends1BlendParameterValue = updatedVeBlends1BlendParameterConst.getValue();
        if (ZERO_BLEND_PARAMETER_VALUE.equals(prevVeBlends1BlendParameterValue) &&
            !ZERO_BLEND_PARAMETER_VALUE.equals(updatedVeBlends1BlendParameterValue)
        ) {
            // We are not going to restore the `veBlends1_blendParameter` field value to its previous `Zero` default
            // value
            context.addMigration(VE_BLENDS_1_BLEND_PARAMETER_FIELD_NAME, updatedVeBlends1BlendParameterConst);
        }
    }
}
