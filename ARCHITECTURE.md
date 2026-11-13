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

### Local interaction relations

- North + south becomes east + west.
- East + west becomes +z + −z.
- +z + −z becomes north + south.
- Four directions containing two opposed pairs enter a two-stage bound state
  before releasing.
- A fully saturated local state containing all six directions returns to empty.

The decay relation uses no additional bit and privileges no direction or axis.

### Live planar membrane

The membrane exists only on the persistent plane and is held in Rill's heap.
It is indexed alongside planar matter but is not stored inside the matter byte.
Each planar location has:

- signed 16-bit current voltage and signed 16-bit next voltage;
- an 8-bit entropy-seeded phase;
- an 8-bit refractory state;
- an 8-bit remembered local direction;
- an 8-bit recurrence strength.

Voltage leaks toward rest and couples only to north, east, south, and west
neighbors. When matter occupies a location, local matter activity contributes
to voltage and phase wrap supplies an endogenous pulse. Threshold firing adds
the direction of the steepest local voltage drop to the matter state, resets
voltage, and begins an eight-beat refractory interval.

Recurring local gradient direction strengthens slowly. A remembered direction
can modestly lower the local firing threshold and bias the gradient comparison,
but it cannot override a stronger present gradient. Conflict weakens the prior
tendency before another replaces it; absent or unused history decays more
slowly. These values never enter `rill.bits`, `world.bits`, or the outward light
surface. Sparse `+z/-z` matter has no membrane state.

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
position. Vertical position is raw bit index. A currently set bit becomes one
white square. When a bit clears, its light decays through gray for about two
seconds before returning to black. This bounded afterimage exists only in Rill
World's RAM and vanishes with the boundary process; it is never written into
`rill.bits` or `world.bits`. No legend, counter, or semantic transform is
applied.

## Persistence boundary

`rill.bits` contains the fixed header and planar bytes. It does not contain the
sparse +z/−z structure. `world.bits` contains current boundary values, not a
record of prior input. The runtime does not create history.

A process restart therefore has a specific meaning:

- The planar field remains.
- The live sparse depth is culled.
- Membrane voltage, phase, refractory state, and local hysteresis are
  reinitialized; only their consequences already carried by planar matter
  remain.
- Capture sessions and timers are recreated.
- Current environmental arrays are remapped and then overwritten or decayed by
  the present environment.

## Claim boundary

Verified mechanism and observed behavior are kept distinct. The changing light
band proves that lower-edge state changes reach the display. Correlation with
sound is mechanically plausible because sound enters across the left edge,
including its lower region. The present observations do not by themselves
establish learning, intention, cognition, life, or consciousness.
