package com.rusefi.ui.widgets;

import com.devexperts.logging.Logging;
import com.opensr5.ini.ExpressionEvaluator;
import com.opensr5.ini.GaugeModel;
import com.opensr5.ini.IniFileModel;
import com.rusefi.core.ISensorCentral;
import com.rusefi.core.ISensorHolder;
import com.rusefi.core.SensorCentral;
import com.rusefi.core.SensorSubscription;
import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.ui.UIContext;

import javax.swing.*;
import java.awt.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.HashSet;
import java.util.Set;
import java.util.function.Consumer;

import static com.devexperts.logging.Logging.getLogging;

/**
 * Compact text-only gauge showing channel title, value, and units.
 * Used in the Tuning view strip for always-visible live data.
 * Handles expression-based labels via SensorCentral resolved labels,
 * matching the pattern used by SensorGauge.
 */
public class TextGauge {
    private static final Logging log = getLogging(TextGauge.class);
    private static final String NO_CONNECTION = "---";

    private final JPanel content = new JPanel(new BorderLayout(2, 0));
    private final JLabel titleLabel = new JLabel();
    private final JLabel valueLabel = new JLabel();
    private final JLabel unitsLabel = new JLabel();

    private ISensorCentral.ListenerToken valueToken;
    private SensorCentral.ResponseListener responseListener;

    public TextGauge(UIContext uiContext, String channelName, Consumer<String> onChannelChange) {
        content.setBorder(BorderFactory.createCompoundBorder(
            BorderFactory.createLineBorder(Color.GRAY, 1),
            BorderFactory.createEmptyBorder(2, 4, 2, 4)
        ));
        content.setBackground(Color.WHITE);

        Font baseFont = UIManager.getFont("Label.font");
        if (baseFont != null) {
            titleLabel.setFont(baseFont.deriveFont(Font.BOLD, baseFont.getSize() - 1f));
            valueLabel.setFont(baseFont.deriveFont(Font.BOLD, baseFont.getSize() + 2f));
            unitsLabel.setFont(baseFont.deriveFont(baseFont.getSize() - 1f));
        }

        titleLabel.setHorizontalAlignment(SwingConstants.CENTER);
        valueLabel.setHorizontalAlignment(SwingConstants.CENTER);
        unitsLabel.setHorizontalAlignment(SwingConstants.CENTER);

        content.add(titleLabel, BorderLayout.NORTH);
        content.add(valueLabel, BorderLayout.CENTER);
        content.add(unitsLabel, BorderLayout.SOUTH);

        valueLabel.setText(NO_CONNECTION);
        valueLabel.setForeground(Color.RED);

        content.addMouseListener(new MouseAdapter() {
            @Override
            public void mousePressed(MouseEvent e) {
                if (SwingUtilities.isRightMouseButton(e)) {
                    showPopupMenu(uiContext, e, channelName, onChannelChange);
                }
            }
        });

        setChannel(uiContext, channelName);

        ConnectionStatusLogic.INSTANCE.addAndFireListener(isConnected -> {
            if (!isConnected) {
                SwingUtilities.invokeLater(() -> {
                    valueLabel.setText(NO_CONNECTION);
                    valueLabel.setForeground(Color.RED);
                });
            }
        });
    }

