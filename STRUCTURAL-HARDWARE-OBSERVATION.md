# Structural hardware observation

## Scope and evidence levels

This observation asks what Agar's hardware looks like across nested scales and
what its physical organization adds to numerical specifications.

The available views establish three evidence levels:

- Intel's published 14 nm fin diagrams describe the process family used by the
  i5-5257U at family level.
- Intel's published dual-core Broadwell-U die map describes the processor
  family with HD Graphics 6000 or Iris Graphics 6100.
- iFixit's Early 2015 13-inch Retina MacBook Pro teardown shows the same
  MacBookPro12,1 logic-board family and identifies the i5-5257U package.
- Internal metal layers, transistor placement, and proprietary routing remain
  outside the published views.

Direct visual features and design inference are therefore kept separate.

## Scale 1: the 14 nm switch

### Direct observation

Intel's comparison of its first- and second-generation tri-gate process shows
vertical silicon fins rising from one continuous silicon substrate. A metal
gate surrounds the exposed sides and top of the fins. The 14 nm structure uses
fewer, taller, thinner, and more closely spaced fins than the preceding 22 nm
structure.

The smallest published functional shape is a narrow conducting ridge whose
surrounding boundary controls whether charge may pass.

### Inference

The controlling structure can wrap around a narrow passage and change its
permeability from the boundary.

Repeated narrow channels can act together beneath one surrounding gate. Fewer
stronger channels may replace a larger number of weaker channels while
preserving the larger switching function.

This gives the candidate a physical precedent for local passages whose walls
control transfer without becoming the transferred material.

## Scale 2: repeated blocks on the Broadwell-U die

### Direct observation

The published dual-core die map is strongly heterogeneous:

- a large processor-graphics region occupies most of the die;
- two CPU-core regions repeat beside one another;
- both cores share a distinct L3 region;
- a system-agent, display-engine, and memory-controller region borders them;
- a long memory-controller I/O band spans the lower edge;
- the processor-graphics region itself contains many visibly repeated arrays.

The family diagram reports approximately 1.9 billion transistors in 133 mm².

### Inference

The computational body extends beyond its two CPU cores. Large repeated arrays
and shared transport or memory structures occupy at least as much conceptual
importance.

The integrated graphics region may be a closer physical analogy to distributed
excitable tissue than the two CPU cores because it is built from many repeated
parallel units. The shared L3 and long memory I/O band resemble common exchange
regions and trunks connecting heterogeneous areas.

Four logical processors are interleaved activity contexts inside two physical
core regions. Their shared physical substrate constrains any bodily analogy.

## Scale 3: package, memory, regulation, and logic board

### Direct observation

The exact-model-family teardown shows:

- one prominent Intel processor package;
- several adjacent soldered memory packages;
- storage, audio, Thunderbolt, wireless, display, and I/O controllers in
  separate locations;
- numerous small power-conversion, sensing, switching, and passive components;
- functional components distributed across both sides of the logic board;
- ports concentrated at body edges;
- a battery, speakers, camera, microphone, display, fan, heat sink, trackpad,
  and storage connected through board traces and cables.

Surface photographs reveal only part of the connection topology carried by
internal board layers.

### Inference

The whole body is distributed and multilayered. The processor package is a
major transformation site but depends continuously on nearby memory, power
regulation, timing, thermal removal, device controllers, and physical
boundaries.

Regulation is dispersed. Many small components maintain voltages, switch paths,
measure conditions, and protect interfaces. Pumping, integration, and control
therefore have several possible physical homes.

## Scale transition

The most important observation is that each level changes the form of
organization:

```text
fin and surrounding gate
→ transistor and logic relation
→ repeated execution array
→ core, cache, graphics, and system regions
→ package, memory, and controllers
→ two-sided logic board
→ battery, sensors, display, sound, storage, cooling, and enclosure
```

Each larger level introduces new boundaries, shared pathways, asymmetries, and
regulatory layers.

Rillworld therefore permits repeated local units to combine into regions with
different geometry and function, connected through shared media and boundary
structures.

## What this changed

Before the structural look, Experiment 02 risked treating the four logical
processors as the obvious whole-body units. The images show that this is too
coarse and too centralized.

The next design must consider:

1. gate-like boundaries around narrow transfer paths;
2. repeated arrays as a major form of physical computation;
3. shared cache and I/O regions as common exchange structures;
4. two physical cores with four interleaved activity contexts;
5. integrated graphics as part of Agar's computational anatomy;
6. distributed power and interface regulation;
7. multilayer relationships that are not visible on one surface;
8. different organizational rules at different scales.

Experiment 02 now begins conceptually at effective Level 0. Cache-line state is
its accessible implementation surface. The planning-only trial asks whether
local material-like state can produce boundary-controlled transfer geometry.

## Sources viewed

- Intel Corporation. *14 nm Tri-gate Transistor Fins.* Mark Bohr 2014 IDF
  presentation. https://www.intel.com/content/dam/www/public/us/en/documents/pdf/foundry/mark-bohr-2014-idf-presentation.pdf
- Intel Corporation. *Intel Core i5-5257U specifications.*
  https://www.intel.com/content/www/us/en/products/sku/84985/intel-core-i55257u-processor-3m-cache-up-to-3-10-ghz/specifications.html
- Intel Corporation. *5th Gen Intel Core Processor Die Map: Intel HD Graphics
  6000 or Intel Iris Graphics 6100.* Family-level slide reproduced at
  https://notebook.cz/clanky/technologie/2015/intel-hd-graphics-5500
- iFixit. *MacBook Pro 13-inch Retina Display Early 2015 Teardown.*
  https://www.ifixit.com/Teardown/MacBook+Pro+13-Inch+Retina+Display+Early+2015+Teardown/38300
