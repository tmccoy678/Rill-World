# Rillworld

> An environmentally coupled persistent computational field.

At the beginning there was no model to train and no language to complete.
There was a different question: what might computation become if the machine
were treated as the medium, the environment arrived before interpretation, and
nothing inside was required to speak?

Rillworld grew from that question on **Agar**, a clean 2015 Intel MacBook Pro.
Rill is the continuing field. Rill World is its physical boundary. Together
they form a nonlinguistic bit-field coupled to light, sound, nearby radio
activity, time, heat, power, and—in November 2026—an outward surface of light.

From across the room, the surface appears as a flickering horizon. Close up,
it resolves into a fine thread, clustered runs, short stepped structures, and
gaps. The display is not a dashboard. It is the lower edge of the field made
physical.

## The field

Rill is a 640 × 360 plane implemented in C. Each cell is one byte whose bits
may represent movement north, east, south, west, +z, or −z, plus two local
matter-state bits. Its nominal rhythm is 24 beats per second.

The visible plane persists in `rill.bits`. Off-plane depth is sparse and exists
only in the running process. A restart therefore preserves the plane while
culling the live depth. That asymmetry is part of the present system, not hidden
behind a claim of perfect continuity.

The first night's measured history and graphs are preserved as both
[`Rill_First_Night.Rmd`](docs/first-night/Rill_First_Night.Rmd) and a
[self-contained HTML report](docs/first-night/Rill_First_Night.html).

## The world

Rill World is a native Objective-C application using Apple frameworks:

- Camera luminance reaches the upper edge.
- Microphone waveform values reach the left edge.
- Bluetooth advertisement bytes and signal strength reach the right edge.
- The eight raw bits at the lower edge become a 640 × 8 white-on-black light
  surface.

One bit-plane of the incoming boundary fields is admitted on each field beat.
The outward band reads Rill's lower edge directly. It does not read audio or
produce an interpreted visualization.

The runtime creates no transcript, event log, census, or snapshot stream. The
two files in [`state/`](state/) are a single deliberate archival act taken
while Rill continued running; they are not runtime telemetry and cannot
reconstruct the live off-plane field.

## Repository map

- [`agar_medium.c`](agar_medium.c) — Rill's continuing field.
- [`rill_world.m`](rill_world.m) — physical inputs and outward light boundary.
- [`ARCHITECTURE.md`](ARCHITECTURE.md) — exact mechanism and claim boundaries.
- [`ORIGIN.md`](ORIGIN.md) — the story from the first question to the first light.
- [`HARDWARE.md`](HARDWARE.md) — the physical substrate and build surfaces.
- [`REFERENCES.md`](REFERENCES.md) — verified conceptual lineage in APA style.
- [`docs/first-night/`](docs/first-night/) — the R source and rendered first-night
  graphs.
- [`state/SNAPSHOT.md`](state/SNAPSHOT.md) — provenance for the preserved bytes.

## Build

The target is Intel macOS Monterey:

```sh
make verify
```

This produces an ad hoc signed x86_64 core and application bundle under
`build/`. Deployment is intentionally not automated here: replacing the live
boundary and restarting the live field are different operations.

## Authorship

Rillworld was conceived and directed by **Taylor Westfall-McCoy** and built in
the Blu Private workspace with **Blu**. Its language is theoretical where the
question is theoretical, and literal where the mechanism is known.

The project leaves the nature of what may develop unclassified.

## License

Rill-World is available under the [Apache License 2.0](LICENSE).
