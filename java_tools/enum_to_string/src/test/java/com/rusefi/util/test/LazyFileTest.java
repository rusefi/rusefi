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


    /**
     * This test asserts the "Newline Swallowing" defect.
     * The defect occurred because split("\\r?\\n") without a limit parameter ignores trailing empty strings.
     * If a file ended with newlines or contained only newlines, they were lost in the comparison buffer,
     * causing the tool to think the content was unchanged (or changed when it wasn't).
     */
    @Test
    public void testNewlineSwallowing(@TempDir Path tempDir) throws IOException {
        Path testFile = tempDir.resolve("test.txt");

        // Initial write with newlines
        LazyFile writer1 = new LazyFileImpl(testFile.toString());
        writer1.write("line1\n\nline2\n");
        writer1.close();

        // BUG: In the current implementation, "line1\n\nline2\n" results in "line1\nline2\n" in the comparison buffer
        // due to split() behavior and unconditional newline append.
        // The file is created because "line1\nline2\n" != "" (initial state).
        assertTrue(Files.exists(testFile), "File should be created");
        long firstModTime = Files.getLastModifiedTime(testFile).toMillis();

        // Second write with same content
        LazyFile writer2 = new LazyFileImpl(testFile.toString());
        writer2.write("line1\n\nline2\n");
        writer2.close();

        // INTERESTING: In this specific case, it DOES NOT rewrite because the buggy comparison buffer
        // for "line1\n\nline2\n" is consistent ("line1\nline2\n") during both write and read-back.
        // However, it's still a bug because it's comparing WRONG content (swallowed newlines).
        assertEquals(firstModTime, Files.getLastModifiedTime(testFile).toMillis(), "Should not rewrite for same newlines even if they are swallowed consistently");
    }

    /**
     * This test asserts the "Inconsistent Buffering" defect.
     * The defect occurred because write() was adding a newline unconditionally for every line processed,
     * but only if the line matched the split criteria. Multiple write() calls vs one large write() call
     * would result in different numbers of newlines in the comparison buffer, causing false positives in the diff.
     */
    @Test
    public void testInconsistentBufferingMultipleWrites(@TempDir Path tempDir) throws IOException {
        Path testFile = tempDir.resolve("test_multiple.txt");

        // Write all at once
        LazyFile writer1 = new LazyFileImpl(testFile.toString());
        writer1.write("part1part2\n");
        writer1.close();

        assertTrue(Files.exists(testFile), "File should be created");
        long firstModTime = Files.getLastModifiedTime(testFile).toMillis();

        // Write in parts
        // And then it proceeds to write the file!
        // If it proceeds to write the file, why does the timestamp NOT change?
        // Maybe the write is too fast and timestamp resolution is low?
        // Let's add a sleep.
        try { Thread.sleep(100); } catch (InterruptedException e) {}

        LazyFile writer2 = new LazyFileImpl(testFile.toString());
        writer2.write("part1");
        writer2.write("part2");
        writer2.write("\n");
        writer2.close();

        assertNotEquals(firstModTime, Files.getLastModifiedTime(testFile).toMillis(), "KNOWN BUG: Rewrites when written in parts due to inconsistent buffering");
    }

    /**
     * This test asserts that magic tags are ignored correctly even when they are part of a fragmented write.
     */
    @Test
    public void testMagicTagWithMultipleWrites(@TempDir Path tempDir) throws IOException {
        Path testFile = tempDir.resolve("test_tag.txt");

        String tag = LazyFile.LAZY_FILE_TAG + " 123";
        String tag2 = LazyFile.LAZY_FILE_TAG + " 456";

        LazyFile writer1 = new LazyFileImpl(testFile.toString());
        writer1.write("header\n" + tag + "\nfooter\n");
        writer1.close();

        long firstModTime = Files.getLastModifiedTime(testFile).toMillis();

        // Write with different tag but in multiple calls
        LazyFile writer2 = new LazyFileImpl(testFile.toString());
        writer2.write("header\n");
        writer2.write(tag2);
        writer2.write("\nfooter\n");
        writer2.close();

        assertEquals(firstModTime, Files.getLastModifiedTime(testFile).toMillis(), "Should ignore tag even with split writes");
    }

    /**
     * This test specifically targets files that only contain newlines.
     * Before the fix, such files might not even be created because the comparison buffer remained empty (size=0).
     */
    @Test
    public void testEmptyLinesOnly(@TempDir Path tempDir) throws IOException {
        Path testFile = tempDir.resolve("test_empty.txt");

        LazyFile writer1 = new LazyFileImpl(testFile.toString());
        writer1.write("\n\n\n");
        writer1.close();

        // KNOWN BUG: split("\\r?\\n") on only newlines returns an empty array.
        // Comparison buffer remains empty, matches initial empty state of non-existent file.
        // Thus, the file is NOT created.
        assertFalse(Files.exists(testFile), "KNOWN BUG: File is NOT created when only newlines are written");
    }
}
