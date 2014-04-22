package com.rusefi.pcb;

import java.util.List;

/**
 * (c) Andrey Belomutskiy
 * 1/21/14.
 */
public class CirclePadNode extends PadNode {
    public CirclePadNode(String nodeName, int i, List<Object> children) {
        super(nodeName, i, children);
    }

    @Override
    public String toString() {
        return "CirclePadNode{" +
                "at=" + at +
                ", size=" + size +
                '}';
    }

}
