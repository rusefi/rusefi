package com.rusefi.ui.widgets;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.geom.Path2D;
import java.awt.geom.RoundRectangle2D;

/**
 * A button with a default action on the left and a dropdown for advanced actions on the right.
 * Renders as a single unified control matching FlatLaf's component style.
 */
public class JSplitButton extends JPanel {
    private final JButton mainButton;
    private final JButton arrowButton;
    private JPopupMenu popupMenu;

    public JSplitButton(String text) {
        this(text, null);
    }

    public JSplitButton(String text, Icon icon) {
        setLayout(new BorderLayout(0, 0));
        setOpaque(false);

        mainButton = new JButton(text, icon);
        mainButton.putClientProperty("Button.arc", 0);
        mainButton.setBorderPainted(false);

        arrowButton = createArrowButton();

        add(mainButton, BorderLayout.CENTER);
        add(arrowButton, BorderLayout.EAST);

        arrowButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                showPopup();
            }
        });
    }

    public void setIcon(Icon icon) {
        mainButton.setIcon(icon);
    }

    public void setText(String text) {
        mainButton.setText(text);
    }

    public void setPopupMenu(JPopupMenu popupMenu) {
        this.popupMenu = popupMenu;
    }

    public void addActionListener(ActionListener listener) {
        mainButton.addActionListener(listener);
    }

    private void showPopup() {
        if (popupMenu != null) {
            popupMenu.show(this, 0, getHeight());
        }
    }

    @Override
    public void setToolTipText(String text) {
        mainButton.setToolTipText(text);
        arrowButton.setToolTipText(text);
    }

    public void setMainButtonEnabled(boolean enabled) {
        mainButton.setEnabled(enabled);
    }

    public void setArrowButtonEnabled(boolean enabled) {
        arrowButton.setEnabled(enabled);
    }

    @Override
    public void setEnabled(boolean enabled) {
        super.setEnabled(enabled);
        mainButton.setEnabled(enabled);
        arrowButton.setEnabled(enabled);
    }

    @Override
    protected void paintBorder(Graphics g) {
        int arc = UIManager.getInt("Button.arc");
        Color borderColor = UIManager.getColor("Component.borderColor");
        if (borderColor == null) {
            borderColor = Color.GRAY;
        }
        Graphics2D g2 = (Graphics2D) g.create();
        g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
        g2.setColor(borderColor);
        g2.draw(new RoundRectangle2D.Float(0.5f, 0.5f, getWidth() - 1f, getHeight() - 1f, arc, arc));
        g2.dispose();
    }

    @Override
    protected void paintChildren(Graphics g) {
        int arc = UIManager.getInt("Button.arc");
        Graphics2D g2 = (Graphics2D) g.create();
        g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
        if (arc > 0) {
            g2.clip(new RoundRectangle2D.Float(1, 1, getWidth() - 2, getHeight() - 2, arc, arc));
        }
        super.paintChildren(g2);

        // Draw separator on top so it isn't hidden by the inner button hover backgrounds
        Color sepColor = UIManager.getColor("ComboBox.buttonSeparatorColor");
        if (sepColor == null) {
            sepColor = UIManager.getColor("Component.borderColor");
        }
        if (sepColor != null) {
            int x = mainButton.getX() + mainButton.getWidth();
            g2.setColor(sepColor);
            g2.drawLine(x, 1, x, getHeight() - 2);
        }
        g2.dispose();
    }

    private static JButton createArrowButton() {
        JButton button = new JButton() {
            @Override
            protected void paintComponent(Graphics g) {
                super.paintComponent(g);
                Color arrowColor = isEnabled()
                    ? UIManager.getColor("ComboBox.buttonArrowColor")
                    : UIManager.getColor("ComboBox.buttonDisabledArrowColor");
                if (arrowColor == null) {
                    arrowColor = getForeground();
                }
                Graphics2D g2 = (Graphics2D) g.create();
                g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
                g2.setColor(arrowColor);
                float cx = getWidth() / 2f;
                float cy = getHeight() / 2f;
                float w = 4.5f;
                float h = 2.5f;
                g2.setStroke(new BasicStroke(1.5f, BasicStroke.CAP_ROUND, BasicStroke.JOIN_ROUND));
                Path2D.Float path = new Path2D.Float();
                path.moveTo(cx - w, cy - h);
                path.lineTo(cx, cy + h);
                path.lineTo(cx + w, cy - h);
                g2.draw(path);
                g2.dispose();
            }
        };
        button.putClientProperty("Button.arc", 0);
        button.setBorderPainted(false);
        button.setFocusable(false);
        button.setMargin(new Insets(0, 8, 0, 8));
        return button;
    }
}
