package com.rusefi.ui.wizard;

import com.rusefi.ui.UIContext;
import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;

public class NumberOfCylindersPanel {
    private final JLayeredPane layeredPane = new JLayeredPane();
    private final JPanel mainPanel = new JPanel(new GridBagLayout());
    private final JPanel overlayPanel = new JPanel() {
        @Override
        protected void paintComponent(Graphics g) {
            g.setColor(new Color(0, 0, 0, 150));
            g.fillRect(0, 0, getWidth(), getHeight());
            super.paintComponent(g);
        }
    };

    private final UIContext uiContext;

    public NumberOfCylindersPanel(UIContext uiContext) {
        this.uiContext = uiContext;
        layeredPane.add(mainPanel, JLayeredPane.DEFAULT_LAYER);
        layeredPane.add(overlayPanel, JLayeredPane.PALETTE_LAYER);
        overlayPanel.setVisible(false);
        overlayPanel.setOpaque(false);
        overlayPanel.setLayout(new GridBagLayout());

        overlayPanel.addMouseListener(new java.awt.event.MouseAdapter() {
            @Override
            public void mouseClicked(java.awt.event.MouseEvent e) {
                overlayPanel.setVisible(false);
            }
        });

        JLabel messageLabel = new JLabel();
        messageLabel.setForeground(Color.WHITE);
        scaleComponent(messageLabel, 2);
        overlayPanel.add(messageLabel);

        layeredPane.addComponentListener(new ComponentAdapter() {
            @Override
            public void componentResized(ComponentEvent e) {
                mainPanel.setBounds(0, 0, layeredPane.getWidth(), layeredPane.getHeight());
                updateOverlayBounds();
            }
        });

        GridBagConstraints gbc = new GridBagConstraints();
        gbc.gridx = 0;
        gbc.gridy = 0;
        gbc.insets = new Insets(20, 20, 20, 20);
        gbc.anchor = GridBagConstraints.CENTER;

        JLabel label = new JLabel("How many cylinders?");
        scaleComponent(label, 3);
        mainPanel.add(label, gbc);

        gbc.gridy++;
        JPanel buttonsPanel = new JPanel(new GridLayout(3, 4, 10, 10));
        int[] options = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 16};

        for (int option : options) {
            JButton button = new JButton(String.valueOf(option));
            scaleComponent(button, 3);
            buttonsPanel.add(button);

            boolean isGotcha = option == 7 || option == 9 || option == 16;
            if (isGotcha) {
                button.addActionListener(new ActionListener() {
                    @Override
                    public void actionPerformed(ActionEvent e) {
                        messageLabel.setText("<html><center>Oh, " + option +
                            "? If you <i>really</i> have an engine like that,<br>please give us a call!</center></html>");
                        overlayPanel.setVisible(true);
                        updateOverlayBounds();
                    }
                });
            }
        }

        mainPanel.add(buttonsPanel, gbc);
    }

    private void updateOverlayBounds() {
        int w = (int) (layeredPane.getWidth() * 0.8);
        int h = (int) (layeredPane.getHeight() * 0.8);
        int x = (layeredPane.getWidth() - w) / 2;
        int y = (layeredPane.getHeight() - h) / 2;
        overlayPanel.setBounds(x, y, w, h);
    }

    private void scaleComponent(JComponent component, float factor) {
        Font font = component.getFont();
        component.setFont(font.deriveFont(font.getSize() * factor));
    }

    public JComponent getPanel() {
        return layeredPane;
    }
}