    private void setChannel(UIContext uiContext, String gaugeName) {
        cleanup();

        IniFileModel ini = uiContext.iniFileState.getIniFileModel();
        GaugeModel gaugeModel = (ini != null) ? ini.getGauge(gaugeName) : null;

        final String channel = (gaugeModel != null) ? gaugeModel.getChannel() : gaugeName;

        // Handle expression-based labels the same way SensorGauge does
        if (gaugeModel != null && (gaugeModel.getTitleValue().isExpression() || gaugeModel.getUnitsValue().isExpression())) {
            if (gaugeModel.getTitleValue().isExpression()) {
                titleLabel.setText("");
            } else {
                titleLabel.setText(gaugeModel.getTitle());
            }
            if (gaugeModel.getUnitsValue().isExpression()) {
                unitsLabel.setText("");
            } else {
                unitsLabel.setText(gaugeModel.getUnits());
            }

            final String[] lastLabels = {null, null};
            Set<String> labelsSensors = new HashSet<>();
            labelsSensors.addAll(ExpressionEvaluator.extractVariables(gaugeModel.getTitle()));
            labelsSensors.addAll(ExpressionEvaluator.extractVariables(gaugeModel.getUnits()));

            responseListener = () ->
                SwingUtilities.invokeLater(() -> {
                    ISensorHolder.ResolvedGaugeLabels labels = SensorCentral.getInstance().getResolvedLabels(gaugeName);
                    if (labels != null) {
                        boolean changed = false;
                        if (!labels.getTitle().equals(lastLabels[0])) {
                            lastLabels[0] = labels.getTitle();
                            titleLabel.setText(labels.getTitle());
                            changed = true;
                        }
                        if (!labels.getUnits().equals(lastLabels[1])) {
                            lastLabels[1] = labels.getUnits();
                            unitsLabel.setText(labels.getUnits());
                            changed = true;
                        }
                        if (changed) {
                            content.repaint();
                        }
                    }
                });
            SensorCentral.getInstance().addListener(responseListener, new SensorSubscription(labelsSensors.toArray(new String[0])));
        } else {
            final String title = (gaugeModel != null) ? gaugeModel.getTitle() : gaugeName;
            final String units = (gaugeModel != null) ? gaugeModel.getUnits() : "";
            titleLabel.setText(title);
            unitsLabel.setText(units);
            responseListener = null;
        }

        valueToken = SensorCentral.getInstance().addListener(channel, value ->
            SwingUtilities.invokeLater(() -> {
                if (ConnectionStatusLogic.INSTANCE.isConnected()) {
                    valueLabel.setText(formatValue(value, gaugeModel));
                    valueLabel.setForeground(Color.BLACK);
                }
            })
        );

        Double currentValue = SensorCentral.getInstance().getValue(channel);
        if (currentValue != null && ConnectionStatusLogic.INSTANCE.isConnected()) {
            valueLabel.setText(formatValue(currentValue, gaugeModel));
            valueLabel.setForeground(Color.BLACK);
        }
    }

    private static String formatValue(double value, GaugeModel gaugeModel) {
        int decimals = (gaugeModel != null) ? gaugeModel.getValueDecimalPlaces() : 1;
        if (decimals == 0) {
            return String.valueOf((long) value);
        }
        return String.format("%." + decimals + "f", value);
    }

    private void showPopupMenu(UIContext uiContext, MouseEvent e, String currentChannel, Consumer<String> onChannelChange) {
        JPopupMenu pm = new JPopupMenu();
        IniFileModel ini = uiContext.iniFileState.getIniFileModel();
        if (ini != null) {
            for (String category : ini.getGaugeCategories().keySet()) {
                JMenu categoryMenu = new JMenu(category);
                pm.add(categoryMenu);
                for (GaugeModel gauge : ini.getGaugeCategories().get(category).getGauges()) {
                    JMenuItem mi = new JMenuItem(gauge.getTitle());
                    mi.addActionListener(ev -> {
                        setChannel(uiContext, gauge.getName());
                        onChannelChange.accept(gauge.getName());
                    });
                    categoryMenu.add(mi);
                }
            }
        }
        if (pm.getSubElements().length > 0) {
            pm.show(e.getComponent(), e.getX(), e.getY());
        }
    }

    private void cleanup() {
        if (valueToken != null) {
            valueToken.remove();
            valueToken = null;
        }
        if (responseListener != null) {
            SensorCentral.getInstance().removeListener(responseListener);
            responseListener = null;
        }
    }

    public void destroy() {
        cleanup();
    }

    public JPanel getContent() {
        return content;
    }
}
