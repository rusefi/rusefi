# Open work: integrated rusEFI troubleshooting assistant

Updated: 2026-10-08.

Keep ECU access in the first release read-only. Local diagnostic case export
is implemented; live acceptance remains below. Tune writes, arbitrary commands
and firmware updates are later work, with concrete proposed changes shown before
execution.

## 1. Verify updated hosted delivery

- After the manifest changes land, verify a hosted archive workflow run and
  download the manifest-enabled published ZIP through `FirmwareSourceCodeDownloader`.
  Confirm its revisions, licenses and payload hashes survive preparation and case export.

The existing hosted workflow/published ZIP, local manifest-enabled archive and
normal universal Console bundle were verified on 2026-10-08. Distribution uses
the normal Console runtime/native bundle with source fetched on demand; users
need no Git, Python or Gradle. Revision metadata, file/payload hashes, technical
guides, index and licenses are implemented. ECU/source compatibility stays
unverified because the current ECU signature does not establish a Git revision.
See [delivery details](llm-troubleshooting.md#distribution-and-archive-contents).

## 2. Validate the complete user journey

Test a clean-machine installation with live ChatGPT and hardware:

- Launch and restore authorization.
- Connect to the ECU and resolve the matching INI.
- Prepare knowledge, collect evidence and answer with citations.
- Exercise missing, valid, stale and failed-download cache paths. Confirm an
  answer cites both a current ECU observation and a relevant source/wiki passage.
- Cancel operations and handle unplug/reconnect without mixing ECU evidence.
- Export a diagnostic case and inspect its findings, referenced evidence and
  provenance outside the Console.

Acceptance scenario: "The engine cranks but won't start." Collect firmware
identity, relevant tune values, cranking voltage/RPM, synchronization evidence
and messages. Distinguish observed findings from hypotheses and state the next
measurement needed. Case export is required before this full scenario is
considered complete.
