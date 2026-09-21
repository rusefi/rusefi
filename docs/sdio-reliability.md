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

The HAL changes target `rusefi/ChibiOS:stable_21.11.x.rusefi_clean_history`:

1. [ChibiOS #75](https://github.com/rusefi/ChibiOS/pull/75): CCM-safe transfers.
2. [ChibiOS #76](https://github.com/rusefi/ChibiOS/pull/76): bounded waits and
   recovery. This branch also contains the first branch's commits.

The submodule URL remains `https://github.com/rusefi/ChibiOS.git`, as on the
firmware's master branch. The integration currently selects the tested head
of ChibiOS #76, `cc07ec71fa237c3db7c0950a22931f2606b53b9b`. Both HAL pull
requests are still pending as of 2026-09-21. Fetching this commit through the
official URL does not mean the HAL changes have been accepted upstream.

Before merging this integration, merge the HAL changes and select the accepted
upstream commit. Repeat the host tests and firmware CI against that revision.

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
