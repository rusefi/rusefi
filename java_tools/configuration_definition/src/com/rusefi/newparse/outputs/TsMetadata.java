package com.rusefi.newparse.outputs;

import java.io.PrintStream;
import java.util.ArrayList;
import java.util.List;

public class TsMetadata {
    private List<String> comments = new ArrayList<>();
    public void addComment(String name, String comment) {
        if (comment == null) {
            return;
        }

        comment = comment.trim();
        if (!comment.startsWith("+")) {
            return;
        }

        // Clip off leading +, and any leading/trailing whitespace
        comment = comment.substring(1).trim();

        comments.add("\t" + name + " = \"" + comment + "\"");
    }

    public void writeComments(PrintStream ps) {
        this.comments.stream().forEach(ps::println);
    }
}
