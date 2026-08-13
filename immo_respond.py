#!/usr/bin/env python3
"""
immo_respond.py — auto-respond to ECU 0x0713 IMMO trigger with 0x0714 challenge.

Usage:
    pip install python-can
    python3 immo_respond.py [channel]

Default channel: PCAN_USBBUS1  (change to 'PCAN_USBBUS2' etc. if needed)

How it works:
  - Listens on CAN bus
  - When 0x0713 arrives (ECU IMMO trigger), sends back Frame1 + Frame2 within ~1ms
  - Repeats every time 0x0713 is seen
  - All events printed with timestamps for correlation with OpenOCD

Challenge data used:
  Frame1+Frame2 from orig_1.trc Pair 1 (known-good from original ECU+BCM).
  Content does not need to be cryptographically correct for the purpose of
  hitting the SLib breakpoint — any 16 bytes that pass CAN ISR basic checks work.
"""

import can
import time
import sys

CHANNEL = sys.argv[1] if len(sys.argv) > 1 else 'PCAN_USBBUS1'
BITRATE = 500000

# Known challenge data from orig_1.trc pair 1.
# trigger=5283dba64769c00d -> F1=660be1e2a34b8140 F2=b45633a0499a01ec
# You can also try: F1=66aaeef37037dee0 F2=cdeefb22bde96807 (pair 5)
FRAME1 = bytes.fromhex('660be1e2a34b8140')
FRAME2 = bytes.fromhex('b45633a0499a01ec')

# Alternatively use rusEFI-style challenge (BCM always sent this to rusEFI):
# FRAME1 = bytes.fromhex('2617 14F094E7297F'.replace(' ',''))
# FRAME2 = bytes.fromhex('B6E2C52595ACD6F6')  # session 0 example

print(f"Opening {CHANNEL} at {BITRATE} baud...")
bus = can.interface.Bus(channel=CHANNEL, bustype='pcan', bitrate=BITRATE)
print(f"Listening for 0x0713 IMMO trigger...")
print(f"  Will reply with:")
print(f"    0x0714 Frame1: {FRAME1.hex(' ')}")
print(f"    0x0714 Frame2: {FRAME2.hex(' ')}")
print(f"Make sure OpenOCD breakpoints are set:")
print(f"  bp 0x082078D4 2 hw   (SLib init BLX R3)")
print(f"  bp 0x0820630C 2 hw   (computation path, if init already done)")
print(f"  bp 0x082027A4 2 hw   (main IMMO computation trigger)")
print()

seen_0713 = 0

try:
    while True:
        msg = bus.recv(timeout=5.0)
        if msg is None:
            print(f"[{time.strftime('%H:%M:%S')}] Waiting for 0x0713...")
            continue

        if msg.arbitration_id == 0x0713:
            seen_0713 += 1
            t0 = time.perf_counter()
            print(f"[{time.strftime('%H:%M:%S')}] 0x0713 #{seen_0713}: {msg.data.hex(' ')} — sending 0x0714...")

            # Send Frame1 immediately
            resp1 = can.Message(
                arbitration_id=0x0714,
                data=FRAME1,
                is_extended_id=False
            )
            bus.send(resp1)
            t1 = time.perf_counter()

            # Send Frame2 right after (~0.2ms gap as seen in original traces)
            resp2 = can.Message(
                arbitration_id=0x0714,
                data=FRAME2,
                is_extended_id=False
            )
            bus.send(resp2)
            t2 = time.perf_counter()

            print(f"           Frame1 sent +{(t1-t0)*1000:.2f}ms, Frame2 sent +{(t2-t0)*1000:.2f}ms")
            print(f"           >>> Now check OpenOCD: did breakpoint fire? <<<")
            print(f"           >>> Commands to run in OpenOCD: <<<")
            print(f"               halt")
            print(f"               reg r3")
            print(f"               reg r0")
            print(f"               reg r1")
            print(f"               reg r2")
            print(f"               reg pc")
            print(f"               mdb 0x200002C8 1")
            print(f"               mdb 0x20001ADC 4")

        elif msg.arbitration_id == 0x0714:
            print(f"[{time.strftime('%H:%M:%S')}] 0x0714 received (our Tx echo or BCM): {msg.data.hex(' ')}")

except KeyboardInterrupt:
    print("\nStopped.")
finally:
    bus.shutdown()
