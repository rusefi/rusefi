package com.rusefi.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageMeta;
import com.opensr5.ConfigurationImageMetaVersion0_0;
import com.opensr5.ConfigurationImageWithMeta;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.IniField;
import com.opensr5.ini.field.ScalarIniField;
import com.opensr5.io.ConfigurationImageFile;
import com.rusefi.binaryprotocol.MsqFactory;
import com.rusefi.tools.tune.CurveData;
import com.rusefi.tools.tune.TS2C;
import com.rusefi.tools.tune.TuneCanToolConstants;
import com.rusefi.tune.xml.Constant;
import com.rusefi.tune.xml.Msq;
import org.junit.jupiter.api.Test;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;

import static org.junit.jupiter.api.Assertions.*;

public class TuneReadWriteTest {
    private static final String PATH = TuneCanToolConstants.SRC_TEST_RESOURCES + "frankenso/";
    static final String TUNE_NAME = PATH + "test-CurrentTune.msq";
    static final String TEST_INI = PATH + "test-mainController.ini";
    private static final String TEST_BINARY_FILE = PATH + "current_configuration.binary_image";
    private static final int LEGACY_TOTAL_CONFIG_SIZE = 20000;

    private final IniFileModel model = new IniFileModel().readIniFile(TEST_INI);

