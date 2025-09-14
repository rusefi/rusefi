package com.rusefi.maintenance.migration.issue8572;

import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.CalibrationsHelper;
import com.rusefi.maintenance.CalibrationsInfo;
import com.rusefi.maintenance.TestTuneMigrationContext;
import org.junit.jupiter.api.Test;

import javax.xml.bind.JAXBException;

import static java.util.Collections.emptySet;
import static org.junit.jupiter.api.Assertions.assertFalse;

public class TestIssue8572 {
    @Test
    void test() throws JAXBException {
        TestTuneMigrationContext testContext = TestTuneMigrationContext.load(
            "src/test/java/com/rusefi/maintenance/migration/issue8572/test_data"
        );
        assertFalse(testContext.getPrevIniFile().getSecondaryIniFields().isEmpty());
        CalibrationsInfo result = CalibrationsHelper.mergeCalibrations(
            testContext.getPrevIniFile(),
            testContext.getPrevTune(),
            testContext.getUpdatedCalibrationsInfo(),
            testContext.getCallbacks(),
            emptySet()
        ).get();

        CalibrationsHelper.backUpCalibrationsInfo(result, "issue", UpdateOperationCallbacks.CONSOLE);
    }
}
