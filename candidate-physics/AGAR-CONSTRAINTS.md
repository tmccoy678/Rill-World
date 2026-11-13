# Agar-specific constraints

## Measured body

Read-only measurement on November 10, 2026:

| Property | Measured value |
|---|---:|
| Model | MacBookPro12,1 |
| Processor | Intel Core i5-5257U at 2.70 GHz |
| Physical cores | 2 |
| Logical processors | 4 |
| Installed memory | 8,589,934,592 bytes |
| Memory page | 4,096 bytes |
| Cache line | 64 bytes |
| L1 instruction cache | 32,768 bytes |
| L1 data cache | 32,768 bytes |
| L2 cache | 262,144 bytes |
| Shared L3 cache | 3,145,728 bytes |
| Machine timebase | 1,000,000,000 Hz |
| Kernel clock rate | 100 Hz |
| Memory modules | 2 × 4 GB DDR3 at 1867 MHz |
| Runtime architecture | x86_64 |
| Operating system | macOS Monterey 12.7.6 |

At measurement, Agar was on external power with a charged battery. The system
reported no thermal or performance warning, all four logical processors
available, and a full scheduler and speed limit.

The body also exposes integrated graphics, a built-in display, two-channel
audio input and output, a camera, a Bluetooth controller, solid-state storage,
and bounded timing and processor-residency measurements. These are caretaker
observations. The inactive candidate remains isolated from those interfaces.

## Memory-hierarchy implications

The continuing planar field contains 640 × 360 one-byte sites, or 230,400
bytes. One plane therefore fits beneath the 262,144-byte L2 capacity, although
two complete update buffers do not. The shared L3 can contain several such
planes plus bounded metadata.

An inactive candidate should use a structure-of-arrays layout and update local
tiles whose hot state remains comfortably beneath L2 capacity. Contiguous tile
dimensions should align with the 64-byte cache line where practical. Persistent
files should align to the 4,096-byte memory-page boundary.

These constraints govern physical efficiency alone. Candidate structure and
outcome remain consequences of the local laws.

## Initial working envelope

The first mechanical candidate should remain far smaller than installed memory
and leave the continuing runtime undisturbed. Its active hot
tile, present local medium, and next-state tile should target L2. The complete
candidate state should target L3 where practical.

The candidate must run at ordinary priority, remain manually invoked, and
terminate after its bounded mechanical trial. It must not attach to, map, copy,
or modify the continuing field or boundary files.

## Timing

Candidate time must be expressed in local update steps. Wall-clock measurements
may characterize cost but must not become a target for the local state.

Later host coupling may admit bounded changes in cadence, capacity,
resistance, or transformation threshold. The first trial remains locally
closed while those couplings are developed.

Administrative privilege is never a candidate input or capability. A future
hardware coupling must expose only the exact bounded physical quantity required
by its stated local rule.
