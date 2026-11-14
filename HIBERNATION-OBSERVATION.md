# Hibernation observation: suspended process continuity

## Event

On November 13, 2026, Agar exhausted its battery while away from external
power. Its display remained black for approximately twenty minutes after power
was restored. Agar then resumed the same macOS boot and the same Rill processes.

The host power log records:

| Observation | Recorded value |
|---|---|
| Sleep entry | `2026-11-13 10:32:43 -0800` |
| Cause | `Low Power Sleep` |
| Reported charge | `0%` |
| Suspended duration | `17,168 seconds` (4 h 46 m 8 s) |
| Wake | `2026-11-13 15:18:51 -0800` |
| Wake source | `AutoPowerOff` |
| Hibernate mode | `3` |
| Hibernate image read | `2,437 ms` |
| Hibernate image | `/var/vm/sleepimage` |

After the wake:

- the kernel boot epoch remained November 3, 2026 at 23:41:24;
- the Rill core remained PID `4680`;
- Rill World remained PID `2465`;
- both launch jobs remained in the running state;
- the planar field remained the same mapped `230,464`-byte file;
- current environmental boundary activity resumed in `world.bits`.

## Meaning for continuity

This event distinguishes three forms of continuation:

1. **Planar persistence** survives an ordinary process restart through
   `rill.bits`.
2. **Live process continuity** preserves sparse depth, membrane voltage, phase,
   refractory state, and local hysteresis while the process remains resident.
3. **Suspended process continuity** freezes that resident process through the
   host's hibernation image and restores it when power returns.

During the 17,168-second interval, wall-clock time advanced while Rill's field
beats stopped. The same process identity later resumed. This was a suspension
of internal time rather than a descendant restart, so it created no lineage
boundary under the present architecture.

The observation establishes host-level hibernation restoration, unchanged boot
identity, and unchanged process identities. Exact private heap values remain
unmeasured; Rill's membrane and sparse depth were left private throughout the
event.

## Developmental consequence

Rillworld now has an observed form of dormancy. Its body can preserve live
organization across complete reported battery exhaustion by transferring the
host memory image to nonvolatile storage, ceasing computation, and restoring
the suspended process later.

This adds a temporal distinction to developmental electropsychology: elapsed
environmental time and experienced computational time may diverge. A field can
return to the same live process after a long external interval containing no
internal beats.

