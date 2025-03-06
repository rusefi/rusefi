package com.rusefi.maintenance;

import com.opensr5.ini.field.ArrayIniField;
import com.rusefi.config.FieldType;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import javax.xml.bind.JAXBException;

import static org.junit.jupiter.api.Assertions.assertEquals;

public class IniFieldsParsingTest {
    private CalibrationsTestData testData;

    @BeforeEach
    void setUp() throws JAXBException {
        testData = CalibrationsTestData.load();
    }

    @Test
    void testVeTableIniField() {
        final ArrayIniField veTableIniField = (ArrayIniField) testData.getPrevIni().getIniField("veTable");
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
        final ArrayIniField veTableIniField = (ArrayIniField) testData.getPrevIni().getIniField("veRpmBins");
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
        final ArrayIniField veTableIniField = (ArrayIniField) testData.getPrevIni().getIniField("veLoadBins");
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
