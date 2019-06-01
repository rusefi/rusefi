package com.rusefi.test.integration;

import com.rusefi.ConfigDefinition;
import org.junit.Ignore;
import org.junit.Test;

import java.io.IOException;


//todo: un-ignore once Ant run works which is probably messed up about missing resources
@Ignore
public class IntegrationTest {
    @Test
    public void largeTest() throws IOException {

        String[] parameters = new String[]{
                ConfigDefinition.KEY_DEFINITION,
                getClass().getResource("define.txt").getFile(),
                ConfigDefinition.KEY_TS_DESTINATION,
                getClass().getResource(".").getFile(),
                ConfigDefinition.KEY_PREPEND,
                getClass().getResource("prepend.txt").getFile()
        };

        ConfigDefinition.main(parameters);

        // todo: actually read and validate output
        // todo: continues integration for this whole module
    }
}
