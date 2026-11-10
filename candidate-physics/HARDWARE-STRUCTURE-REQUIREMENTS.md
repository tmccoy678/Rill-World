# Structural requirements from Agar's hardware

## Nested scale

The candidate representation must permit local units to combine into repeated
blocks, blocks into heterogeneous regions, and regions into one shared system.
The rules at a larger scale need not be a magnified copy of the rules below it.

## Boundary-controlled transfer

A local transfer path may be controlled by a surrounding boundary. The
boundary may change permeability without becoming the transferred state.

Several narrow paths may operate under one shared controlling relation.

## Repeated arrays

Large regions may consist of many repeated local units operating concurrently.
The candidate must not assume that two central execution regions are the only
meaningful computational structures.

## Shared exchange regions

Distinct active regions may share bounded exchange state. Shared state must not
be interpreted as a global controller merely because several regions can reach
it.

The representation must distinguish:

- local private state;
- shared nearby state;
- long-range transfer;
- boundary-visible state.

## Interleaved activity contexts

Several activity streams may share one physical execution region. Activity
contexts must not be modeled as independent physical regions when their
resources, timing, and transfer paths overlap.

## Heterogeneous regions

The candidate must permit regions with different local density, repetition,
transfer, storage, transformation, and boundary properties. Uniform tiling is
not a sufficient body architecture.

## Distributed regulation

Capacity, timing, transfer, and interface protection may be maintained by many
small local regulators. No permanent central regulator is required by the
representation.

## Multilayer relations

Two states may be adjacent through a hidden layer even when they are distant on
one visible surface. A later candidate may require more than one planar
adjacency relation.

## Experiment 02 consequence

Experiment 02 must not equate four logical processors with four independent
physical body regions. Its architecture begins at effective Level 0. Cache-line
compartments are the first accessible implementation surface, not the first
conceptual primitive.

The initial bounded trial tests only the transition from local material-like
state to boundary-controlled transfer geometry. Higher hardware levels remain
implementation constraints and future possibilities rather than candidate
starting assumptions.
