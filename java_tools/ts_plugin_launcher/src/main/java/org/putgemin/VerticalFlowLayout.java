package org.putgemin;
/*
 * @(#)VerticalFlowLayout.java  1.52 03/12/19
 *
 * Copyright 2004 Sun Microsystems, Inc. All rights reserved.
 * SUN PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

import java.awt.Component;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.Insets;
import java.awt.LayoutManager;
import java.io.ObjectInputStream;
import java.io.IOException;

/**
 * A flow layout arranges components in a directional flow, much
 * like lines of text in a paragraph. The flow direction is
 * determined by the container's <code>componentOrientation</code>
 * property and may be one of two values:
 * <ul>
 * <li><code>ComponentOrientation.TOP_TO_BOTTOM</code>
 * <li><code>ComponentOrientation.BOTTOM_TO_TOP</code>
 * </ul>
 * VerticalFlow layouts are typically used
 * to arrange buttons in a panel. It arranges buttons
 * horizontally until no more buttons fit on the same line.
 * The line alignment is determined by the <code>align</code>
 * property. The possible values are:
 * <ul>
 * <li>{@link #TOP TOP}
 * <li>{@link #BOTTOM BOTTOM}
 * <li>{@link #CENTER CENTER}
 * <li>{@link #LEADING LEADING}
 * <li>{@link #TRAILING TRAILING}
 * </ul>
 * <p/>
 * For example, the following picture shows an applet using the flow
 * layout manager (its default layout manager) to position three buttons:
 * <p/>
 * <img src="doc-files/VerticalFlowLayout-1.gif"
 * ALT="Graphic of Layout for Three Buttons"
 * ALIGN=center HSPACE=10 VSPACE=7>
 * <p/>
 * Here is the code for this applet:
 * <p/>
 * <hr><blockquote><pre>
 * import java.awt.*;
 * import java.applet.Applet;
 * <p/>
 * public class myButtons extends Applet {
 *     Button button1, button2, button3;
 *     public void init() {
 *         button1 = new Button("Ok");
 *         button2 = new Button("Open");
 *         button3 = new Button("Close");
 *         add(button1);
 *         add(button2);
 *         add(button3);
 *     }
 * }
 * </pre></blockquote><hr>
 * <p/>
 * A flow layout lets each component assume its natural (preferred) size.
 *
 * @author Arthur van Hoff
 * @author Sami Shaio
 * @version 1.52, 12/19/03
 * @see java.awt.ComponentOrientation
 * @since JDK1.0
 */
public class VerticalFlowLayout implements LayoutManager, java.io.Serializable {

    private boolean maximizeOtherDimension = false;

    public void setMaximizeOtherDimension(boolean max) {
        maximizeOtherDimension = max;
    }

    public boolean isMaximizeOtherDimension() {
        return maximizeOtherDimension;
    }

    /**
     * This value indicates that each row of components
     * should be left-justified.
     */
    public static final int TOP = 0;

    /**
     * This value indicates that each row of components
     * should be centered.
     */
    public static final int CENTER = 1;

    /**
     * This value indicates that each row of components
     * should be right-justified.
     */
    public static final int BOTTOM = 2;

    /**
     * This value indicates that each row of components
     * should be justified to the leading edge of the container's
     * orientation, for example, to the left in left-to-right orientations.
     *
     * @see java.awt.Component#getComponentOrientation
     * @see java.awt.ComponentOrientation
     * @since 1.2
     * Package-private pending API change approval
     */
    public static final int LEADING = 3;

    /**
     * This value indicates that each row of components
     * should be justified to the trailing edge of the container's
     * orientation, for example, to the right in left-to-right orientations.
     *
     * @see java.awt.Component#getComponentOrientation
     * @see java.awt.ComponentOrientation
     * @since 1.2
     * Package-private pending API change approval
     */
    public static final int TRAILING = 4;

    /**
     * <code>align</code> is the property that determines
     * how each row distributes empty space.
     * It can be one of the following values:
     * <ul>
     * <code>TOP</code>
     * <code>BOTTOM</code>
     * <code>CENTER</code>
     * <code>LEADING</code>
     * <code>TRAILING</code>
     * </ul>
     *
     * @serial
     * @see #getAlignment
     * @see #setAlignment
     */
    int align;          // This is for 1.1 serialization compatibility

