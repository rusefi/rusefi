package com.rusefi.pcb.test;


import com.rusefi.misc.NameAndOffset;
import com.rusefi.misc.ChangesModel;
import org.junit.Assert;
import org.junit.Test;

import java.util.Arrays;

import static junit.framework.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

/**
 * @author Andrey Belomutskiy
 *         1/24/14
 */
public class ChangesModelTest {
    @Test
    public void testConfigParser() {

        ChangesModel model = ChangesModel.getInstance();
        model.clear();

        model.read(Arrays.asList("reMove    c1", "add mmc_usb_1.kicad_pcb",
                "adD mmc_usb_1.kicad_pcb 4 6",
                "optimize mmc_usb_1.kicad_pcb out.x"));

        assertEquals(1, model.DEL_REQUESTS.size());
        assertTrue(model.DEL_REQUESTS.contains("C1"));

        assertEquals(2, model.ADD_REQUESTS.size());

        NameAndOffset ar = model.ADD_REQUESTS.get(1);
        Assert.assertEquals(4.0, ar.x, 0);
        Assert.assertEquals(6.0, ar.y, 0);

        assertEquals(1, model.OPTIMIZE_REQUESTS.size());
    }
}
