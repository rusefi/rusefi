package com.rusefi.ts_plugin;

import com.efiAnalytics.plugin.ecu.ControllerAccess;
import com.rusefi.TsTuneReader;
import com.rusefi.core.ui.AutoupdateUtil;
import com.rusefi.tools.online.Online;
import com.rusefi.tools.online.UploadResult;
import org.apache.http.concurrent.FutureCallback;
import org.jetbrains.annotations.NotNull;
import org.putgemin.VerticalFlowLayout;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.io.File;
import java.util.Objects;
import java.util.function.Supplier;

public class LogUploadSelector {
    private final JPanel content = new JPanel(new BorderLayout());
    private final JLabel uploadState = new JLabel();

    private final JPanel fileList = new JPanel(new VerticalFlowLayout());

    private final Supplier<ControllerAccess> controllerAccessSupplier;

    public LogUploadSelector(Supplier<ControllerAccess> controllerAccessSupplier) {
        this.controllerAccessSupplier = controllerAccessSupplier;

        JButton refresh = new JButton("Refresh");
        refresh.addActionListener(new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                refresh();
            }
        });

        JPanel topPanel = new JPanel(new FlowLayout());
        topPanel.add(refresh);

        JPanel filePanel = new JPanel(new BorderLayout());
        filePanel.add(fileList, BorderLayout.CENTER);

        var screenSize = Toolkit.getDefaultToolkit().getScreenSize();
        fileList.setMaximumSize(new Dimension((int)(screenSize.width / 1.3), 800));
        fileList.setPreferredSize(new Dimension((int)(screenSize.width / 1.3), 800));

        JScrollPane fileScroll = new JScrollPane(fileList, JScrollPane.VERTICAL_SCROLLBAR_ALWAYS, JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);



        content.add(topPanel, BorderLayout.NORTH);
        content.add(fileScroll, BorderLayout.CENTER);
        content.add(uploadState, BorderLayout.SOUTH);
        refresh();
    }

    private void refresh() {
        fileList.removeAll();

        String[] ecuConfigurationNames = controllerAccessSupplier.get().getEcuConfigurationNames();
        if (ecuConfigurationNames != null && ecuConfigurationNames.length > 0) {
            String folder = getLogsFolderDir(ecuConfigurationNames[0]);
            processFolder(folder);
        }

        AutoupdateUtil.trueLayout(content);
    }

    private void processFolder(String folder) {
        for (String fileName : Objects.requireNonNull(new File(folder).list((dir, name) -> name.endsWith(".mlg")))) {
            JPanel panel = new JPanel(new FlowLayout());
            JButton delete = new JButton("Delete");
            JButton upload = new JButton("Upload");
            final String fullFileName = folder + File.separator + fileName;

            panel.add(delete);
            panel.add(upload);

            delete.addActionListener(new AbstractAction() {
                @SuppressWarnings("ResultOfMethodCallIgnored")
                @Override
                public void actionPerformed(ActionEvent e) {
                    int result = JOptionPane.showConfirmDialog(null, "Are you sure you want to remove " + fileName,
                            "rusEfi", JOptionPane.YES_NO_OPTION);
                    if (result == JOptionPane.YES_OPTION) {
                        new File(fullFileName).delete();
                    }
                }
            });

            upload.addActionListener(new AbstractAction() {
                @Override
                public void actionPerformed(ActionEvent e) {

                    Online.uploadFile(content, new FutureCallback<UploadResult>() {
                        @Override
                        public void completed(UploadResult uploadResult) {
                            SwingUtilities.invokeLater(() -> UploadView.setResult(uploadResult, uploadState));
                        }

                        @Override
                        public void failed(Exception e) {

                        }

                        @Override
                        public void cancelled() {

                        }
                    }, fullFileName);

                }
            });


            panel.add(new JLabel(fileName));
            fileList.add(panel);
        }
    }

    @NotNull
    public static String getLogsFolderDir(String projectName) {
        return TsTuneReader.getProjectsDir() + File.separator + projectName + File.separator + "DataLogs";
    }

    public JComponent getContent() {
        return content;
    }
}
