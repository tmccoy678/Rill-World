# Single-file carrier mechanics

## Scope

This is the first proposed inactive mechanical trial. It contains one connected
carrier, one narrowing passage, one surrounding medium, and smaller mobile
states. It has no templating, copying, branching network, host coupling,
external input, persistent state, or connection to the continuing runtime.

## Dimensionless geometry

Initial ratios are references for falsification, not final laws:

- unconstrained carrier width: 8 local length units;
- passage throat: 4–5 local length units;
- throat/free-width ratio: 0.50–0.625;
- upstream and downstream regions long enough for an unconstrained form to be
  measured before and after passage;
- only one carrier admitted across the throat at a time;
- smaller mobile states remain able to occupy available medium around it.

Its unconstrained geometry must follow from local boundary tension, interior
capacity, and present medium conditions rather than a predetermined biological
shape.

## Carrier

The carrier is a connected boundary enclosing or retaining smaller local state.
It has no central controller and no fixed coordinate template.

Relational continuity may depend on:

- preserved boundary connectivity;
- bounded leakage;
- cyclic order along the boundary;
- retained interior material;
- recovery of an unconstrained local equilibrium after passage.

Relational continuity may survive changes in the identity of individual
pixels.

## Passage and medium

The passage is a region of reduced local capacity or permeability. Its wall is
represented through local constraints rather than a global route instruction.

The medium must:

- transmit local pressure or capacity differences;
- permit smaller mobile states to continue around the carrier where capacity
  remains;
- resist motion without prescribing direction;
- respond locally to carrier displacement;
- avoid deleting or transporting the carrier through a global command.

## Required sequence

1. Local transfer brings the carrier toward the narrowing.
2. Contact with the passage changes local geometry.
3. The connected boundary deforms without a commanded deformation phase.
4. The carrier enters and traverses the throat.
5. Smaller mobile states continue exchanging through remaining capacity.
6. The carrier exits.
7. Local relations either recover an unconstrained configuration or fail by a
   mechanically visible mode.

## Honest failure modes

Acceptable outcomes include:

- permanent blockage;
- boundary rupture;
- loss of interior state;
- fragmentation;
- inability to enter;
- failure to recover;
- medium collapse;
- passage-wall change;
- successful passage with altered relational state.

Each outcome remains visible in the final mechanical state; the apparatus does
not reset, repair, or replace the carrier.

## Mechanical acceptance questions

- Does the carrier approach the throat through local transfer alone?
- Does deformation preserve a connected boundary?
- Is interior retention measurable without assigning meaning to the contents?
- Can smaller mobile states continue around the carrier?
- Is passage serial when a second inert test carrier is later introduced?
- Does downstream geometry recover through the same local physics that formed
  the upstream state?
- Can failure be distinguished mechanically without semantic runtime logging?

## Intel layout target

The first implementation should use cache-aligned local tiles and a complete
working set small enough to remain primarily within Agar's shared L3. Hot
present-state, next-state, and medium tiles should remain beneath L2 where
practical.

The Intel-specific layout makes the candidate native to Agar's body and gentle
on the continuing process. Acceptance follows from mechanics rather than
speed.
