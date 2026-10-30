package com.rusefi.ui.widgets;

import com.devexperts.logging.Logging;
import com.opensr5.ini.ExpressionEvaluator;
import com.opensr5.ini.GaugeModel;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.TsStringFunction;
import com.rusefi.core.ISensorCentral;
import com.rusefi.core.ISensorHolder;
import com.rusefi.core.SensorCentral;
import com.rusefi.core.SensorSubscription;
import com.rusefi.io.ConnectionStatusLogic;
import com.opensr5.ConfigurationImage;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.ui.UIContext;
import com.rusefi.ui.widgets.tune.LiveValueTrace;
import com.rusefi.ui.widgets.tune.TuningStyle;

import javax.swing.*;
import java.awt.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.geom.RoundRectangle2D;
import java.util.HashSet;
import java.util.Set;
import java.util.function.Consumer;

import static com.devexperts.logging.Logging.getLogging;

/**
 * Compact text-only gauge showing channel title, value, and units.
 * Used in the Tuning view strip for always-visible live data. Behind the text the card fills by
 * where the value sits in the gauge's range and draws its last seconds as a faint line
 * ({@link LiveValueTrace}), so oscillation shows at a glance.
 * Handles expression-based labels via SensorCentral resolved labels,
 * matching the pattern used by SensorGauge.
 */
public class TextGauge {
    private static final Logging log = getLogging(TextGauge.class);
    private static final String NO_CONNECTION = "---";

    private final LiveValueTrace trace = new LiveValueTrace();
    private final JPanel content = new JPanel(new BorderLayout(0, 0)) {
        @Override
        protected void paintComponent(Graphics g) {
            super.paintComponent(g);
            // below the title row, so title and units stay on the plain card; inside the 1 px card
            // outline, which the border paints on top
            int top = header.getY() + header.getHeight();
            Graphics2D g2 = (Graphics2D) g.create();
            try {
                g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
                g2.clip(new RoundRectangle2D.Double(1, 1, getWidth() - 2, getHeight() - 2,
                    TuningStyle.ARC - 1, TuningStyle.ARC - 1));
                trace.paint(g2, new Rectangle(1, top, getWidth() - 2, getHeight() - 1 - top), null, valueLabel.getForeground());
            } finally {
                g2.dispose();
            }
        }
    };
    private final JLabel titleLabel = new JLabel();
    private final JLabel valueLabel = new JLabel();
    private final JLabel unitsLabel = new JLabel();
    private final JPanel header = new JPanel(new BorderLayout());

    private ISensorCentral.ListenerToken valueToken;
    private SensorCentral.ResponseListenerToken responseToken;
    private final ConnectionStatusLogic.Listener connectionStatusListener;
    private boolean active;
    private GaugeModel gaugeModel;

    public TextGauge(UIContext uiContext, String channelName, Consumer<String> onChannelChange) {
        TuningStyle.styleLiveValue(content, 3, 8);

        Font baseFont = UIManager.getFont("Label.font");
        if (baseFont != null) {
            titleLabel.setFont(baseFont.deriveFont(Font.PLAIN, baseFont.getSize() - 1f));
            valueLabel.setFont(baseFont.deriveFont(Font.BOLD, baseFont.getSize() + 9f));
            unitsLabel.setFont(baseFont.deriveFont(Font.PLAIN, baseFont.getSize() - 1f));
        }
        titleLabel.setForeground(TuningStyle.muted());
        unitsLabel.setForeground(TuningStyle.muted());

        // Title and units share the top row (units in the right corner), the value fills the rest.
        // A long title is cut with an ellipsis before the units.
        titleLabel.setHorizontalAlignment(SwingConstants.LEFT);
        valueLabel.setHorizontalAlignment(SwingConstants.CENTER);
        unitsLabel.setHorizontalAlignment(SwingConstants.RIGHT);
        unitsLabel.setBorder(BorderFactory.createEmptyBorder(0, 6, 0, 0));

        header.setOpaque(false);
        header.add(titleLabel, BorderLayout.CENTER);
        header.add(unitsLabel, BorderLayout.EAST);

        content.add(header, BorderLayout.NORTH);
        content.add(valueLabel, BorderLayout.CENTER);

        valueLabel.setText(NO_CONNECTION);
        valueLabel.setForeground(TuningStyle.muted());

        content.addMouseListener(new MouseAdapter() {
            @Override
            public void mousePressed(MouseEvent e) {
                if (SwingUtilities.isRightMouseButton(e)) {
                    showPopupMenu(uiContext, e, channelName, onChannelChange);
                }
            }
        });

        setChannel(uiContext, channelName);

        connectionStatusListener = isConnected -> {
            if (!isConnected) {
                SwingUtilities.invokeLater(() -> {
                    valueLabel.setText(NO_CONNECTION);
                    valueLabel.setForeground(TuningStyle.muted());
                    trace.clear();
                    content.repaint();
                });
            }
        };
        ConnectionStatusLogic.INSTANCE.addAndFireListener(connectionStatusListener);
    }

