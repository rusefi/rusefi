# L9779 rebase review - 2026-09-22

Decision: use the driver from master `1ea061b5d5c`, retaining the m74_9
startup requirements from the original branch `3e7fd52dcee`. Keep the
branch's board configuration and AT32 interrupt priorities. The review
compares the final implementations, not individual development commits.

## Differences that determine the choice

| Area | Original branch | Master | Decision |
| --- | --- | --- | --- |
| SPI parity | Array and ISR writes OR in parity without first clearing bit 0; a previously set parity bit can stay wrong | Every path uses `l9779PrepareSpiWord`, which clears and recomputes parity | Master |
| SPI reply ownership | Single-frame thread transfer clears `spi_busy` before validating the reply and updating its queue; the watchdog ISR can interrupt that bookkeeping | Keeps exclusion through validation, queue insertion and logging | Master |
| Pipelined reads | Enqueues the new request before validating the previous reply, permitting a stale reply to consume the new request | Validates the received reply before recording the new request | Master |
| Injector/coil edges | Calls `wake_driver()` even after a direct GPIO write | Direct GPIO writes return without waking the SPI thread; permanent SPI enables still come from the output-enable mask | Master |
| `pins` diagnostics | Issues SPI reads of clear-on-read fault registers and resets the shared DWT counter for clock measurements | Reports cached diagnostics and the SPI frame log without diagnostic SPI transactions or counter resets | Master |
| Diagnostic scheduling | Predicts the next watchdog deadline from the end of the previous feed, although TIM7 is free-running; can replenish a refresh before the previous batch finishes | Uses TIM7 CNT/ARR directly and starts a refresh only when no previous refresh is pending | Master |
| Unsupported output diagnostics | Returns `PIN_OK` where the pin has no diagnostic field | Returns `PIN_UNKNOWN` | Master |
| Driver shutdown | Leaves TIM7 feeding | Stops the watchdog timer | Master |
| Chip-select startup | Explicitly configures the GPIO as an output | Omits that setup; the SPI HAL only toggles the pad | Branch requirement; set the idle-high latch before enabling the output |
| First reset reply | Allows startup despite a bad initial pipelined reply | Can fail initialization on that pre-reset reply | Branch requirement; later reads validate the link, and the board waits for a valid KEY_ON reading |
| Realtime counter startup | Explicitly enables DWT, but also resets the shared counter | Uses ChibiOS realtime-counter delays | Keep explicit enable for AT32 port variants, without resetting the counter |

The final `l9779.cpp` differs from master only in startup: chip-select
configuration, enabling DWT without resetting it, and tolerating the first
reset reply. The public header matches master; its counter API remains
compatible with the branch's TLE9201 diagnostic caller.

## Behavior retained

- TIM7 feeds the watchdog at priority 5, below the priority-3 executor and
  angle-clock timers. `rccEnableTIM7(true)` keeps the timer clock active
  during CPU sleep. Reload changes still issue EGR/UG; unchanged periods
  leave the timer free-running.
- The watchdog still uses RESPTIME=10, an initial 27 ms answer period,
  bounded adaptation, content-addressed read matching, and a contiguous
  four-byte answer. The feed does not reset the L9779 to recover a high
  watchdog error count.
- Full-adaptive VRS register values and permanent SPI enables for directly
  driven outputs match the branch. Output packing and watchdog data now
  use the same helpers covered by master's tests.
- The branch's board wiring, TLE9201 initialization, PB13 ignition gate,
  KEY_ON configuration, flash-write guards, CAN/LIN behavior and additional
  angle-clock priority definitions remain intact.
- Debug counters, cached faults, timer registers and recent SPI frames
  remain available. The branch's additional liveness/event history and
  clock cross-measurement routines are not retained. No firmware thread
  wakeups are needed merely to service direct injector/coil edges.

## Shared gaps that this selection does not resolve

These exist in both source versions and require separate fault-injection
coverage before changing their recovery policy:

1. The driver marks a requested power-stage transition as applied before
   checking whether the PSOFF write succeeds. A failed power-off write is
   logged, but that transition is not retried until the request changes.
2. The driver clears `need_init` and ignores the results of `chip_reset`,
   `chip_init` and the initial `update_output`, then starts feeding. This
   can leave incomplete configuration without an explicit retry path.

The retained implementation is the better-supported merge choice, not a
claim that hardware failure recovery is complete.

## Validation and limits

- All 17 existing tests in `test_at32_port.cpp` pass when compiled directly
  with GCC, C++17, `-Wall -Wextra -Werror`, and the branch's Google Test
  revision. They cover IRQ constants, reset decoding, SPI parity over all
  65,536 input words, read tracking, frame logging, output/diagnostic
  mapping, VRS values and watchdog helpers. They do not execute the actual
  driver's HAL/thread/ISR paths.
- Compared the TLE9201 counter call against the selected API and checked
  the branch's TIM7 ownership, interrupt priorities and ignition hooks.
- An ARM syntax-check attempt using the board's generated Makefile command
  stops at the missing `rusefi_generated_m74_9.h`; it does not establish a
  successful production compilation.
- Full firmware validation is blocked by three branch submodule revisions
  unavailable locally and rejected by their configured remotes:
  ChibiOS `33f0f243a1461d93c1049dedb3b2f8b2611035a6`, OpenBLT
  `dd3dd7ef3ff849c04b91195b6cbbded02bcbc2d0`, and libfirmware
  `a7ccb41e500d5ded7282bff30d54b2e7334604cd`. Their recorded gitlinks are
  preserved. The current local ChibiOS source confirms that SPI select
  toggles the pad without configuring its mode; the unavailable branch
  revision cannot be inspected.
- No hardware was flashed. Once dependencies are available, validate a
  board build, cold start, key-off/key-on, watchdog recovery and `pins`
  while outputs are active.
