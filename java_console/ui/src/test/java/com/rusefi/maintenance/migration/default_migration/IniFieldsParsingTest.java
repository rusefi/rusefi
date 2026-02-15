package com.rusefi.maintenance.migration.default_migration;

import com.opensr5.ini.field.ArrayIniField;
import com.opensr5.ini.field.StringIniField;
import com.rusefi.config.FieldType;
import com.rusefi.maintenance.TestTuneMigrationContext;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import jakarta.xml.bind.JAXBException;

import java.io.FileNotFoundException;

import static org.junit.jupiter.api.Assertions.assertEquals;

public class IniFieldsParsingTest {
    private TestTuneMigrationContext testContext;

    @BeforeEach
    void setUp() throws JAXBException, FileNotFoundException {
        testContext = DefaultTestTuneMigrationContext.load();
    }

    @Test
    void testVehicleNameIniField() {
        final StringIniField veTableIniField = (StringIniField) testContext.getPrevIniFile().getIniField("vehicleName");
        assertEquals(1116, veTableIniField.getOffset());
    }

    @Test
    void testVeTableIniField() {
        final ArrayIniField veTableIniField = (ArrayIniField) testContext.getPrevIniFile().getIniField("veTable");
        assertEquals(FieldType.UINT16, veTableIniField.getType());
        assertEquals(16544, veTableIniField.getOffset());
        assertEquals(16, veTableIniField.getCols());
        assertEquals(16, veTableIniField.getRows());
        assertEquals(0.1, veTableIniField.getMultiplier());
        assertEquals("0", veTableIniField.getMin());
        assertEquals("999", veTableIniField.getMax());
        assertEquals("1", veTableIniField.getDigits());
    }

    @Test
    void testVeRpmBinsIniField() {
        final ArrayIniField veTableIniField = (ArrayIniField) testContext.getPrevIniFile().getIniField("veRpmBins");
        assertEquals(FieldType.UINT16, veTableIniField.getType());
        assertEquals(17088, veTableIniField.getOffset());
        assertEquals(1, veTableIniField.getCols());
        assertEquals(16, veTableIniField.getRows());
        assertEquals(1, veTableIniField.getMultiplier());
        assertEquals("0", veTableIniField.getMin());
        assertEquals("18000", veTableIniField.getMax());
        assertEquals("0", veTableIniField.getDigits());
    }

    @Test
    void testVeLoadBinsIniField() {
        final ArrayIniField veTableIniField = (ArrayIniField) testContext.getPrevIniFile().getIniField("veLoadBins");
        assertEquals(FieldType.UINT16, veTableIniField.getType());
        assertEquals(17056, veTableIniField.getOffset());
        assertEquals(1, veTableIniField.getCols());
        assertEquals(16, veTableIniField.getRows());
        assertEquals(1, veTableIniField.getMultiplier());
        assertEquals("0", veTableIniField.getMin());
        assertEquals("1000", veTableIniField.getMax());
        assertEquals("0", veTableIniField.getDigits());
    }
}

