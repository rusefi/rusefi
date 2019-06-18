package com.rusefi.ui.livedocs;

import com.opensr5.ConfigurationImage;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.Field;
import com.rusefi.config.generated.Fields;
import com.rusefi.ldmp.ConfigRequest;
import com.rusefi.ldmp.FieldRequest;
import com.rusefi.ldmp.Request;
import com.rusefi.ldmp.TextRequest;
import com.rusefi.ldmp.generated.ThermistorsMeta;
import net.miginfocom.swing.MigLayout;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import java.awt.*;
import java.util.*;
import java.util.List;

public class LiveDocPanel {
    @NotNull
    public static JPanel getThermistorPanel(String title, String instancePrefix, final int id, Field[] values) {
        JPanel panel = new JPanel(new MigLayout());
        panel.setBorder(BorderFactory.createTitledBorder(title));

        List<RefreshActions> actions = createComponents(panel, ThermistorsMeta.CONTENT, values, instancePrefix);

        LiveDocHolder holder = new LiveDocHolder(id, actions, values) {
            @Override
            public boolean isVisible() {
                boolean isVisible = !panel.getVisibleRect().isEmpty();
                return isVisible && isRecursivelyVisible(panel);
            }
        };

        LiveDocsRegistry.INSTANCE.register(holder);
        return panel;
    }

    static boolean isRecursivelyVisible(Container c) {
        Container parent = c.getParent();
        return c.isVisible() && (parent == null || isRecursivelyVisible(parent));
    }

    private static List<RefreshActions> createComponents(JPanel panel, Request[] content, Field[] values, String instancePrefix) {
        List<RefreshActions> actionsList = new ArrayList<>();
        for (Request r : content) {
            if (r instanceof TextRequest) {
                TextRequest request = (TextRequest) r;
                if (request.isEol()) {
                    panel.add(new JLabel(), "wrap");
                } else {
                    panel.add(new JLabel(request.getValue().replaceAll("_", " ")));
                }
            } else if (r instanceof FieldRequest) {
                FieldRequest request = (FieldRequest) r;
                Field field = Field.findField(values, "", request.getField());
                JLabel label = new JLabel(field.getName());
                panel.add(label);
                actionsList.add(new RefreshActions() {
                    @Override
                    public void refresh(BinaryProtocol bp, byte[] response) {
                        String value = field.getValue(new ConfigurationImage(response)).toString();
                        label.setText(value);
                    }
                });
            } else if (r instanceof ConfigRequest) {
                ConfigRequest request = (ConfigRequest) r;
                Field field = Field.findField(Fields.VALUES, instancePrefix, request.getField());

                JLabel label = new JLabel(field.getName());
                actionsList.add(new RefreshActions() {
                    @Override
                    public void refresh(BinaryProtocol bp, byte[] response) {
                        String value = field.getAnyValue(bp.getController()).toString();
                        label.setText(value);
                    }
                });
                panel.add(label);

            } else {
                throw new UnsupportedOperationException(r.toString());
            }
        }
        return actionsList;
    }
}
