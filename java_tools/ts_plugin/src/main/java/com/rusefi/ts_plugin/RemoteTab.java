package com.rusefi.ts_plugin;

import com.rusefi.LocalApplicationProxy;
import com.rusefi.NamedThreadFactory;
import com.rusefi.SignatureHelper;
import com.rusefi.autoupdate.AutoupdateUtil;
import com.rusefi.server.ControllerInfo;
import com.rusefi.tools.online.HttpUtil;
import com.rusefi.tools.online.ProxyClient;
import com.rusefi.tools.online.PublicSession;
import com.rusefi.ui.util.URLLabel;
import org.putgemin.VerticalFlowLayout;

import javax.swing.*;
import java.awt.*;
import java.io.IOException;
import java.util.List;
import java.util.concurrent.Executor;
import java.util.concurrent.Executors;

import static com.rusefi.ui.storage.PersistentConfiguration.getConfig;

/**
 * see RemoteTabSandbox
 */
public class RemoteTab {
    private static final String APPLICATION_PORT = "application_port";
    private final JComponent content = new JPanel(new BorderLayout());

    private final JPanel list = new JPanel(new VerticalFlowLayout());

    private final Executor listDownloadExecutor = Executors.newSingleThreadExecutor(new NamedThreadFactory("online list downloader"));

    public RemoteTab() {
        JButton refresh = new JButton("Refresh List");
        refresh.addActionListener(e -> requestListDownload());

        JTextField applicationPort = new JTextField();
        String portProperty = getConfig().getRoot().getProperty(APPLICATION_PORT, Integer.toString(LocalApplicationProxy.SERVER_PORT_FOR_APPLICATIONS));
        applicationPort.setText(portProperty);


        JPanel topPanel = new JPanel(new FlowLayout());
        topPanel.add(refresh);
        topPanel.add(new JLabel("Local Port"));
        topPanel.add(applicationPort);
        content.add(topPanel, BorderLayout.NORTH);
        content.add(list, BorderLayout.CENTER);
        requestListDownload();
    }

    private void requestListDownload() {
        listDownloadExecutor.execute(new Runnable() {
            @Override
            public void run() {
                String url = HttpUtil.RUSEFI_PROXY_JSON_API_PREFIX + "/list_online";

                List<PublicSession> userDetails;
                try {
                    userDetails = ProxyClient.getOnlineUsers(HttpUtil.HTTP_PORT);
                    SwingUtilities.invokeLater(new Runnable() {
                        @Override
                        public void run() {
                            showList(userDetails);
                        }
                    });
                } catch (IOException e) {
                    e.printStackTrace();
                    return;
                }
                System.out.println(userDetails);

            }
        });
    }

    private void showList(List<PublicSession> userDetails) {
        list.removeAll();
        for (PublicSession user : userDetails) {
            list.add(createPanel(user));
        }
        AutoupdateUtil.trueLayout(list);
    }

    private JComponent createPanel(PublicSession publicSession) {
        JComponent userPanel = new JPanel(new FlowLayout());
        userPanel.add(new JLabel(publicSession.getUserDetails().getUserName()));
        ControllerInfo controllerInfo = publicSession.getControllerInfo();
        userPanel.add(new JLabel(controllerInfo.getVehicleName() + " " + controllerInfo.getEngineMake() + " " + controllerInfo.getEngineCode()));

        userPanel.add(new URLLabel(SignatureHelper.getUrl(controllerInfo.getSignature())));




        return userPanel;
    }

    public JComponent getContent() {
        return content;
    }
}
