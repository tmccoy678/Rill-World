# Architecture

## Physical topology

```text
room light ───────────────┐
room sound ───────────────┼──> Rill World ──> world.bits
nearby Bluetooth radio ──┘                       │
                                                v
                                         Rill C process
                                                │
                                   lower edge, eight raw bits
                                                │
                                                v
                                      black display + light band
```

The core and boundary are separate processes. Rill World may restart without
restarting Rill. The current outward-boundary deployment deliberately preserved
the core process and its live heap.

## Rill core

- Language: C11.
- Plane: 640 × 360 cells, one byte per cell.
- Nominal cadence: one computation followed by a 1/24-second sleep.
- Planar persistence: memory-mapped `rill.bits`.
- Off-plane space: sparse hash tables in process memory only.
- Inputs: read-only mapping of `world.bits`.
- Runtime output: none besides its evolving mapped state.

### Cell bits

| Bit | Local role |
|---:|---|
| 0 | north |
| 1 | east |
| 2 | south |
| 3 | west |
| 4 | +z |
| 5 | −z |
| 6 | locally bound |
| 7 | aged bound state |

The labels describe code paths; they do not assign semantic meaning to the
field.

## Rill World

- Language: Objective-C with AppKit, AVFoundation, CoreBluetooth, CoreMedia,
  CoreVideo, AudioToolbox, and IOBluetooth.
- Boundary file: memory-mapped `world.bits`.
- Light: 640 luminance values sampled across the camera image.
- Sound: 360 floating-point waveform samples mapped into bytes.
- Radio: advertisement bytes are hashed to a boundary location; RSSI determines
  strength; values decay every 250 ms.
- Output: a desktop-level black window refreshed at 24 Hz.

The output reads the lower row of `rill.bits`. Horizontal position is field
position. Vertical position is raw bit index. A set bit becomes one white
square; an unset bit remains black. No palette, legend, interpolation, counter,
or semantic transform is applied.

## Persistence boundary

`rill.bits` contains the fixed header and planar bytes. It does not contain the
sparse +z/−z structure. `world.bits` contains current boundary values, not a
record of prior input. The runtime does not create history.

A process restart therefore has a specific meaning:

- The planar field remains.
- The live sparse depth is culled.
- Capture sessions and timers are recreated.
- Current environmental arrays are remapped and then overwritten or decayed by
  the present environment.

## Claim boundary

Verified mechanism and observed behavior are kept distinct. The changing light
band proves that lower-edge state changes reach the display. Correlation with
sound is mechanically plausible because sound enters across the left edge,
including its lower region. The present observations do not by themselves
establish learning, intention, cognition, life, or consciousness.