    /**
     * <code>newAlign</code> is the property that determines
     * how each row distributes empty space for the Java 2 platform,
     * v1.2 and greater.
     * It can be one of the following three values:
     * <ul>
     * <code>TOP</code>
     * <code>BOTTOM</code>
     * <code>CENTER</code>
     * <code>LEADING</code>
     * <code>TRAILING</code>
     * </ul>
     *
     * @serial
     * @see #getAlignment
     * @see #setAlignment
     * @since 1.2
     */
    int newAlign;       // This is the one we actually use

    /**
     * The flow layout manager allows a seperation of
     * components with gaps.  The horizontal gap will
     * specify the space between components and between
     * the components and the borders of the
     * <code>Container</code>.
     *
     * @serial
     * @see #getHgap()
     * @see #setHgap(int)
     */
    int hgap;

    /**
     * The flow layout manager allows a seperation of
     * components with gaps.  The vertical gap will
     * specify the space between rows and between the
     * the rows and the borders of the <code>Container</code>.
     *
     * @serial
     * @see #getHgap()
     * @see #setHgap(int)
     */
    int vgap;

    /*
     * JDK 1.1 serialVersionUID
     */
    private static final long serialVersionUID = -7262534875583282631L;

    /**
     * Constructs a new <code>VerticalFlowLayout</code> with a centered alignment and a
     * default 5-unit horizontal and vertical gap.
     */
    public VerticalFlowLayout() {
        this(CENTER, 5, 5);
    }

    /**
     * Constructs a new <code>VerticalFlowLayout</code> with the specified
     * alignment and a default 5-unit horizontal and vertical gap.
     * The value of the alignment argument must be one of
     * <code>VerticalFlowLayout.TOP</code>, <code>VerticalFlowLayout.BOTTOM</code>,
     * <code>VerticalFlowLayout.CENTER</code>, <code>VerticalFlowLayout.LEADING</code>,
     * or <code>VerticalFlowLayout.TRAILING</code>.
     *
     * @param align the alignment value
     */
    public VerticalFlowLayout(int align) {
        this(align, 5, 5);
    }

    /**
     * Creates a new flow layout manager with the indicated alignment
     * and the indicated horizontal and vertical gaps.
     * <p/>
     * The value of the alignment argument must be one of
     * <code>VerticalFlowLayout.TOP</code>, <code>VerticalFlowLayout.BOTTOM</code>,
     * <code>VerticalFlowLayout.CENTER</code>, <code>VerticalFlowLayout.LEADING</code>,
     * or <code>VerticalFlowLayout.TRAILING</code>.
     *
     * @param align the alignment value
     * @param hgap  the horizontal gap between components
     *              and between the components and the
     *              borders of the <code>Container</code>
     * @param vgap  the vertical gap between components
     *              and between the components and the
     *              borders of the <code>Container</code>
     */
    public VerticalFlowLayout(int align, int hgap, int vgap) {
        this.hgap = hgap;
        this.vgap = vgap;
        setAlignment(align);
    }

    /**
     * Gets the alignment for this layout.
     * Possible values are <code>VerticalFlowLayout.TOP</code>,
     * <code>VerticalFlowLayout.BOTTOM</code>, <code>VerticalFlowLayout.CENTER</code>,
     * <code>VerticalFlowLayout.LEADING</code>,
     * or <code>VerticalFlowLayout.TRAILING</code>.
     *
     * @return the alignment value for this layout
     * @since JDK1.1
     */
    public int getAlignment() {
        return newAlign;
    }

    /**
     * Sets the alignment for this layout.
     * Possible values are
     * <ul>
     * <li><code>VerticalFlowLayout.TOP</code>
     * <li><code>VerticalFlowLayout.BOTTOM</code>
     * <li><code>VerticalFlowLayout.CENTER</code>
     * <li><code>VerticalFlowLayout.LEADING</code>
     * <li><code>VerticalFlowLayout.TRAILING</code>
     * </ul>
     *
     * @param align one of the alignment values shown above
     * @see #getAlignment()
     * @since JDK1.1
     */
    public void setAlignment(int align) {
        this.newAlign = align;

        // this.align is used only for serialization compatibility,
        // so set it to a value compatible with the 1.1 version
        // of the class

        switch (align) {
            case LEADING:
                this.align = TOP;
                break;
            case TRAILING:
                this.align = BOTTOM;
                break;
            default:
                this.align = align;
                break;
        }
    }

