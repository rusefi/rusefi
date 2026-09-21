# SDIO reliability changes

The HAL changes are maintained in two branches, based on the ChibiOS revision
already selected by the firmware:

1. `codex/sdio-ccm-buffer`: keep DMA away from STM32F4 CCM by copying sectors
   through the existing SRAM buffer. Aligned SRAM keeps direct multi-block I/O.
2. `codex/sdio-bounded-waits`: builds on the first branch and adds bounded
   SDIO interrupt, DMA and command waits, plus safe error cleanup and an optional
   card-busy deadline.

Each branch contains a passing bad-behavior reproduction followed by the fix
and corrected expectations. The firmware integration branch is
`codex/sdio-reliability`.

## Reviewing and upstreaming

The two HAL branches are in
[Matheusschmitz/ChibiOS](https://github.com/Matheusschmitz/ChibiOS).
Review the first against `rusefi/ChibiOS:stable_21.11.x.rusefi_clean_history`,
and the second against the first branch until its prerequisite is merged.
The second branch contains the first branch's commits.

This integration temporarily selects the fork's URL in `.gitmodules`, so a
fresh checkout can fetch the exact tested revision without relying on GitHub
serving an unmerged fork commit through the upstream repository.

Before submitting the firmware integration upstream, merge the HAL changes,
restore `https://github.com/rusefi/ChibiOS.git` in `.gitmodules`, and select the
accepted upstream commit. Repeat the host tests and firmware CI against that
commit. No pull requests are created by these branches or their workflows.

## Validation

```sh
git submodule update --init firmware/ChibiOS
CC=gcc python -m unittest discover -s firmware/ChibiOS/testhal/host/sdio -p 'test_*.py' -v
```

The dedicated workflow runs this suite with Linux GCC/Clang, macOS Clang and
Windows GCC/MSVC. The local suite has 17 tests covering data copies, partial
failures, invalid ranges, error flags, missing completions, clock wraparound
and cleanup ordering. The generic STM32F4 SDC HAL example also builds with
ARM GCC 12.3.1. Neither substitutes for real interrupt/DMA timing tests.

## Compatibility

There are no calibration-layout or protocol changes. No additional full-page
buffer is allocated. The SDC driver object itself must remain in DMA-accessible
SRAM. SDMMCv1/v2 drivers are outside the scope of these changes.

`SDC_WAIT_FOR_TRANSFER_TIMEOUT_MS` defaults to zero to preserve the existing
policy for long card erase operations. Enabling it is an application decision;
the host regression uses 1000 ms. The other software deadlines are per operation,
not a fixed maximum for a complete file save. A physically stuck DMA stream
halts instead of releasing memory that the peripheral may still access.

The completion ordering includes the SDIO portion of Giovanni Di Sirio's
[upstream reliability repair](https://github.com/ChibiOS/ChibiOS/commit/6c8f03904b149b9a136152250420a9032e711794),
with additional checks and timeout handling; the complete upstream commit was
not imported.
