package com.rusefi.pcb;

import java.util.List;

/**
 * (c) Andrey Belomutskiy
 * 1/21/14.
 */
public class RectPadNode extends PadNode {
    public RectPadNode(String nodeName, int i, List<Object> children) {
        super(nodeName, i, children);
    }

    @Override
    public String toString() {
        return "RectPadNode{" +
                "name=" + name +
                ", at=" + at +
                ", size=" + size +
                '}';
    }
}
