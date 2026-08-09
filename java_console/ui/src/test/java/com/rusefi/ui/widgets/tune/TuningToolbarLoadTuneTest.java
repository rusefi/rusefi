package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.ArrayIniField;
import com.rusefi.maintenance.TestTuneMigrationContext;
import com.rusefi.maintenance.migration.afr_msq_import_migration.AfrMsqImportMigrationContext;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;

class TuningToolbarLoadTuneTest {
    @Test
    void connectedLoadAfrLambdaTableEncoding() throws Exception {
        TestTuneMigrationContext context = AfrMsqImportMigrationContext.loadAfrToLambda();
        IniFileModel targetIni = context.getUpdatedIniFile();
        ConfigurationImage targetImage = context.getUpdatedTune().asImage(targetIni);

        ConfigurationImage loaded = TuningToolbarWidget.applyLoadedTune(
            context.getPrevTune(), context.getPrevIniFile(), targetImage, targetIni, context.getCallbacks());

        ArrayIniField field = (ArrayIniField) targetIni.findIniField("lambdaTable").orElseThrow();
        assertEquals(147, rawValue(loaded, field, 0, 0));
        assertEquals(140, rawValue(loaded, field, 8, 4));
        assertEquals(126, rawValue(loaded, field, 11, 13));
    }

    private static int rawValue(ConfigurationImage image, ArrayIniField field, int row, int column) {
        return Byte.toUnsignedInt(image.getContent()[field.getOffset(row, column)]);
    }
}
