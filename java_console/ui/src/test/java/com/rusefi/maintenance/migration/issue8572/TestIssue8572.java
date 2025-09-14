package com.rusefi.maintenance.migration.issue8572;

import com.opensr5.ini.field.OrdinalOutOfRangeException;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.CalibrationsHelper;
import com.rusefi.maintenance.CalibrationsInfo;
import com.rusefi.maintenance.TestTuneMigrationContext;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import javax.xml.bind.JAXBException;

import static java.util.Collections.emptySet;

public class TestIssue8572 {
    @Test
    void test() throws JAXBException {
        TestTuneMigrationContext testContext = TestTuneMigrationContext.load(
            "src/test/java/com/rusefi/maintenance/migration/issue8572/test_data"
        );
        CalibrationsInfo result = CalibrationsHelper.mergeCalibrations(
            testContext.getPrevIniFile(),
            testContext.getPrevTune(),
            testContext.getUpdatedCalibrationsInfo(),
            testContext.getCallbacks(),
            emptySet()
        ).get();
        CalibrationsHelper.RETHROW = true;

        CalibrationsHelper.backUpCalibrationsInfo(result, "issue", UpdateOperationCallbacks.CONSOLE);
    }
}
