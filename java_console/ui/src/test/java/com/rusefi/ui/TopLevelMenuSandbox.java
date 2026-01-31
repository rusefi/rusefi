package com.rusefi.ui;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import com.rusefi.core.ui.FrameHelper;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.rusefi.tune.xml.Msq;
import com.rusefi.ui.widgets.tune.CalibrationDialogWidget;
import com.rusefi.ui.widgets.tune.MainMenuTreeWidget;

import javax.swing.*;
import javax.xml.bind.JAXBException;
import java.awt.*;
import java.io.FileNotFoundException;

import static com.rusefi.ui.basic.UiHelper.commonUiStartup;

public class TopLevelMenuSandbox {
    public static void main(String[] args) throws JAXBException, FileNotFoundException {
        commonUiStartup();

        String iniPath = "../java_console/ui/src/test/resources/january.ini";
        IniFileModel model = IniFileReaderUtil.readIniFile(iniPath);
        Msq msq = Msq.readTune("../java_console/ui/src/test/resources/january_tune.msq");
        ConfigurationImage ci = msq.asImage(model);

        SwingUtilities.invokeLater(() -> runAwt(model, ci));
    }

    private static void runAwt(IniFileModel model, ConfigurationImage ci) {
        FrameHelper frameHelper = new FrameHelper(JDialog.EXIT_ON_CLOSE);

        JPanel panel = new JPanel(new GridBagLayout());

        MainMenuTreeWidget left = new MainMenuTreeWidget(model);
        panel.add(left.getContentPane(), new GridBagConstraints(0, 0, 1, 1, 1, 1, GridBagConstraints.CENTER, GridBagConstraints.BOTH, new Insets(0, 0, 0, 0), 0, 0));

        CalibrationDialogWidget right = new CalibrationDialogWidget();
        panel.add(right.getContentPane(), new GridBagConstraints(1, 0, 1, 1, 2, 1, GridBagConstraints.CENTER, GridBagConstraints.BOTH, new Insets(0, 0, 0, 0), 0, 0));

        left.setOnSelect(subMenu -> {
            String dialogKey = subMenu.getKey();
            right.update(model.getDialogs().get(dialogKey), model, ci);
        });

        frameHelper.showFrame(panel);
        left.selectSubMenu("dwellSettings");
    }
}