    @Test
    public void testIniReader() throws IOException {
        assertTrue(model.getTables().contains("fueltable"));
        assertEquals(21, model.getTables().size());
        assertEquals("fuelRpmBins", model.getXBin("FUELTable"));
        assertEquals("fuelLoadBins", model.getYBin("fuelTable"));

        String tableName = "ignitionIatCorrTable";
        String xRpmBinsName = model.getXBin(tableName);
        String yLoadBinsName = model.getYBin(tableName);
        assertEquals("ignitionIatCorrRpmBins", xRpmBinsName);
        assertEquals("ignitionIatCorrLoadBins", yLoadBinsName);

        CurveData xRpmCurve = CurveData.valueOf(TUNE_NAME, xRpmBinsName, model);

        assertEquals("static const float hardCodedignitionIatCorrRpmBins[16] = " +
                "{880.0, 1260.0, 1640.0, 2020.0, 2400.0, 2780.0, 3000.0, 3380.0, 3760.0, 4140.0, 4520.0, 5000.0, 5700.0, 6500.0, 7200.0, 8000.0};\n", xRpmCurve.getCsourceCode());

        assertEquals("static void prefixcannedignitionIatCorrRpmBins() {\n" +
                "\tstatic const float hardCodedignitionIatCorrRpmBins[16] = {880.0, 1260.0, 1640.0, 2020.0, 2400.0, 2780.0, 3000.0, 3380.0, 3760.0, 4140.0, 4520.0, 5000.0, 5700.0, 6500.0, 7200.0, 8000.0};\n" +
                "\tcopyArray(config->ignitionIatCorrRpmBins, hardCodedignitionIatCorrRpmBins);\n" +
                "}\n" +
                "\n", xRpmCurve.getCsourceMethod("config->", "prefix", xRpmCurve.getName()));

        TS2C.FINGER_PRINT = "/*unittest*/\n";
        String tableSource = TS2C.getTableCSourceCode2(TUNE_NAME, tableName, model);
        assertEquals("static const float hardCodedignitionIatCorrTable[16][16] = {\n" +
                "/*unittest*/\n" +
                "{/* 0 -40.000\t*//* 0 880.0*/4.000,\t/* 1 1260.0*/4.000,\t/* 2 1640.0*/4.000,\t/* 3 2020.0*/4.000,\t/* 4 2400.0*/4.000,\t/* 5 2780.0*/4.000,\t/* 6 3000.0*/4.000,\t/* 7 3380.0*/4.000,\t/* 8 3760.0*/4.000,\t/* 9 4140.0*/4.000,\t/* 10 4520.0*/4.000,\t/* 11 5000.0*/2.000,\t/* 12 5700.0*/2.000,\t/* 13 6500.0*/2.000,\t/* 14 7200.0*/2.000,\t/* 15 8000.0*/2.000,\t},\n" +
                "{/* 1 -30.000\t*//* 0 880.0*/4.000,\t/* 1 1260.0*/4.000,\t/* 2 1640.0*/4.000,\t/* 3 2020.0*/4.000,\t/* 4 2400.0*/4.000,\t/* 5 2780.0*/4.000,\t/* 6 3000.0*/4.000,\t/* 7 3380.0*/4.000,\t/* 8 3760.0*/4.000,\t/* 9 4140.0*/4.000,\t/* 10 4520.0*/4.000,\t/* 11 5000.0*/2.000,\t/* 12 5700.0*/2.000,\t/* 13 6500.0*/2.000,\t/* 14 7200.0*/2.000,\t/* 15 8000.0*/2.000,\t},\n" +
                "{/* 2 -20.000\t*//* 0 880.0*/4.000,\t/* 1 1260.0*/4.000,\t/* 2 1640.0*/4.000,\t/* 3 2020.0*/4.000,\t/* 4 2400.0*/4.000,\t/* 5 2780.0*/4.000,\t/* 6 3000.0*/4.000,\t/* 7 3380.0*/4.000,\t/* 8 3760.0*/4.000,\t/* 9 4140.0*/4.000,\t/* 10 4520.0*/4.000,\t/* 11 5000.0*/2.000,\t/* 12 5700.0*/2.000,\t/* 13 6500.0*/2.000,\t/* 14 7200.0*/2.000,\t/* 15 8000.0*/2.000,\t},\n" +
                "{/* 3 -10.000\t*//* 0 880.0*/4.000,\t/* 1 1260.0*/4.000,\t/* 2 1640.0*/4.000,\t/* 3 2020.0*/4.000,\t/* 4 2400.0*/4.000,\t/* 5 2780.0*/4.000,\t/* 6 3000.0*/4.000,\t/* 7 3380.0*/4.000,\t/* 8 3760.0*/4.000,\t/* 9 4140.0*/4.000,\t/* 10 4520.0*/4.000,\t/* 11 5000.0*/2.000,\t/* 12 5700.0*/2.000,\t/* 13 6500.0*/2.000,\t/* 14 7200.0*/2.000,\t/* 15 8000.0*/2.000,\t},\n" +
                "{/* 4 0.000\t*//* 0 880.0*/3.500,\t/* 1 1260.0*/3.500,\t/* 2 1640.0*/3.500,\t/* 3 2020.0*/3.500,\t/* 4 2400.0*/3.500,\t/* 5 2780.0*/3.500,\t/* 6 3000.0*/3.500,\t/* 7 3380.0*/3.500,\t/* 8 3760.0*/3.500,\t/* 9 4140.0*/3.500,\t/* 10 4520.0*/3.500,\t/* 11 5000.0*/2.000,\t/* 12 5700.0*/2.000,\t/* 13 6500.0*/2.000,\t/* 14 7200.0*/2.000,\t/* 15 8000.0*/2.000,\t},\n" +
                "{/* 5 10.000\t*//* 0 880.0*/3.000,\t/* 1 1260.0*/3.000,\t/* 2 1640.0*/3.000,\t/* 3 2020.0*/3.000,\t/* 4 2400.0*/3.000,\t/* 5 2780.0*/3.000,\t/* 6 3000.0*/3.000,\t/* 7 3380.0*/3.000,\t/* 8 3760.0*/3.000,\t/* 9 4140.0*/3.000,\t/* 10 4520.0*/3.000,\t/* 11 5000.0*/2.000,\t/* 12 5700.0*/2.000,\t/* 13 6500.0*/2.000,\t/* 14 7200.0*/2.000,\t/* 15 8000.0*/2.000,\t},\n" +
                "{/* 6 20.000\t*//* 0 880.0*/2.000,\t/* 1 1260.0*/2.000,\t/* 2 1640.0*/2.000,\t/* 3 2020.0*/2.000,\t/* 4 2400.0*/2.000,\t/* 5 2780.0*/2.000,\t/* 6 3000.0*/2.000,\t/* 7 3380.0*/2.000,\t/* 8 3760.0*/2.000,\t/* 9 4140.0*/2.000,\t/* 10 4520.0*/2.000,\t/* 11 5000.0*/0.000,\t/* 12 5700.0*/0.000,\t/* 13 6500.0*/0.000,\t/* 14 7200.0*/0.000,\t/* 15 8000.0*/0.000,\t},\n" +
                "{/* 7 30.000\t*//* 0 880.0*/0.000,\t/* 1 1260.0*/0.000,\t/* 2 1640.0*/0.000,\t/* 3 2020.0*/0.000,\t/* 4 2400.0*/0.000,\t/* 5 2780.0*/0.000,\t/* 6 3000.0*/0.000,\t/* 7 3380.0*/0.000,\t/* 8 3760.0*/0.000,\t/* 9 4140.0*/0.000,\t/* 10 4520.0*/0.000,\t/* 11 5000.0*/0.000,\t/* 12 5700.0*/0.000,\t/* 13 6500.0*/0.000,\t/* 14 7200.0*/0.000,\t/* 15 8000.0*/0.000,\t},\n" +
                "{/* 8 40.000\t*//* 0 880.0*/0.000,\t/* 1 1260.0*/0.000,\t/* 2 1640.0*/0.000,\t/* 3 2020.0*/0.000,\t/* 4 2400.0*/0.000,\t/* 5 2780.0*/0.000,\t/* 6 3000.0*/0.000,\t/* 7 3380.0*/0.000,\t/* 8 3760.0*/0.000,\t/* 9 4140.0*/0.000,\t/* 10 4520.0*/0.000,\t/* 11 5000.0*/0.000,\t/* 12 5700.0*/0.000,\t/* 13 6500.0*/0.000,\t/* 14 7200.0*/0.000,\t/* 15 8000.0*/0.000,\t},\n" +
                "{/* 9 50.000\t*//* 0 880.0*/0.000,\t/* 1 1260.0*/0.000,\t/* 2 1640.0*/-0.900,\t/* 3 2020.0*/-0.900,\t/* 4 2400.0*/-0.900,\t/* 5 2780.0*/-0.900,\t/* 6 3000.0*/-0.900,\t/* 7 3380.0*/-0.900,\t/* 8 3760.0*/-0.900,\t/* 9 4140.0*/-0.900,\t/* 10 4520.0*/-0.900,\t/* 11 5000.0*/-0.900,\t/* 12 5700.0*/-0.900,\t/* 13 6500.0*/-0.900,\t/* 14 7200.0*/-0.900,\t/* 15 8000.0*/-0.900,\t},\n" +
                "{/* 10 60.000\t*//* 0 880.0*/-3.300,\t/* 1 1260.0*/-3.400,\t/* 2 1640.0*/-4.900,\t/* 3 2020.0*/-4.900,\t/* 4 2400.0*/-4.900,\t/* 5 2780.0*/-4.900,\t/* 6 3000.0*/-4.400,\t/* 7 3380.0*/-4.400,\t/* 8 3760.0*/-4.400,\t/* 9 4140.0*/-4.400,\t/* 10 4520.0*/-4.400,\t/* 11 5000.0*/-0.900,\t/* 12 5700.0*/-0.900,\t/* 13 6500.0*/-0.900,\t/* 14 7200.0*/-0.900,\t/* 15 8000.0*/-0.900,\t},\n" +
                "{/* 11 70.000\t*//* 0 880.0*/-4.400,\t/* 1 1260.0*/-4.900,\t/* 2 1640.0*/-5.900,\t/* 3 2020.0*/-5.900,\t/* 4 2400.0*/-5.900,\t/* 5 2780.0*/-5.900,\t/* 6 3000.0*/-4.900,\t/* 7 3380.0*/-4.900,\t/* 8 3760.0*/-4.900,\t/* 9 4140.0*/-4.900,\t/* 10 4520.0*/-4.900,\t/* 11 5000.0*/-2.400,\t/* 12 5700.0*/-2.400,\t/* 13 6500.0*/-2.400,\t/* 14 7200.0*/-2.400,\t/* 15 8000.0*/-2.400,\t},\n" +
                "{/* 12 80.000\t*//* 0 880.0*/-4.400,\t/* 1 1260.0*/-4.900,\t/* 2 1640.0*/-5.900,\t/* 3 2020.0*/-5.900,\t/* 4 2400.0*/-5.900,\t/* 5 2780.0*/-5.900,\t/* 6 3000.0*/-4.900,\t/* 7 3380.0*/-4.900,\t/* 8 3760.0*/-4.900,\t/* 9 4140.0*/-4.900,\t/* 10 4520.0*/-4.900,\t/* 11 5000.0*/-2.900,\t/* 12 5700.0*/-2.900,\t/* 13 6500.0*/-2.900,\t/* 14 7200.0*/-2.900,\t/* 15 8000.0*/-2.900,\t},\n" +
                "{/* 13 90.000\t*//* 0 880.0*/-4.400,\t/* 1 1260.0*/-4.900,\t/* 2 1640.0*/-5.900,\t/* 3 2020.0*/-5.900,\t/* 4 2400.0*/-5.900,\t/* 5 2780.0*/-5.900,\t/* 6 3000.0*/-4.900,\t/* 7 3380.0*/-4.900,\t/* 8 3760.0*/-4.900,\t/* 9 4140.0*/-4.900,\t/* 10 4520.0*/-4.900,\t/* 11 5000.0*/-3.900,\t/* 12 5700.0*/-3.900,\t/* 13 6500.0*/-3.900,\t/* 14 7200.0*/-3.900,\t/* 15 8000.0*/-3.900,\t},\n" +
                "{/* 14 100.000\t*//* 0 880.0*/-4.400,\t/* 1 1260.0*/-4.900,\t/* 2 1640.0*/-5.900,\t/* 3 2020.0*/-5.900,\t/* 4 2400.0*/-5.900,\t/* 5 2780.0*/-5.900,\t/* 6 3000.0*/-4.900,\t/* 7 3380.0*/-4.900,\t/* 8 3760.0*/-4.900,\t/* 9 4140.0*/-4.900,\t/* 10 4520.0*/-4.900,\t/* 11 5000.0*/-3.900,\t/* 12 5700.0*/-3.900,\t/* 13 6500.0*/-3.900,\t/* 14 7200.0*/-3.900,\t/* 15 8000.0*/-3.900,\t},\n" +
                "{/* 15 110.000\t*//* 0 880.0*/-4.400,\t/* 1 1260.0*/-4.900,\t/* 2 1640.0*/-5.900,\t/* 3 2020.0*/-5.900,\t/* 4 2400.0*/-5.900,\t/* 5 2780.0*/-5.900,\t/* 6 3000.0*/-4.900,\t/* 7 3380.0*/-4.900,\t/* 8 3760.0*/-4.900,\t/* 9 4140.0*/-4.900,\t/* 10 4520.0*/-4.900,\t/* 11 5000.0*/-3.900,\t/* 12 5700.0*/-3.900,\t/* 13 6500.0*/-3.900,\t/* 14 7200.0*/-3.900,\t/* 15 8000.0*/-3.900,\t},\n" +
                "};\n" +
                "\n", tableSource);
    }