    /**
     * Gets the horizontal gap between components
     * and between the components and the borders
     * of the <code>Container</code>
     *
     * @return the horizontal gap between components
     * and between the components and the borders
     * of the <code>Container</code>
     */
    public int getHgap() {
        return hgap;
    }

    /**
     * Sets the horizontal gap between components and
     * between the components and the borders of the
     * <code>Container</code>.
     *
     * @param hgap the horizontal gap between components
     *             and between the components and the borders
     *             of the <code>Container</code>
     */
    public void setHgap(int hgap) {
        this.hgap = hgap;
    }

    /**
     * Gets the vertical gap between components and
     * between the components and the borders of the
     * <code>Container</code>.
     *
     * @return the vertical gap between components
     * and between the components and the borders
     * of the <code>Container</code>
     */
    public int getVgap() {
        return vgap;
    }

    /**
     * Sets the vertical gap between components and between
     * the components and the borders of the <code>Container</code>.
     *
     * @param vgap the vertical gap between components
     *             and between the components and the borders
     *             of the <code>Container</code>
     */
    public void setVgap(int vgap) {
        this.vgap = vgap;
    }

    /**
     * Adds the specified component to the layout.
     * Not used by this class.
     *
     * @param name the name of the component
     * @param comp the component to be added
     */
    public void addLayoutComponent(String name, Component comp) {
    }

    /**
     * Removes the specified component from the layout.
     * Not used by this class.
     *
     * @param comp the component to remove
     * @see java.awt.Container#removeAll
     */
    public void removeLayoutComponent(Component comp) {
    }

    /**
     * Returns the preferred dimensions for this layout given the
     * <i>visible</i> components in the specified target container.
     *
     * @param target the container that needs to be laid out
     * @return the preferred dimensions to lay out the
     * subcomponents of the specified container
     * @see Container
     * @see #minimumLayoutSize
     * @see java.awt.Container#getPreferredSize
     */
    public Dimension preferredLayoutSize(Container target) {
        synchronized (target.getTreeLock()) {
            Dimension dim = new Dimension(0, 0);
            int nmembers = target.getComponentCount();
            boolean firstVisibleComponent = true;

            for (int i = 0; i < nmembers; i++) {
                Component m = target.getComponent(i);
                if (m.isVisible()) {
                    Dimension d = m.getPreferredSize();
                    dim.width = Math.max(dim.width, d.width);
                    if (firstVisibleComponent) {
                        firstVisibleComponent = false;
                    } else {
                        dim.height += vgap;
                    }
                    dim.height += d.height;
                }
            }

            Insets insets = target.getInsets();
            dim.width += insets.left + insets.right + hgap * 2;
            dim.height += insets.top + insets.bottom + vgap * 2;
            return dim;
        }
    }

    /**
     * Returns the minimum dimensions needed to layout the <i>visible</i>
     * components contained in the specified target container.
     *
     * @param target the container that needs to be laid out
     * @return the minimum dimensions to lay out the
     * subcomponents of the specified container
     * @see #preferredLayoutSize
     * @see java.awt.Container
     * @see java.awt.Container#doLayout
     */
    public Dimension minimumLayoutSize(Container target) {
        synchronized (target.getTreeLock()) {
            Dimension dim = new Dimension(0, 0);
            int nmembers = target.getComponentCount();

            for (int i = 0; i < nmembers; i++) {
                Component m = target.getComponent(i);
                if (m.isVisible()) {
                    Dimension d = m.getMinimumSize();
                    dim.width = Math.max(dim.width, d.width);
                    if (i > 0) {
                        dim.height += vgap;
                    }
                    dim.height += d.height;
                }
            }
            Insets insets = target.getInsets();
            dim.width += insets.left + insets.right + hgap * 2;
            dim.height += insets.top + insets.bottom + vgap * 2;
            return dim;
        }
    }

