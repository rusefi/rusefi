package com.rusefi.maintenance.migration;

import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.ArrayIniField;
import com.opensr5.ini.field.IniField;
import com.rusefi.maintenance.TestCallbacks;
import com.rusefi.maintenance.TestTuneMigrationContext;
import com.rusefi.maintenance.migration.migrators.ComposedTuneMigrator;
import com.rusefi.tune.xml.Constant;
import com.rusefi.tune.xml.Msq;
import org.junit.jupiter.api.Test;

import java.util.Map;
import java.util.Optional;

import static com.rusefi.config.FieldType.UINT16;
import static com.rusefi.maintenance.migration.migrators.BoostLoadBinsMigrator.*;
import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;

public class BoostLoadBinsMigratorTest {
    private static final IniField TEST_BOOST_LOAD_BINS_INI_FIELD = new ArrayIniField(
        BOOST_LOAD_BINS_FIELD_NAME,
        5340,
        UINT16,
        1,
        8,
        "",
        1,
        "0",
        "1000",
        "0"
    );
    private static final Constant TEST_BOOST_LOAD_BINS_VALUE = new Constant(
        BOOST_LOAD_BINS_FIELD_NAME,
        null,
        "\n" +
            "         0.0 \n" +
            "         1.0 \n" +
            "         2.0 \n" +
            "         3.0 \n" +
            "         4.0 \n" +
            "         5.0 \n" +
            "         6.0 \n" +
            "         7.0 \n",
        "0"
    );


    private static final IniField TEST_BOOST_OPEN_LOOP_LOAD_BINS_INI_FIELD = new ArrayIniField(
        BOOST_OPEN_LOOP_LOAD_BINS_FIELD_NAME,
        5248,
        UINT16,
        1,
        8,
        "",
        1,
        "0",
        "650",
        "0"
    );
    private static final Constant TEST_BOOST_OPEN_LOOP_LOAD_BINS_VALUE = new Constant(
        BOOST_OPEN_LOOP_LOAD_BINS_FIELD_NAME,
        null,
        "\n" +
            "         0.0 \n" +
            "         14.0 \n" +
            "         29.0 \n" +
            "         43.0 \n" +
            "         57.0 \n" +
            "         71.0 \n" +
            "         86.0 \n" +
            "         100.0 \n",
        "0"
    );

    private static final IniField TEST_BOOST_CLOSED_LOOP_LOAD_BINS_INI_FIELD = new ArrayIniField(
        BOOST_CLOSED_LOOP_LOAD_BINS_FIELD_NAME,
        5328,
        UINT16,
        1,
        8,
        "",
        1,
        "0",
        "650",
        "0"
    );
    private static final Constant TEST_BOOST_CLOSED_LOOP_LOAD_BINS_VALUE = new Constant(
        BOOST_CLOSED_LOOP_LOAD_BINS_FIELD_NAME,
        null,
        "\n" +
            "         0.0 \n" +
            "         14.0 \n" +
            "         29.0 \n" +
            "         43.0 \n" +
            "         57.0 \n" +
            "         71.0 \n" +
            "         86.0 \n" +
            "         100.0 \n",
        "0"
    );

    @Test
    void checkDisappearedBoostLoadBinsMigration() {
        final IniFileModel prevIniFile = mock(IniFileModel.class);
        when(prevIniFile.findIniField(TEST_BOOST_LOAD_BINS_INI_FIELD.getName())).thenReturn(Optional.of(
            TEST_BOOST_LOAD_BINS_INI_FIELD)
        );
        when(prevIniFile.findIniField(TEST_BOOST_OPEN_LOOP_LOAD_BINS_INI_FIELD.getName())).thenReturn(
            Optional.empty()
        );
        when(prevIniFile.findIniField(TEST_BOOST_CLOSED_LOOP_LOAD_BINS_INI_FIELD.getName())).thenReturn(
            Optional.empty()
        );
        when(prevIniFile.getAllIniFields()).thenReturn(Map.of(
            TEST_BOOST_LOAD_BINS_INI_FIELD.getName(),
            TEST_BOOST_LOAD_BINS_INI_FIELD
        ));

        final Msq prevMsq = mock(Msq.class);
        when(prevMsq.getConstantsAsMap()).thenReturn(Map.of(
            TEST_BOOST_LOAD_BINS_VALUE.getName(),
            TEST_BOOST_LOAD_BINS_VALUE
        ));

        final IniFileModel updatedIniFile = mock(IniFileModel.class);
        when(updatedIniFile.findIniField(TEST_BOOST_LOAD_BINS_VALUE.getName())).thenReturn(Optional.empty());
        when(updatedIniFile.findIniField(TEST_BOOST_OPEN_LOOP_LOAD_BINS_INI_FIELD.getName())).thenReturn(Optional.of(
            TEST_BOOST_OPEN_LOOP_LOAD_BINS_INI_FIELD
        ));
        when(updatedIniFile.findIniField(TEST_BOOST_CLOSED_LOOP_LOAD_BINS_INI_FIELD.getName())).thenReturn(Optional.of(
            TEST_BOOST_CLOSED_LOOP_LOAD_BINS_INI_FIELD
        ));
        when(updatedIniFile.getAllIniFields()).thenReturn(Map.of(
            TEST_BOOST_OPEN_LOOP_LOAD_BINS_INI_FIELD.getName(), TEST_BOOST_OPEN_LOOP_LOAD_BINS_INI_FIELD,
            TEST_BOOST_CLOSED_LOOP_LOAD_BINS_INI_FIELD.getName(), TEST_BOOST_CLOSED_LOOP_LOAD_BINS_INI_FIELD
        ));

        final Msq updatedMsq = mock(Msq.class);
        when(updatedMsq.getConstantsAsMap()).thenReturn(Map.of(
            TEST_BOOST_OPEN_LOOP_LOAD_BINS_VALUE.getName(), TEST_BOOST_OPEN_LOOP_LOAD_BINS_VALUE,
            TEST_BOOST_CLOSED_LOOP_LOAD_BINS_VALUE.getName(), TEST_BOOST_CLOSED_LOOP_LOAD_BINS_VALUE
        ));

        final TestTuneMigrationContext testContext = new TestTuneMigrationContext(
            prevMsq,
            prevIniFile,
            updatedMsq,
            updatedIniFile,
            new TestCallbacks()
        );

        ComposedTuneMigrator.INSTANCE.migrateTune(testContext);

        assertTrue(testContext.getMigratedConstants().containsKey(BOOST_LOAD_BINS_FIELD_NAME));
        assertNull(testContext.getMigratedConstants().get(BOOST_LOAD_BINS_FIELD_NAME));
        assertFalse(testContext.getTestCallbacks().getContent().contains(BOOST_LOAD_BINS_FIELD_NAME));

        assertEquals(
            TEST_BOOST_LOAD_BINS_VALUE,
            testContext.getMigratedConstants().get(BOOST_OPEN_LOOP_LOAD_BINS_FIELD_NAME)
        );
        assertFalse(testContext.getTestCallbacks().getContent().contains(BOOST_OPEN_LOOP_LOAD_BINS_FIELD_NAME));

        assertEquals(
            TEST_BOOST_LOAD_BINS_VALUE,
            testContext.getMigratedConstants().get(BOOST_CLOSED_LOOP_LOAD_BINS_FIELD_NAME)
        );
        assertFalse(testContext.getTestCallbacks().getContent().contains(BOOST_CLOSED_LOOP_LOAD_BINS_FIELD_NAME));
    }
}
