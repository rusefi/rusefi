package com.rusefi.util.test;

import com.rusefi.util.LazyFile;
import com.rusefi.util.LazyFileImpl;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;

import static org.junit.jupiter.api.Assertions.*;

public class LazyFileTest {
    @Test
    public void testUnifySpaces() {
        assertEquals("a\nb\nc", LazyFileImpl.unifySpaces("a\r\n\r\nb\n\n\nc"));
    }

    @Test
    public void testLazyFileDoesNotRewriteWhenContentUnchanged(@TempDir Path tempDir) throws IOException, InterruptedException {
        Path testFile = tempDir.resolve("TestFile.java");

        // Write initial content with a timestamp tag
        String initialContent = "package com.test;\n" +
            "// " + LazyFile.LAZY_FILE_TAG + "generator on Mon Jan 01 12:00:00 UTC 2024\n" +
            "\n" +
            "public class TestFile {\n" +
            "    public static void init() {\n" +
            "        System.out.println(\"test\");\n" +
            "    }\n" +
            "}";

        LazyFile writer1 = new LazyFileImpl(testFile.toString());
        writer1.write(initialContent);
        writer1.close();

        assertTrue(Files.exists(testFile), "File should be created");
        long firstModTime = Files.getLastModifiedTime(testFile).toMillis();
        String firstContent = Files.readString(testFile);

        // Write again with different timestamp but same actual content
        String secondContent = "package com.test;\n" +
            "// " + LazyFile.LAZY_FILE_TAG + "generator on Mon Jan 01 12:01:00 UTC 2024\n" +
            "\n" +
            "public class TestFile {\n" +
            "    public static void init() {\n" +
            "        System.out.println(\"test\");\n" +
            "    }\n" +
            "}";

        LazyFile writer2 = new LazyFileImpl(testFile.toString());
        writer2.write(secondContent);
        writer2.close();

        long secondModTime = Files.getLastModifiedTime(testFile).toMillis();
        String secondFileContent = Files.readString(testFile);

        // File should NOT have been modified despite different timestamp
        assertEquals(firstModTime, secondModTime, "File modification time should not change when only timestamp differs");
        assertEquals(firstContent, secondFileContent, "File content should remain unchanged");
    }

    @Test
    public void testLazyFileRewritesWhenContentChanged(@TempDir Path tempDir) throws IOException, InterruptedException {
        Path testFile = tempDir.resolve("TestFile.java");

        // Write initial content
        String initialContent = "package com.test;\n" +
            "// " + LazyFile.LAZY_FILE_TAG + "generator on Mon Jan 01 12:00:00 UTC 2024\n" +
            "\n" +
            "public class TestFile {\n" +
            "    public static void init() {\n" +
            "        System.out.println(\"test1\");\n" +
            "    }\n" +
            "}";

        LazyFile writer1 = new LazyFileImpl(testFile.toString());
        writer1.write(initialContent);
        writer1.close();

        long firstModTime = Files.getLastModifiedTime(testFile).toMillis();

        // Write with actual content change
        String changedContent = "package com.test;\n" +
            "// " + LazyFile.LAZY_FILE_TAG + "generator on Mon Jan 01 12:01:00 UTC 2024\n" +
            "\n" +
            "public class TestFile {\n" +
            "    public static void init() {\n" +
            "        System.out.println(\"test2\");\n" +
            "    }\n" +
            "}";

        LazyFile writer2 = new LazyFileImpl(testFile.toString());
        writer2.write(changedContent);
        writer2.close();

        long secondModTime = Files.getLastModifiedTime(testFile).toMillis();
        String finalContent = Files.readString(testFile);

        // File SHOULD have been modified when actual content changes
        assertTrue(secondModTime >= firstModTime, "File modification time should change when content differs");
        assertTrue(finalContent.contains("test2"), "File should contain the new content");
        assertFalse(finalContent.contains("test1"), "File should not contain the old content");
    }

    @Test
    public void testTrailingNewlineConsistency(@TempDir Path tempDir) throws IOException, InterruptedException {
        Path testFile = tempDir.resolve("TestFile.java");

        // Content without trailing newline in the source string
        String contentNoTrailingNewline = "package com.test;\n" +
            "// " + LazyFile.LAZY_FILE_TAG + "test\n" +
            "public class Test {}";

        // Write the file
        LazyFile writer1 = new LazyFileImpl(testFile.toString());
        writer1.write(contentNoTrailingNewline);
        writer1.close();

        // Verify by reading what was written and comparing
        String fileContent = Files.readString(testFile);

        // Write once more and verify it wasn't changed (content should match)
        long modTime = Files.getLastModifiedTime(testFile).toMillis();
        Thread.sleep(10);

        LazyFile writer4 = new LazyFileImpl(testFile.toString());
        writer4.write(contentNoTrailingNewline);
        writer4.close();

        long newModTime = Files.getLastModifiedTime(testFile).toMillis();
        String newFileContent = Files.readString(testFile);

        assertEquals(modTime, newModTime, "File should not be rewritten when content is identical");
        assertEquals(fileContent, newFileContent, "File content should remain stable across writes");
    }
}
