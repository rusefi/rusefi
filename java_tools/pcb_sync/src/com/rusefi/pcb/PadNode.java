package com.rusefi.pcb;

import java.util.List;

/**
 * (c) Andrey Belomutskiy
 * 1/21/14.
 */
public abstract class PadNode extends PcbNode {
    protected final PointNode at;
    protected final SizeNode size;
    protected final String name;

    public PadNode(String nodeName, int i, List<Object> children) {
        super(nodeName, i, children);
        name = (String) children.get(0);
        at = (PointNode) find("at");
        size = (SizeNode) find("size");
    }

    @Override
    public boolean isConnected(PointNode point) {
        return point.isConnected(at, size);
    }

    public static PcbNode parse(String nodeName, int i, List<Object> children) {
        Object shape = children.get(2);
        if ("rect".equals(shape))
            return new RectPadNode(nodeName, i, children);
        if ("circle".equals(shape))
            return new CirclePadNode(nodeName, i, children);
        if ("oval".equals(shape))
            return new CirclePadNode(nodeName, i, children); // yes, let's treat oval as circle. good enough
        throw new IllegalStateException("Unknown pad shape: " + shape.toString());
    }

    @Override
    public String toString() {
        return "PadNode{" +
                "at=" + at +
                ", size=" + size +
                '}';
    }
}
