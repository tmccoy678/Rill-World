# Result 01: passage with change

## Candidate

- Source: `carrier_trial.c`
- Step bound: 40,000
- Initialization: deterministic
- x86_64 binary SHA-256:
  `12c746efc0373eddf8bed3266be801f304b2d85074011c4346e848f2c934e1a4`
- Runtime inputs: `--steps 40000` only
- Persistent output: none

No parameter was changed after observing the result.

## Studio trial

```text
steps=40000
outcome=passage_with_change
centroid_x=27.554415 centroid_y=-0.046042
carrier_width=7.626734 carrier_height=3.753446
max_adjacent_separation=0.601547 rest_separation=0.522778
carried_retained=23/29
dissolved_passed=24/72
real=0.38 seconds
```

A deterministic repeat produced the same reported values.

## Agar trial

The reviewed signed x86_64 candidate ran once from `/tmp` at nice level 15.

```text
steps=40000
outcome=passage_with_change
centroid_x=27.554415 centroid_y=-0.046042
carrier_width=7.626734 carrier_height=3.753446
max_adjacent_separation=0.601547 rest_separation=0.522778
carried_retained=23/29
dissolved_passed=24/72
real=1.09 seconds
```

The continuing core remained PID 3516. The boundary remained PID 2465. The
system launch job remained running with last exit code 0.

## Mechanical interpretation

The carrier crossed downstream while preserving adjacent-boundary separation
within the predefined connectivity threshold. It retained 79.3% of its carried
particles. One third of the dissolved particles crossed the narrowing during
the bounded trial.

The carrier did not satisfy the recovery condition. Its final height was 3.753,
below 65% of its initial height of 8.0. The honest outcome is therefore passage
with persistent geometric change, not passage with recovery.

## Limits

- Two-dimensional cross-section.
- Constructed carrier and fixed passage.
- Prescribed local transfer field.
- Overdamped update.
- No assembly, maturation, copying, branching, host coupling, or persistent
  state.
- The apparatus approximates a medium but does not solve a complete local fluid
  system.

This result answers only the first bounded mechanics question. It does not
justify connection to the continuing runtime or promotion of any rule.