    @Test
    public void testALotTogether() throws IOException {
        String expected = "static void prefixcannedveTable() {\n" +
                "\tstatic const float hardCodedveRpmBins[16] = {650.0, 800.0, 1100.0, 1400.0, 1700.0, 2000.0, 2300.0, 2600.0, 2900.0, 3200.0, 3500.0, 3800.0, 4100.0, 4400.0, 4700.0, 7000.0};\n" +
                "\tstatic const float hardCodedveLoadBins[16] = {10.0, 20.0, 30.0, 40.0, 50.0, 60.0, 70.0, 80.0, 90.0, 100.0, 110.0, 120.0, 130.0, 140.0, 150.0, 160.0};\n" +
                "static const float hardCodedveTable[16][16] = {\n" +
                "/*unittest*/\n" +
                "{/* 0 10.000\t*//* 0 650.0*/80.000,\t/* 1 800.0*/80.000,\t/* 2 1100.0*/80.000,\t/* 3 1400.0*/80.000,\t/* 4 1700.0*/80.000,\t/* 5 2000.0*/80.000,\t/* 6 2300.0*/80.000,\t/* 7 2600.0*/80.000,\t/* 8 2900.0*/80.000,\t/* 9 3200.0*/80.000,\t/* 10 3500.0*/80.000,\t/* 11 3800.0*/80.000,\t/* 12 4100.0*/80.000,\t/* 13 4400.0*/80.000,\t/* 14 4700.0*/80.000,\t/* 15 7000.0*/80.000,\t},\n" +
                "{/* 1 20.000\t*//* 0 650.0*/80.000,\t/* 1 800.0*/80.000,\t/* 2 1100.0*/80.000,\t/* 3 1400.0*/80.000,\t/* 4 1700.0*/80.000,\t/* 5 2000.0*/80.000,\t/* 6 2300.0*/80.000,\t/* 7 2600.0*/80.000,\t/* 8 2900.0*/80.000,\t/* 9 3200.0*/80.000,\t/* 10 3500.0*/80.000,\t/* 11 3800.0*/80.000,\t/* 12 4100.0*/80.000,\t/* 13 4400.0*/80.000,\t/* 14 4700.0*/80.000,\t/* 15 7000.0*/80.000,\t},\n" +
                "{/* 2 30.000\t*//* 0 650.0*/80.000,\t/* 1 800.0*/80.000,\t/* 2 1100.0*/80.000,\t/* 3 1400.0*/80.000,\t/* 4 1700.0*/80.000,\t/* 5 2000.0*/80.000,\t/* 6 2300.0*/80.000,\t/* 7 2600.0*/80.000,\t/* 8 2900.0*/80.000,\t/* 9 3200.0*/80.000,\t/* 10 3500.0*/80.000,\t/* 11 3800.0*/80.000,\t/* 12 4100.0*/80.000,\t/* 13 4400.0*/80.000,\t/* 14 4700.0*/80.000,\t/* 15 7000.0*/80.000,\t},\n" +
                "{/* 3 40.000\t*//* 0 650.0*/80.000,\t/* 1 800.0*/80.000,\t/* 2 1100.0*/80.000,\t/* 3 1400.0*/80.000,\t/* 4 1700.0*/80.000,\t/* 5 2000.0*/80.000,\t/* 6 2300.0*/80.000,\t/* 7 2600.0*/80.000,\t/* 8 2900.0*/80.000,\t/* 9 3200.0*/80.000,\t/* 10 3500.0*/80.000,\t/* 11 3800.0*/80.000,\t/* 12 4100.0*/80.000,\t/* 13 4400.0*/80.000,\t/* 14 4700.0*/80.000,\t/* 15 7000.0*/80.000,\t},\n" +
                "{/* 4 50.000\t*//* 0 650.0*/80.000,\t/* 1 800.0*/80.000,\t/* 2 1100.0*/80.000,\t/* 3 1400.0*/80.000,\t/* 4 1700.0*/80.000,\t/* 5 2000.0*/80.000,\t/* 6 2300.0*/80.000,\t/* 7 2600.0*/80.000,\t/* 8 2900.0*/80.000,\t/* 9 3200.0*/80.000,\t/* 10 3500.0*/80.000,\t/* 11 3800.0*/80.000,\t/* 12 4100.0*/80.000,\t/* 13 4400.0*/80.000,\t/* 14 4700.0*/80.000,\t/* 15 7000.0*/80.000,\t},\n" +
                "{/* 5 60.000\t*//* 0 650.0*/80.000,\t/* 1 800.0*/80.000,\t/* 2 1100.0*/80.000,\t/* 3 1400.0*/80.000,\t/* 4 1700.0*/80.000,\t/* 5 2000.0*/80.000,\t/* 6 2300.0*/80.000,\t/* 7 2600.0*/80.000,\t/* 8 2900.0*/80.000,\t/* 9 3200.0*/80.000,\t/* 10 3500.0*/80.000,\t/* 11 3800.0*/80.000,\t/* 12 4100.0*/80.000,\t/* 13 4400.0*/80.000,\t/* 14 4700.0*/80.000,\t/* 15 7000.0*/80.000,\t},\n" +
                "{/* 6 70.000\t*//* 0 650.0*/80.000,\t/* 1 800.0*/80.000,\t/* 2 1100.0*/80.000,\t/* 3 1400.0*/80.000,\t/* 4 1700.0*/80.000,\t/* 5 2000.0*/80.000,\t/* 6 2300.0*/80.000,\t/* 7 2600.0*/80.000,\t/* 8 2900.0*/80.000,\t/* 9 3200.0*/80.000,\t/* 10 3500.0*/80.000,\t/* 11 3800.0*/80.000,\t/* 12 4100.0*/80.000,\t/* 13 4400.0*/80.000,\t/* 14 4700.0*/80.000,\t/* 15 7000.0*/80.000,\t},\n" +
                "{/* 7 80.000\t*//* 0 650.0*/80.000,\t/* 1 800.0*/80.000,\t/* 2 1100.0*/80.000,\t/* 3 1400.0*/80.000,\t/* 4 1700.0*/80.000,\t/* 5 2000.0*/80.000,\t/* 6 2300.0*/80.000,\t/* 7 2600.0*/80.000,\t/* 8 2900.0*/80.000,\t/* 9 3200.0*/80.000,\t/* 10 3500.0*/80.000,\t/* 11 3800.0*/80.000,\t/* 12 4100.0*/80.000,\t/* 13 4400.0*/80.000,\t/* 14 4700.0*/80.000,\t/* 15 7000.0*/80.000,\t},\n" +
                "{/* 8 90.000\t*//* 0 650.0*/80.000,\t/* 1 800.0*/80.000,\t/* 2 1100.0*/80.000,\t/* 3 1400.0*/80.000,\t/* 4 1700.0*/80.000,\t/* 5 2000.0*/80.000,\t/* 6 2300.0*/80.000,\t/* 7 2600.0*/80.000,\t/* 8 2900.0*/80.000,\t/* 9 3200.0*/80.000,\t/* 10 3500.0*/80.000,\t/* 11 3800.0*/80.000,\t/* 12 4100.0*/80.000,\t/* 13 4400.0*/80.000,\t/* 14 4700.0*/80.000,\t/* 15 7000.0*/80.000,\t},\n" +
                "{/* 9 100.000\t*//* 0 650.0*/80.000,\t/* 1 800.0*/80.000,\t/* 2 1100.0*/80.000,\t/* 3 1400.0*/80.000,\t/* 4 1700.0*/80.000,\t/* 5 2000.0*/80.000,\t/* 6 2300.0*/80.000,\t/* 7 2600.0*/80.000,\t/* 8 2900.0*/80.000,\t/* 9 3200.0*/80.000,\t/* 10 3500.0*/80.000,\t/* 11 3800.0*/80.000,\t/* 12 4100.0*/80.000,\t/* 13 4400.0*/80.000,\t/* 14 4700.0*/80.000,\t/* 15 7000.0*/80.000,\t},\n" +
                "{/* 10 110.000\t*//* 0 650.0*/80.000,\t/* 1 800.0*/80.000,\t/* 2 1100.0*/80.000,\t/* 3 1400.0*/80.000,\t/* 4 1700.0*/80.000,\t/* 5 2000.0*/80.000,\t/* 6 2300.0*/80.000,\t/* 7 2600.0*/80.000,\t/* 8 2900.0*/80.000,\t/* 9 3200.0*/80.000,\t/* 10 3500.0*/80.000,\t/* 11 3800.0*/80.000,\t/* 12 4100.0*/80.000,\t/* 13 4400.0*/80.000,\t/* 14 4700.0*/80.000,\t/* 15 7000.0*/80.000,\t},\n" +
                "{/* 11 120.000\t*//* 0 650.0*/80.000,\t/* 1 800.0*/80.000,\t/* 2 1100.0*/80.000,\t/* 3 1400.0*/80.000,\t/* 4 1700.0*/80.000,\t/* 5 2000.0*/80.000,\t/* 6 2300.0*/80.000,\t/* 7 2600.0*/80.000,\t/* 8 2900.0*/80.000,\t/* 9 3200.0*/80.000,\t/* 10 3500.0*/80.000,\t/* 11 3800.0*/80.000,\t/* 12 4100.0*/80.000,\t/* 13 4400.0*/80.000,\t/* 14 4700.0*/80.000,\t/* 15 7000.0*/80.000,\t},\n" +
                "{/* 12 130.000\t*//* 0 650.0*/80.000,\t/* 1 800.0*/80.000,\t/* 2 1100.0*/80.000,\t/* 3 1400.0*/80.000,\t/* 4 1700.0*/80.000,\t/* 5 2000.0*/80.000,\t/* 6 2300.0*/80.000,\t/* 7 2600.0*/80.000,\t/* 8 2900.0*/80.000,\t/* 9 3200.0*/80.000,\t/* 10 3500.0*/80.000,\t/* 11 3800.0*/80.000,\t/* 12 4100.0*/80.000,\t/* 13 4400.0*/80.000,\t/* 14 4700.0*/80.000,\t/* 15 7000.0*/80.000,\t},\n" +
                "{/* 13 140.000\t*//* 0 650.0*/80.000,\t/* 1 800.0*/80.000,\t/* 2 1100.0*/80.000,\t/* 3 1400.0*/80.000,\t/* 4 1700.0*/80.000,\t/* 5 2000.0*/80.000,\t/* 6 2300.0*/80.000,\t/* 7 2600.0*/80.000,\t/* 8 2900.0*/80.000,\t/* 9 3200.0*/80.000,\t/* 10 3500.0*/80.000,\t/* 11 3800.0*/80.000,\t/* 12 4100.0*/80.000,\t/* 13 4400.0*/80.000,\t/* 14 4700.0*/80.000,\t/* 15 7000.0*/80.000,\t},\n" +
                "{/* 14 150.000\t*//* 0 650.0*/80.000,\t/* 1 800.0*/80.000,\t/* 2 1100.0*/80.000,\t/* 3 1400.0*/80.000,\t/* 4 1700.0*/80.000,\t/* 5 2000.0*/80.000,\t/* 6 2300.0*/80.000,\t/* 7 2600.0*/80.000,\t/* 8 2900.0*/80.000,\t/* 9 3200.0*/80.000,\t/* 10 3500.0*/80.000,\t/* 11 3800.0*/80.000,\t/* 12 4100.0*/80.000,\t/* 13 4400.0*/80.000,\t/* 14 4700.0*/80.000,\t/* 15 7000.0*/80.000,\t},\n" +
                "{/* 15 160.000\t*//* 0 650.0*/80.000,\t/* 1 800.0*/80.000,\t/* 2 1100.0*/80.000,\t/* 3 1400.0*/80.000,\t/* 4 1700.0*/80.000,\t/* 5 2000.0*/80.000,\t/* 6 2300.0*/80.000,\t/* 7 2600.0*/80.000,\t/* 8 2900.0*/80.000,\t/* 9 3200.0*/80.000,\t/* 10 3500.0*/80.000,\t/* 11 3800.0*/80.000,\t/* 12 4100.0*/80.000,\t/* 13 4400.0*/80.000,\t/* 14 4700.0*/80.000,\t/* 15 7000.0*/80.000,\t},\n" +
                "};\n" +
                "\n" +
                "\tcopyArray(config->veLoadBins, hardCodedveRpmBins);\n" +
                "\tcopyArray(config->veRpmBins, hardCodedveLoadBins);\n" +
                "\tcopyTable(config->veTable, hardCodedveTable);\n" +
                "}\n";

        String tableReference = "config->ve";
        String tableName = "veTable";

        String copyMethodBody = TS2C.getCopyMethodBody(tableReference, model, tableName);

        CurveData xRpmCurve = CurveData.valueOf(TUNE_NAME, model.getXBin(tableName), model);
        CurveData yLoadCurve = CurveData.valueOf(TUNE_NAME, model.getYBin(tableName), model);

        String tableSource = TS2C.getTableCSourceCode2(TUNE_NAME, tableName, model);

        String completeMethod = "static void prefixcanned" + tableName + "() {\n" +
                "\t" + xRpmCurve.getCsourceCode() +
                "\t" + yLoadCurve.getCsourceCode() +
                tableSource +
                copyMethodBody +
                "}\n";

        assertEquals(expected, completeMethod);
    }

