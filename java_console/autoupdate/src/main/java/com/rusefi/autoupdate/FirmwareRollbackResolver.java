package com.rusefi.autoupdate;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;
import java.nio.charset.StandardCharsets;
import java.time.LocalDateTime;
import java.time.ZoneOffset;
import java.time.format.DateTimeFormatter;
import java.time.format.DateTimeParseException;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;
import java.util.Objects;
import java.util.Optional;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public final class FirmwareRollbackResolver {
    private static final int TIMEOUT_MILLIS = 10_000;
    private static final Pattern SAFE_SEGMENT = Pattern.compile("[A-Za-z0-9._-]+");
    private static final Pattern BUILD_LINK = Pattern.compile(
        "<a\\s+href=[\"']([0-9a-f]{40})/[\"'][^>]*>[^\\r\\n]*?</a>\\s+" +
            "(\\d{4}-\\d{2}-\\d{2}\\s+\\d{2}:\\d{2})",
        Pattern.CASE_INSENSITIVE);
    private static final DateTimeFormatter APACHE_TIMESTAMP = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm");
    private static final Comparator<Build> NEWEST_FIRST = Comparator.comparingLong(Build::getLastModified)
        .reversed()
        .thenComparing(Build::getSha);

    private final String updateRoot;

    public FirmwareRollbackResolver(String updateRoot) {
        this.updateRoot = withTrailingSlash(Objects.requireNonNull(updateRoot, "updateRoot"));
    }

    public List<Build> discover(String board, String ltsBranch) throws IOException {
        String directoryUrl = historyUrl(board, ltsBranch);
        HttpURLConnection connection = (HttpURLConnection) new URL(directoryUrl).openConnection();
        connection.setConnectTimeout(TIMEOUT_MILLIS);
        connection.setReadTimeout(TIMEOUT_MILLIS);
        connection.setRequestProperty("User-Agent", "rusEFI-console");

        try {
            int responseCode = connection.getResponseCode();
            if (responseCode < 200 || responseCode >= 300) {
                throw new IOException("Firmware index returned HTTP " + responseCode + " for " + directoryUrl);
            }
            try (InputStream input = connection.getInputStream()) {
                return parseIndex(readAll(input), board, ltsBranch, directoryUrl);
            }
        } finally {
            connection.disconnect();
        }
    }

    String historyUrl(String board, String ltsBranch) {
        requireSafeSegment(board, "board");
        requireSafeSegment(ltsBranch, "ltsBranch");
        return updateRoot + board + "/" + ltsBranch + "/";
    }

    public Optional<Build> latest(List<Build> builds) {
        return sorted(builds).stream().findFirst();
    }

    public Optional<Build> previous(List<Build> builds, String currentSha) {
        List<Build> candidates = rollbackCandidates(builds, currentSha);
        return currentSha != null && contains(builds, currentSha) && !candidates.isEmpty()
            ? Optional.of(candidates.get(0))
            : Optional.empty();
    }

    public List<Build> rollbackCandidates(List<Build> builds, String currentSha) {
        List<Build> sorted = sorted(builds);
        if (sorted.isEmpty()) {
            return new ArrayList<>();
        }
        if (currentSha != null) {
            for (int i = 0; i < sorted.size(); i++) {
                if (sorted.get(i).getSha().equalsIgnoreCase(currentSha)) {
                    return new ArrayList<>(sorted.subList(i + 1, sorted.size()));
                }
            }
        }
        return sorted;
    }

    static List<Build> parseIndex(String html, String board, String branch, String directoryUrl) {
        requireSafeSegment(board, "board");
        requireSafeSegment(branch, "branch");
        String baseUrl = withTrailingSlash(directoryUrl);
        List<Build> builds = new ArrayList<>();
        Matcher matcher = BUILD_LINK.matcher(html);
        while (matcher.find()) {
            try {
                String sha = matcher.group(1).toLowerCase();
                long lastModified = LocalDateTime.parse(matcher.group(2), APACHE_TIMESTAMP)
                    .toInstant(ZoneOffset.UTC)
                    .toEpochMilli();
                builds.add(new Build(board, branch, sha, lastModified, baseUrl + sha + "/"));
            } catch (DateTimeParseException ignored) {
                // Ignore malformed index rows; a valid row still remains usable.
            }
        }
        builds.sort(NEWEST_FIRST);
        return builds;
    }

    private static List<Build> sorted(List<Build> builds) {
        List<Build> result = new ArrayList<>(Objects.requireNonNull(builds, "builds"));
        result.sort(NEWEST_FIRST);
        return result;
    }

    private static boolean contains(List<Build> builds, String sha) {
        return builds.stream().anyMatch(build -> build.getSha().equalsIgnoreCase(sha));
    }

    private static String readAll(InputStream input) throws IOException {
        StringBuilder result = new StringBuilder();
        try (BufferedReader reader = new BufferedReader(new InputStreamReader(input, StandardCharsets.UTF_8))) {
            String line;
            while ((line = reader.readLine()) != null) {
                result.append(line).append('\n');
            }
        }
        return result.toString();
    }

    private static void requireSafeSegment(String value, String name) {
        if (value == null || !SAFE_SEGMENT.matcher(value).matches()) {
            throw new IllegalArgumentException("Invalid " + name + ": " + value);
        }
    }

    private static String withTrailingSlash(String value) {
        return value.endsWith("/") ? value : value + "/";
    }

    public static final class Build {
        private final String board;
        private final String branch;
        private final String sha;
        private final long lastModified;
        private final String directoryUrl;

        private Build(String board, String branch, String sha, long lastModified, String directoryUrl) {
            this.board = board;
            this.branch = branch;
            this.sha = sha;
            this.lastModified = lastModified;
            this.directoryUrl = directoryUrl;
        }

        public String getBoard() {
            return board;
        }

        public String getBranch() {
            return branch;
        }

        public String getSha() {
            return sha;
        }

        public long getLastModified() {
            return lastModified;
        }

        public String getDirectoryUrl() {
            return directoryUrl;
        }
    }
}
