# Open work: integrated rusEFI troubleshooting assistant

Updated: 2026-10-08.

## 1. Extend read-only diagnostics

| Tool/capability | Open work |
| --- | --- |
| Lua, logging and sniffer evidence | Adapt implementations to the borrowed session and bound results |
| `export_diagnostic_case` | Save findings, referenced tune/log/capture evidence and provenance |

Keep the first release read-only. Tune writes, arbitrary commands and firmware
updates are later work, with concrete proposed changes shown before execution.

## 2. Finish delivery and provenance

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

## 3. Validate the complete user journey

Test a clean-machine installation with live ChatGPT and hardware:

- Launch and restore authorization.
- Connect to the ECU and resolve the matching INI.
- Prepare knowledge, collect evidence and answer with citations.
- Exercise missing, valid, stale and failed-download cache paths. Confirm an
  answer cites both a current ECU observation and a relevant source/wiki passage.
- Cancel operations and handle unplug/reconnect without mixing ECU evidence.
- Export a diagnostic case once that capability is available.

Acceptance scenario: "The engine cranks but won't start." Collect firmware
identity, relevant tune values, cranking voltage/RPM, synchronization evidence
and messages. Distinguish observed findings from hypotheses and state the next
measurement needed. Case export is required before this full scenario is
considered complete.
