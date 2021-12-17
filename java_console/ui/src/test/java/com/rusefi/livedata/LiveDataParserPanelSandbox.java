package com.rusefi.livedata;

import com.rusefi.enums.live_data_e;
import com.rusefi.ui.UIContext;
import com.rusefi.ui.livedocs.LiveDocsRegistry;
import com.rusefi.ui.util.FrameHelper;

import javax.swing.*;

public class LiveDataParserPanelSandbox {
    public static void main(String[] args) {

        UIContext context = new UIContext();

        JPanel panel = LiveDataParserPanel.createLiveDataParserContent(context,
                LiveDataView.BOOST_CONTROL);

        LiveDocsRegistry.INSTANCE.refresh(new LiveDocsRegistry.LiveDataProvider() {
                                              @Override
                                              public byte[] provide(live_data_e context) {
                                                  System.out.println("provide");
                                                  return new byte[0];
                                              }
                                          }
        );

        new FrameHelper().showFrame(panel);
    }
}
