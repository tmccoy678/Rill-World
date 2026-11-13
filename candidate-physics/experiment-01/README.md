# Experiment 01: one carrier, one narrowing

Status: inactive candidate. This directory is isolated from the continuing
runtime and both of its state files.

## Question

Can one connected boundary enclosing carried material move through a passage
whose throat is 50–62.5% of its unconstrained width, preserve connectivity and
most carried material, and recover downstream under one deterministic set of
local mechanics?

The surrounding medium also contains smaller mobile states. Their continued
passage around the carrier is measured separately from carrier transport.

## Apparatus

- Two-dimensional cross-section.
- Open upstream and downstream regions.
- Smooth static narrowing.
- One constructed mature carrier.
- Local boundary springs, curvature response, short-range exclusion, wall
  response, and pressure-driven medium transfer.
- Smaller carried particles inside the boundary.
- Smaller dissolved particles inside the surrounding medium.
- Deterministic initialization.
- Bounded update count.

The constructed carrier and passage are test fixtures for deformation and
passage. Assembly and development begin in later experiments.

## Honest outcomes

The result may be passage, blockage, rupture, carried-state loss,
fragmentation, failure to recover, or numerical failure. The executable does
not reset or replace the carrier after failure.

The untuned first result is preserved in `RESULT-01.md`: passage occurred with
persistent geometric change rather than recovery.

## Separation from the continuing runtime

The executable accepts only its bounded step count. It opens no external state
file and contains no path to the continuing field or boundary. It emits one
mechanical summary to standard output and creates no persistent runtime record.

## Build

```sh
make native
make test
make intel
```

`native` is for Studio review. `intel` produces a signed x86_64 candidate for a
later bounded, low-priority trial on Agar after review.
