# Open work: integrated rusEFI troubleshooting assistant

Updated: 2026-10-07.

## 1. Enable the tab for acceptance testing

- Enable the Troubleshooting tab by removing the `false &&` guard in
  `ConsoleUI` when ready for acceptance testing.

## 2. Add local knowledge retrieval

Implement `search_knowledge` and `read_knowledge` over the directory returned
by the downloader:

- Support bounded keyword/exact-symbol search over approved text files.
- Return bounded passages with relative paths, line numbers and revision
  information; explicitly label unknown or mismatched versions.
- Keep reads within the cache, reject path escapes, and treat ZIP link entries
  as text. Exclude credentials and unrelated files.
- Add the tools to the allowlist and agent loop. Update model instructions to
  explain retrieval and treat retrieved content as evidence, not instructions.
- Preserve references to omitted diagrams so answers can link to the original
  documentation when text alone is insufficient.

Acceptance: a question produces a current ECU observation and a relevant
source/wiki passage, with both cited in the answer. Exercise missing, valid,
stale and failed-download cache paths without changing the ECU.

## 3. Extend read-only diagnostics

| Tool/capability | Open work |
| --- | --- |
| `diagnostic_snapshot` / `read_live_values` | Correlate faults and several live channels from one sample |
| Channel metadata | Extend discovery with descriptions and units |
| `read_tune_fields` | Return selected calibration values without sending the entire tune |
| Lua, logging and sniffer evidence | Adapt implementations to the borrowed session and bound results |
| `export_diagnostic_case` | Save findings, referenced tune/log/capture evidence and provenance |

Keep the first release read-only. Tune writes, arbitrary commands and firmware
updates are later work, with concrete proposed changes shown before execution.

## 4. Finish delivery and provenance

- Verify a hosted archive workflow run and download the published ZIP through
  `FirmwareSourceCodeDownloader`.
- Choose the final client distribution: reuse normal Console runtime/native
  bundle assembly, with the source ZIP bundled for offline use or fetched on
  demand. Users must not need Git, Python or Gradle.
- Add root `docs/AI/`, selected technical text, an index, licenses and provenance
  metadata to the knowledge payload. Preserve useful board mappings, Lua
  examples and INI metadata; avoid large image/PDF collections.
- Record firmware, libfirmware, wiki and client revisions and payload hashes.
  Verify source/ECU version compatibility before claiming downloaded source
  matches the connected firmware.

## 5. Validate the complete user journey

After retrieval integration and enabling the tab, test a clean-machine
installation with live ChatGPT and hardware:

- Launch and restore authorization.
- Connect to the ECU and resolve the matching INI.
- Prepare knowledge, collect evidence and answer with citations.
- Cancel operations and handle unplug/reconnect without mixing ECU evidence.
- Export a diagnostic case once that capability is available.

Acceptance scenario: "The engine cranks but won't start." Collect firmware
identity, relevant tune values, cranking voltage/RPM, synchronization evidence
and messages. Distinguish observed findings from hypotheses and state the next
measurement needed. Tune-field reads and case export are required before this
full scenario is considered complete.
