package com.rusefi.ts_plugin.auth;

import com.rusefi.server.UserDetails;

import javax.swing.*;
import java.awt.*;

public class SelfInfo {
    private final JLabel jLabel = new JLabel();

    public SelfInfo() {
        InstanceAuthContext.listeners.add(new InstanceAuthContext.Listener() {
            @Override
            public void onUserDetails(UserDetails userDetails) {
                setInfo();
            }
        });
        setInfo();
    }

    private void setInfo() {
        UserDetails userDetails = InstanceAuthContext.self;
        if (userDetails == null) {
            jLabel.setText("Authorizing...");
        } else {
            jLabel.setText("Logged in as " + userDetails.getUserName());
        }
    }

    public Component getContent() {
        return jLabel;
    }
}