    private void setChannel(UIContext uiContext, String gaugeName) {
        cleanup();

        IniFileModel ini = uiContext.iniFileState.getIniFileModel();
        GaugeModel gaugeModel = (ini != null) ? ini.getGauge(gaugeName) : null;

        final String channel = (gaugeModel != null) ? gaugeModel.getChannel() : gaugeName;
        this.gaugeModel = gaugeModel;
        if (gaugeModel != null) {
            BinaryProtocol bp = uiContext.getBinaryProtocol();
            ConfigurationImage image = bp == null ? null : bp.getControllerConfiguration();
            GaugeScale scale = GaugeScale.fromIni(gaugeModel, ini, image);
            trace.setRange(scale.getMin(), scale.getMax());
            trace.setZones(CircularGauge.Zones.of(scale));
        } else {
            trace.setRange(Double.NaN, Double.NaN);
            trace.setZones(null);
        }
        content.repaint();

        // Handle dynamic string labels the same way SensorGauge does.
        boolean dynamicTitle = gaugeModel != null && gaugeModel.getTitleValue().isExpression()
            && TsStringFunction.containsStringFunction(gaugeModel.getTitle());
        boolean dynamicUnits = gaugeModel != null && gaugeModel.getUnitsValue().isExpression()
            && TsStringFunction.containsStringFunction(gaugeModel.getUnits());
        if (dynamicTitle || dynamicUnits) {
            if (dynamicTitle) {
                titleLabel.setText("");
            } else {
                titleLabel.setText(gaugeModel.getTitle());
            }
            if (dynamicUnits) {
                unitsLabel.setText("");
            } else {
                unitsLabel.setText(gaugeModel.getUnits());
            }

            final String[] lastLabels = {null, null};
            Set<String> labelsSensors = new HashSet<>();
            if (dynamicTitle) {
                labelsSensors.addAll(ExpressionEvaluator.extractVariables(gaugeModel.getTitle()));
            }
            if (dynamicUnits) {
                labelsSensors.addAll(ExpressionEvaluator.extractVariables(gaugeModel.getUnits()));
            }

            SensorCentral.ResponseListener responseListener = () ->
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
            responseToken = SensorCentral.getInstance().addListener(
                responseListener, new SensorSubscription(labelsSensors.toArray(new String[0])));
        } else {
            final String title = (gaugeModel != null) ? gaugeModel.getTitle() : gaugeName;
            final String units = (gaugeModel != null) ? gaugeModel.getUnits() : "";
            titleLabel.setText(title);
            unitsLabel.setText(units);
            responseToken = null;
        }

        valueToken = SensorCentral.getInstance().addListener(channel, value ->
            SwingUtilities.invokeLater(() -> {
                if (ConnectionStatusLogic.INSTANCE.isConnected()) {
                    showValue(value, gaugeModel);
                }
            })
        );
        setActive(active);

        Double currentValue = SensorCentral.getInstance().getValue(channel);
        if (currentValue != null && ConnectionStatusLogic.INSTANCE.isConnected()) {
            showValue(currentValue, gaugeModel);
        }
    }

    private void showValue(double value, GaugeModel gaugeModel) {
        showValue(value, gaugeModel, System.currentTimeMillis());
    }

    private void showValue(double value, GaugeModel gaugeModel, long nowMs) {
        this.gaugeModel = gaugeModel;
        trace.add(value, nowMs);
        valueLabel.setText(formatValue(value, gaugeModel));
        valueLabel.setForeground(trace.textColor(TuningStyle.text()));
        content.repaint();
    }

    /** Shows a reading taken at {@code nowMs}, as a live update would. */
    void showValueForTests(double value, long nowMs) {
        showValue(value, gaugeModel, nowMs);
    }

    /** Package-private so tests check what the card draws behind its text. */
    LiveValueTrace getTraceForTests() {
        return trace;
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
                    JMenuItem mi = new JMenuItem(GaugeMenuLabel.getTitle(uiContext, gauge));
                    mi.addActionListener(ev -> {
                        setChannel(uiContext, gauge.getName());
                        onChannelChange.accept(gauge.getName());
                    });
                    categoryMenu.add(mi);
                }
            }
        }
        if (pm.getSubElements().length > 0) {
            GaugeMenuSearch.addTo(pm);
            pm.show(e.getComponent(), e.getX(), e.getY());
        }
    }

    private void cleanup() {
        if (valueToken != null) {
            valueToken.remove();
            valueToken = null;
        }
        if (responseToken != null) {
            responseToken.remove();
            responseToken = null;
        }
    }

    public void setActive(boolean active) {
        this.active = active;
        if (valueToken != null) {
            valueToken.setActive(active);
        }
        if (responseToken != null) {
            responseToken.setActive(active);
        }
    }

    public void destroy() {
        cleanup();
        ConnectionStatusLogic.INSTANCE.removeListener(connectionStatusListener);
    }

    public JPanel getContent() {
        return content;
    }
}