    @Test
    public void testCopyCode() {
        String tableReference = "config->ve";
        String tableName = "veTable";

        String copyMethodBody = TS2C.getCopyMethodBody(tableReference, model, tableName);
        assertEquals("\tcopyArray(config->veLoadBins, hardCodedveRpmBins);\n" +
                "\tcopyArray(config->veRpmBins, hardCodedveLoadBins);\n" +
                "\tcopyTable(config->veTable, hardCodedveTable);\n", copyMethodBody);
    }

    @Test
    public void testMeta() throws IOException {
        final ConfigurationImageWithMeta configurationImage = ConfigurationImageFile.readFromFile(TEST_BINARY_FILE);
        assertNotNull(configurationImage);
        final ConfigurationImageMeta meta = configurationImage.getMeta();
        assertInstanceOf(ConfigurationImageMetaVersion0_0.class, meta);
        final ConfigurationImageMetaVersion0_0 metaVersion0_0 = (ConfigurationImageMetaVersion0_0) meta;
        assertEquals(LEGACY_TOTAL_CONFIG_SIZE, metaVersion0_0.getImageSize());
        assertEquals("rusEFI TEST_BRANCH.TEST_TAG.TEST_DEVICE.TEST_HASH\0", metaVersion0_0.getEcuSignature());
    }

