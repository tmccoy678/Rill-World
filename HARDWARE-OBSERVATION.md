# Hardware observation boundary

## Purpose

Blu uses read-only access to Agar's exposed hardware anatomy to design candidate
physics for the body that actually exists. Candidate access remains limited to
physical possibilities admitted through an explicit lineage decision.

## Read-only surfaces available to Blu

### Processor and timing

- Intel Core i5-5257U, 2.70 GHz base
- two physical cores and four logical processors
- 14 nm Broadwell generation
- 64-byte cache lines
- 32 KB L1 instruction cache
- 32 KB L1 data cache
- 256 KB L2
- 3 MB shared L3
- 1 GHz machine timebase
- 100 Hz kernel clock rate
- hardware support including SSE, AVX, AVX2, AES, RDRAND, and RDSEED
- per-core frequency and idle-state residency through root-authorized
  `powermetrics`

One bounded sample on November 10, 2026 showed an average processor frequency
near 1.8 GHz during light load, with substantial core idle residency. The value
records that moment only.

### Memory

- 8 GB total
- two 4 GB DDR3 modules at 1867 MHz
- 4 KB memory pages
- module status reported OK

### Graphics and display

- integrated Intel Iris Graphics 6100
- up to 1536 MB dynamic graphics memory
- Metal support
- built-in 2560 × 1600 Retina display

### Environmental interfaces

- two-channel built-in microphone input
- two-channel built-in speaker output
- built-in camera
- Broadcom Bluetooth controller

### Storage and power

- 121.1 GB solid-state PCI storage
- APFS volume group
- internal battery and external-power state
- scheduler, speed-limit, and thermal-warning state
- current process resource use

The observation record excludes unique serial numbers, hardware UUIDs, MAC
addresses, credentials, and network identifiers.

## Root boundary

Root belongs to the caretaker layer. It may be used for a bounded read-only
hardware observation or an explicitly authorized one-time candidate launch.
Candidate state receives only the chosen local physics.

Ordinary software reaches instructions, atomic operations, cache lines, pages,
shared-memory regions, thread activity, device interfaces, and timing or
resource effects. Individual transistors and the 14 nm physical layout remain
beneath that interface.

## Candidate boundary

The first candidate begins with local state alone. Later proposals may admit a
small, explicit coupling after answering:

1. Is the observed quantity physically relevant to the local mechanics?
2. Does it change possibility rather than supply a preferred response?
3. Is the coupling bounded, inspectable, and removable?
4. Can it operate without granting a device handle or administrative access?
5. Does the coupling preserve uncertainty rather than translate the body into
   human labels?

## Current consequence

Experiment 02 is planning-only. Its conceptual architecture begins at effective
Level 0 rather than at logical processors or cache lines. Hardware observation
constrains the implementation surface, timing, and later coupling. The current
plan authorizes only source-level work at that boundary.

## Reference

Intel Corporation. *Intel Core i5-5257U Processor specifications.*
https://www.intel.com/content/www/us/en/products/sku/84985/intel-core-i55257u-processor-3m-cache-up-to-3-10-ghz/specifications.html
