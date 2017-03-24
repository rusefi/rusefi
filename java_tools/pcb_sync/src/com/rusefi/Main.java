package com.rusefi;

import com.rusefi.bom.BomBuilder;
import com.rusefi.misc.CmpMergeTool;
import com.rusefi.misc.RemoveUnneededTraces;
import com.rusefi.pcb.PcbMergeTool;
import com.rusefi.pcb.PcbMoveTool;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.Arrays;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

/**
 * (c) Andrey Belomutskiy
 * 12/16/13.
 */
public class Main {
    private static final Map<String, Class> TOOLS = new TreeMap<String, Class>(String.CASE_INSENSITIVE_ORDER);

    static {
        registerTool("pcb_move", PcbMoveTool.class);
        registerTool("pcb_merge", PcbMergeTool.class);

        registerTool("copy", PcbCopyTool.class);

        registerTool("cmp_merge", CmpMergeTool.class);
        registerTool("rename", RenameComponents.class);

        registerTool("optimize", RemoveUnneededTraces.class);

        registerTool("bom_builder", BomBuilder.class);
    }

    private static void registerTool(String name, Class<?> clazz) {
        TOOLS.put(name, clazz);
    }

    public static void main(String[] args) throws NoSuchMethodException, InvocationTargetException, IllegalAccessException {
        if (args.length < 1) {
            System.out.println("At least tool name parameter expected");
            System.out.println(TOOLS.size() + " tools available:");
            for (String name : TOOLS.keySet())
                System.out.println("\t\t" + name);
            return;
        }
        String name = args[0];
        Class c = TOOLS.get(name);
        if (c == null)
            throw new NullPointerException("No tool: " + name);

        Method main = c.getMethod("main", String[].class);

        List<String> asList = Arrays.asList(args);
        String[] a = asList.subList(1, asList.size()).toArray(new String[args.length - 1]);

        main.invoke(null, new Object[]{a});
    }
}