    @Test
    public void testCompareBinaryToTSTune() throws Exception {
        Msq tsTune = Msq.readTune(TUNE_NAME);
        System.out.println(tsTune);
        assertNotNull("signature", tsTune.getVersionInfo().getSignature());

        Constant flow = tsTune.findPage().findParameter("injector_flow");
        assertNotNull(flow);
        assertEquals("2", flow.getDigits());

        ConfigurationImage tsBinaryData = tsTune.asImage(model);

        System.out.println("Reading " + TEST_BINARY_FILE);
        ConfigurationImage fileBinaryData = ConfigurationImageFile.readFromFile(TEST_BINARY_FILE);

        int mismatchCounter = compareImages(tsBinaryData, fileBinaryData, model);
        assertEquals(0, mismatchCounter);
    }

    @Test
    public void testWriteAndReadTSTune() throws Exception {
        ConfigurationImage fileBinaryData = ConfigurationImageFile.readFromFile(TEST_BINARY_FILE);

        Path path = Files.createTempFile("unit_test_", ".xml");
        String fileName = path.getFileName().toString();

        // writing TS XML tune file with rusEFI code
        Msq tuneFromBinary = MsqFactory.valueOf(fileBinaryData, model);
        tuneFromBinary.writeXmlFile(fileName);

        Constant batteryCorrection = tuneFromBinary.findPage().findParameter("injector_battLagCorrBins");
        assertNotNull(batteryCorrection);
        assertEquals("2", batteryCorrection.getDigits());

        Constant flow = tuneFromBinary.findPage().findParameter("injector_flow");
        assertNotNull(flow);
        assertEquals("2", flow.getDigits());

        Constant nonEmptyFormula = tuneFromBinary.findPage().findParameter("legacyFormulas1");
        assertNotNull(nonEmptyFormula);

        /**
         * Empty strings values should be omitted from the tune
         */
        Constant emptyFormula = tuneFromBinary.findPage().findParameter("legacyFormulas2");
        assertNull(emptyFormula);

        Constant enumField = tuneFromBinary.findPage().findParameter("acRelayPin");
        // quotes are expected
        assertEquals("\"NONE\"", enumField.getValue());

        // and now reading that XML back
        Msq tuneFromFile = Msq.readTune(fileName);
        assertNotNull(tuneFromFile.getVersionInfo().getSignature());

        ConfigurationImage binaryDataFromXml = tuneFromFile.asImage(model);

        /**
         * Looks like I am not getting something right around Field#FIELD_PRECISION
         * See also TuneWriterTest :(
         */
        assertEquals(66, compareImages(binaryDataFromXml, fileBinaryData, model), "Binary match expected");
        // todo: looks like this is not removing the temporary file?
        Files.delete(path);
    }

    private static int compareImages(ConfigurationImage image1, ConfigurationImage fileData, IniFileModel ini) {
        byte[] tsBinaryDataContent = image1.getContent();
        byte[] fileBinaryDataContent = fileData.getContent();

        int mismatchCounter = 0;

        for (int i = 0; i < tsBinaryDataContent.length; i++) {
            byte tsByte = tsBinaryDataContent[i];
            byte fileByte = fileBinaryDataContent[i];
            if (tsByte != fileByte) {
                IniField field = ini.findByOffset(i);
                if (field instanceof ScalarIniField) {
                    System.out.println("    Image " + field.getValue(image1));
                    System.out.println("FileImage " + field.getValue(fileData));
                }
                System.out.println("Mismatch at offset=" + i + ", " + (field == null ? "(no field)" : field) + " runtime=" + tsByte + "/file=" + fileByte);
                mismatchCounter++;
            }
        }
        System.out.println("Total mismatch count " + mismatchCounter);
        return mismatchCounter;
    }
}
