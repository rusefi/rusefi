package rusefi;

import java.io.*;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import static rusefi.NetListMergeUtil.validateSection;

/**
 * 12/8/13
 * (c) Andrey Belomutskiy
 */
public class NetListMerge {
    private static final Pattern GET_HEADER = Pattern.compile("(.*)\\(components(.*)\\s*$", Pattern.DOTALL);

    private static final Pattern GET_COMPONENTS = Pattern.compile("(.*)\\)\\s*\\(libparts(.*)\\)\\)\\s*$", Pattern.DOTALL);

    private static final Pattern GET_LIBRARTS = Pattern.compile("(.*)\\)\\s*\\(libraries(.*)", Pattern.DOTALL);
    private static final Pattern GET_LIBRARIES = Pattern.compile("(.*)\\)\\s*\\(nets(.*)", Pattern.DOTALL);


    private NetListMerge() {
    }

    public static void main(String[] args) throws IOException {
        if (args.length < 2) {
            System.out.println("This tool takes 'components' sections of multiple KiCad .net files and merges them into one");
            System.out.println("usage:");
            System.out.println("NetListMerge DEST_FILE SOURCE_1 SOURCE_2 [SOURCE_x]*");
            return;
        }

        String destinationFile = args[0];
        NetContent destination = parse(NetListMergeUtil.readFile(destinationFile));

        StringBuilder components = new StringBuilder();
        StringBuilder libparts = new StringBuilder();
        StringBuilder libraries = new StringBuilder();
        StringBuilder nets = new StringBuilder();
        for (int i = 1; i < args.length; i++) {
            String sourceFile = args[i];
            NetContent source = parse(NetListMergeUtil.readFile(sourceFile));
            components.append(source.components);
            libparts.append(source.libparts);
            libraries.append(source.libraries);
            nets.append(source.nets);
        }

        System.out.println("Total " + components.length() + " bytes of components");
        System.out.println("Total " + libparts.length() + " bytes of libparts");
        System.out.println("Total " + libraries.length() + " bytes of libraries");
        System.out.println("Total " + nets.length() + " bytes of nets");

        StringBuilder result = new StringBuilder(destination.header);
        result.append("  (components").append(components).append(")\r\n");
        result.append("  (libparts").append(libparts).append(")\n");
        result.append("  (libraries").append(libraries).append(")\n");
        result.append("  (nets").append(nets).append(")");
        result.append(")");
        validateSection(result.toString());
        NetListMergeUtil.writeResult(result.toString(), "out.txt");
    }

    private static class NetContent {
        private final String header;
        private final String components;
        private final String libparts;
        private final String nets;
        private final String libraries;

        private NetContent(String header, String components, String libparts, String libraries, String nets) {
            this.header = header;
            this.components = components;
            this.libparts = libparts;
            this.nets = nets;
            this.libraries = libraries;
            System.out.println("Got " + components.length() + " bytes of components");
            System.out.println("Got " + libparts.length() + " bytes of libparts");
            System.out.println("Got " + libraries.length() + " bytes of libraries");
            System.out.println("Got " + nets.length() + " bytes of nets");
        }
    }

    private static NetContent parse(String content) {
        Matcher headerMatcher = GET_HEADER.matcher(content);
        headerMatcher.find();
        if (!headerMatcher.matches())
            throw new IllegalStateException("Does not match expected patten for header");

        String header = headerMatcher.group(1);
        String nonHeader = headerMatcher.group(2);

        Matcher componentsMatcher = GET_COMPONENTS.matcher(nonHeader);
        if (!componentsMatcher.matches())
            throw new IllegalStateException("Does not match expected patten for components");

        String components = componentsMatcher.group(1);
        String nonComponents = componentsMatcher.group(2);
        validateSection(components);

        Matcher libpartsMatcher = GET_LIBRARTS.matcher(nonComponents);
        if (!libpartsMatcher.matches())
            throw new IllegalStateException("Does not match expected patten for libparts");

        String libparts = libpartsMatcher.group(1);
        String nonlibparts = libpartsMatcher.group(2);
        validateSection(libparts);

        Matcher librariesMatcher = GET_LIBRARIES.matcher(nonlibparts);
        if (!librariesMatcher.matches())
            throw new IllegalStateException("Does not match expected patten for nets");

        String libraries = librariesMatcher.group(1);
        String nets = librariesMatcher.group(2);
        validateSection(libraries);
        validateSection(nets);

//        System.out.println("components: " + components);

        return new NetContent(header, components, libparts, libraries, nets);
    }

}
