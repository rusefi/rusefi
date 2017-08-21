package com.rusefi;

import com.rusefi.misc.ChangesModel;
import com.rusefi.pcb.PcbMergeTool;
import com.rusefi.pcb.PcbNode;

import java.io.IOException;

/**
 * @author Andrey Belomutskiy
 *         1/24/14
 */
public class PcbCopyTool {
    public static void main(String[] args) throws IOException {
        if (args.length != 2) {
            System.out.println("Two parameters expected: SOURCE DESTINATION");
            return;
        }

        ChangesModel.readConfiguration("pcb_merge_changes.txt");

        String input = args[0];
        String output = args[1];

        copy(input, output);
    }

    public static void copy(String input, String output) throws IOException {
        System.out.println("From " + input + " to " + output);
        PcbNode node = PcbNode.readFromFile(input);

        PcbMergeTool.removeNodes(node);

        node.write(output);
    }
}
