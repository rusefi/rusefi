package com.rusefi.ui.livedocs.test;

import com.rusefi.ui.livedocs.LiveDocPanel;
import org.junit.Test;

import static org.junit.Assert.assertNotNull;

public class LiveDocPanelTest {
    @Test
    public void testFindField() throws NoSuchFieldException {
//        Field reflectField = Fields.class.getField("LDS_CLT_STATE_INDEX");
//        assertNotNull(reflectField);
    }

    @Test
    public void testValidDocumentationMetaInfo() {
        assertNotNull(LiveDocPanel.createLiveDocumentationPanel(null));
    }

    @Test
    public void testValidSensorsMetaInfo() {
        assertNotNull(LiveDocPanel.createSensorsLiveDataPanel(null));
    }
}