    /**
     * Centers the elements in the specified row, if there is any slack.
     *
     * @param target   the component which needs to be moved
     * @param x        the x coordinate
     * @param y        the y coordinate
     * @param width    the width dimensions
     * @param height   the height dimensions
     * @param colStart the beginning of the row
     * @param colEnd   the the ending of the row
     */
    private void moveComponents(Container target, int x, int y, int width, int height,
                                int colStart, int colEnd, boolean ltr) {
        synchronized (target.getTreeLock()) {
            switch (newAlign) {
                case TOP:
                    y += ltr ? 0 : height;
                    break;
                case CENTER:
                    y += height / 2;
                    break;
                case BOTTOM:
                    y += ltr ? height : 0;
                    break;
                case LEADING:
                    break;
                case TRAILING:
                    y += height;
                    break;
            }
            for (int i = colStart; i < colEnd; i++) {
                Component m = target.getComponent(i);
                if (m.isVisible()) {
                    if (ltr) {
                        m.setLocation(x + (width - m.getWidth()) / 2, y);
                    } else {
                        m.setLocation(x + (width - m.getWidth()) / 2, target.getHeight() - y - m.getHeight());
                    }
                    y += m.getHeight() + vgap;
                }
            }
        }
    }

    /**
     * Lays out the container. This method lets each
     * <i>visible</i> component take
     * its preferred size by reshaping the components in the
     * target container in order to satisfy the alignment of
     * this <code>VerticalFlowLayout</code> object.
     *
     * @param target the specified component being laid out
     * @see Container
     * @see java.awt.Container#doLayout
     */
    public void layoutContainer(Container target) {
        synchronized (target.getTreeLock()) {
            Insets insets = target.getInsets();
            int maxwidth = target.getWidth() - (insets.left + insets.right + hgap * 2);
            int maxheight = target.getHeight() - (insets.top + insets.bottom + vgap * 2);
            int nmembers = target.getComponentCount();
            int x = insets.left + hgap, y = 0;
            int colw = 0, start = 0;

            boolean ltr = target.getComponentOrientation().isLeftToRight();

            for (int i = 0; i < nmembers; i++) {
                Component m = target.getComponent(i);
                if (m.isVisible()) {
                    Dimension d = m.getPreferredSize();
                    if (maximizeOtherDimension) {
                        d.width = maxwidth;
                    }
                    m.setSize(d.width, d.height);

                    if ((y == 0) || ((y + d.height) <= maxheight)) {
                        if (y > 0) {
                            y += vgap;
                        }
                        y += d.height;
                        colw = Math.max(colw, d.width);
                    } else {
                        moveComponents(target, insets.left + hgap, y, maxheight - x, colw, start, i, ltr);
                        moveComponents(target, x, insets.top + vgap, colw, maxheight - y, start, i, ltr);
                        y = d.height;
                        x += hgap + colw;
                        colw = d.width;
                        start = i;
                    }
                }
            }
            moveComponents(target, x, insets.top + vgap, colw, maxheight - y, start, nmembers, ltr);
        }
    }

    //
    // the internal serial version which says which version was written
    // - 0 (default) for versions before the Java 2 platform, v1.2
    // - 1 for version >= Java 2 platform v1.2, which includes "newAlign" field
    //
    private static final int currentSerialVersion = 1;
    /**
     * This represent the <code>currentSerialVersion</code>
     * which is bein used.  It will be one of two values :
     * <code>0</code> versions before Java 2 platform v1.2..
     * <code>1</code> versions after  Java 2 platform v1.2..
     *
     * @serial
     * @since 1.2
     */
    private int serialVersionOnStream = currentSerialVersion;

    /**
     * Reads this object out of a serialization stream, handling
     * objects written by older versions of the class that didn't contain all
     * of the fields we use now..
     */
    private void readObject(ObjectInputStream stream)
            throws IOException, ClassNotFoundException {
        stream.defaultReadObject();

        if (serialVersionOnStream < 1) {
            // "newAlign" field wasn't present, so use the old "align" field.
            setAlignment(this.align);
        }
        serialVersionOnStream = currentSerialVersion;
    }

    /**
     * Returns a string representation of this <code>VerticalFlowLayout</code>
     * object and its values.
     *
     * @return a string representation of this layout
     */
    public String toString() {
        String str = "";
        switch (align) {
            case TOP:
                str = ",align=top";
                break;
            case CENTER:
                str = ",align=center";
                break;
            case BOTTOM:
                str = ",align=bottom";
                break;
            case LEADING:
                str = ",align=leading";
                break;
            case TRAILING:
                str = ",align=trailing";
                break;
        }
        return getClass().getName() + "[hgap=" + hgap + ",vgap=" + vgap + str + "]";
    }


}