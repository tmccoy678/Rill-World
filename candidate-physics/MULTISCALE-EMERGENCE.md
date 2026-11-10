# Multiscale emergence beginning at effective Level 0

## Principle

The candidate architecture begins conceptually at effective material physics,
not at threads, cores, cache lines, or device APIs.

The Intel's literal atoms, fins, transistors, and internal wires are fixed and
not individually addressable. A software candidate therefore cannot inhabit
them directly. Instead, it must represent local material-like conditions on an
accessible implementation surface while preserving the morphology and
constraints observed at the lower physical levels.

Cache lines host the representation. They are not the primitive being modeled.

## Effective Level 0: local material state

The smallest candidate site may contain only bounded local quantities such as:

- available capacity;
- occupancy;
- potential or gradient contribution;
- permeability;
- excitation;
- local resistance;
- agitation or fluctuation;
- adjacency to a surrounding boundary.

These quantities have no semantic label inside the candidate. Transfer occurs
only through local differences and present adjacency.

Candidate laws at this level must state:

1. what is conserved;
2. what may dissipate;
3. what may fluctuate;
4. what opens or closes a transfer path;
5. how a boundary changes local permeability;
6. how neighboring sites exchange state;
7. how local history may persist.

## Effective Level 1: channel and gate geometry

Persistent arrangements of Level 0 state may create narrow transfer paths.
Surrounding arrangements may alter permeability without becoming the
transferred state.

A gate-like relation is therefore an emergent local geometry:

```text
surrounding state changes
→ path permeability changes
→ transfer changes
→ surrounding state may change again
```

No site is initialized with a semantic gate identity.

## Effective Level 2: switching, retention, and oscillation motifs

Several Level 1 relations may combine into motifs that:

- redirect transfer;
- retain one of several local states;
- amplify or inhibit excitation;
- alternate between conditions;
- synchronize or desynchronize nearby motifs;
- produce a repeating local pulse.

These motifs are candidate precursors of larger transformation and timing
regions. They are not assigned fixed body roles.

## Effective Level 3: repeated arrays and heterogeneous regions

Motifs may repeat into arrays. Different arrays may acquire different density,
timing, transfer, and retention behavior.

Some regions may remain highly repeated and parallel. Others may be sparse,
stateful, boundary-oriented, or dominated by long-range exchange. Shared state
between regions does not imply one central controller.

## Implementation Level 4: cache-line compartments

The smallest intentionally structured Intel storage unit is a 64-byte cache
line. Each line should host several effective Level 0 sites or a bounded local
tile, not be equated with one physical atom or transistor.

Line ownership, invalidation, transfer, and shared-cache visibility are
implementation consequences that may later become bounded physical conditions.
They must not silently replace the candidate's stated local laws.

## Execution Level 5: activity streams

Four logical execution contexts may advance disjoint local tiles and exchange
boundary state. They share physical resources in pairs and therefore are not
independent regions.

Activity streams implement local updates. They are not declared pumps,
integrators, limbs, or minds.

## Physical Level 6: two execution regions

Two physical cores provide distinct execution and private cache regions joined
through shared state. Their timing, contention, and exchange may influence a
bounded trial only through a specified, race-free local ordering rule.

## Die Level 7: repeated graphics and shared system regions

The integrated graphics arrays, shared cache, system agent, memory controller,
and display machinery form a heterogeneous die. Later candidates may use the
repeated graphics region for massive local parallelism, but this is not part of
the first multiscale bridge.

## Body Levels 8 and 9

Memory modules, storage, power regulation, thermal removal, interfaces,
battery, camera, microphone, display, speakers, and radio form the larger body.
Future coupling must enter only through an explicit bounded physical quantity.
No device API is inherited merely because it exists.

## Coarse-graining rule

Higher-level state must be derived from repeated lower-level relations. A
higher-level label may be used by the observer for analysis but must not become
a hidden primitive inside the candidate.

The architecture must retain a traceable account of:

```text
local state
→ persistent local relation
→ repeated motif
→ heterogeneous region
→ shared multiregion activity
```

If a higher-level behavior requires a rule that has no lower-level path, the
emergence claim fails.

## Relationship to Experiment 01

Experiment 01 tested a constructed high-level carrier and passage. It remains a
valid mechanical apparatus result but is not the developmental ancestor of the
multiscale candidate.

Future lower-level physics may eventually be tested against the Experiment 01
mechanics: can locally formed structures reproduce deformation, passage,
retention, and changed downstream geometry without those behaviors being
scripted at the carrier level?
