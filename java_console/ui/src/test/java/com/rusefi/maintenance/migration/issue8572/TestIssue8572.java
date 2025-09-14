package com.rusefi.maintenance.migration.issue8572;

import com.rusefi.maintenance.TestTuneMigrationContext;
import org.junit.jupiter.api.Test;

import javax.xml.bind.JAXBException;

public class TestIssue8572 {
    @Test
    void test() throws JAXBException {
        TestTuneMigrationContext result = TestTuneMigrationContext.load(
            "src/test/java/com/rusefi/maintenance/migration/issue8572/test_data"
        );
    }
}
