package com.rusefi.pcb;

import java.util.Collections;
import java.util.List;

/**
 * @author Andrey Belomutskiy
 *         1/21/14
 */
public class PointNode extends PcbNode {
    public final double x;
    public final double y;
    public final double angle;

    public PointNode(String nodeName, int i, List<Object> children) {
        super(nodeName, i, children);
        if (children.size() == 1) {
            // xyz use-case
            x = 0;
            y = 0;
            angle = 0;
            return;
        }

        if (children.size() != 2 && children.size() != 3)
            throw new IllegalStateException("Unexpected children count");
        x = Double.parseDouble((String) children.get(0));
        y = Double.parseDouble((String) children.get(1));
        angle = children.size() == 2 ? 0 : Double.parseDouble((String) children.get(2));
    }

    public PointNode(double x, double y) {
        super("", 0, Collections.emptyList());
        this.x = x;
        this.y = y;
        angle = 0;
    }

    public boolean isConnected(PointNode at, SizeNode size) {
        boolean isConnectedX = (x >= at.x - size.w / 2) && (x <= at.x + size.w / 2);
        boolean isConnectedY = (y >= at.y - size.h / 2) && (y <= at.y + size.h / 2);
        return isConnectedX && isConnectedY;
    }

    @Override
    public String toString() {
        return "PointNode{" +
                "x=" + x +
                ", y=" + y +
                ", angle=" + angle +
                '}';
    }

    public boolean isSameLocation(PointNode point) {
        return x == point.x && y == point.y;
    }

    public PointNode translate(PointNode at) {
        double nx = at.x - x;
        double ny = at.y - y;
        if (angle == 0)
            return new PointNode(nx, ny);
        if (angle == 270)
            return new PointNode(ny, -nx);
        if (angle == 90)
            return new PointNode(-ny, nx);
        if (angle == 180)
            return new PointNode(-nx, -ny);
        throw new IllegalStateException("Angle not supported: " + angle);
    }

    public void setLocation(double x, double y) {
        children.set(0, Double.toString(x));
        children.set(1, Double.toString(y));
    }
}
