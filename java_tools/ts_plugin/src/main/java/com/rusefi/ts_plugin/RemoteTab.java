package com.rusefi.ts_plugin;

import com.rusefi.NamedThreadFactory;
import com.rusefi.server.UserDetails;
import com.rusefi.tools.online.HttpUtil;
import com.rusefi.tools.online.ProxyClient;
import org.apache.http.HttpResponse;

import javax.swing.*;
import java.awt.*;
import java.io.IOException;
import java.util.List;
import java.util.concurrent.Executor;
import java.util.concurrent.Executors;

public class RemoteTab {
    private final JComponent content = new JPanel(new BorderLayout());

    private final JButton refresh = new JButton("Refresh List");

    private final Executor listDownloadExecutor = Executors.newSingleThreadExecutor(new NamedThreadFactory("online list downloader"));

    public RemoteTab() {

        JPanel topPanel = new JPanel(new FlowLayout());

        topPanel.add(refresh);

        refresh.addActionListener(e -> requestListDownload());

        content.add(topPanel, BorderLayout.NORTH);
        requestListDownload();
    }

    private void requestListDownload() {
        listDownloadExecutor.execute(new Runnable() {
            @Override
            public void run() {
                String url = HttpUtil.RUSEFI_PROXY_JSON_API_PREFIX + "/list_online";

                List<UserDetails> userDetails;
                try {
                    userDetails = ProxyClient.getOnlineUsers(HttpUtil.HTTP_PORT);
                } catch (IOException e) {
                    e.printStackTrace();
                    return;
                }
                System.out.println(userDetails);

            }
        });
    }

    public JComponent getContent() {
        return content;
    }
}
