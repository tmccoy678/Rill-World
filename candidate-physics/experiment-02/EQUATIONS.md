# Experiment 02 equations: effective Level 0

Status: candidate mathematics only. These equations do not authorize source,
an executable, a runtime connection, or a root trial.

## Meaning of Level 0

Level 0 is the candidate's irreducible physics, not a claim of software access
to Intel's atoms, fins, or transistors. Ordinary memory will eventually carry
the representation. Nothing inside the candidate receives a cache-line,
thread, core, device, or host label.

This first projection uses two conserved, unnamed contents, `q` and `r`, and
unoccupied local capacity. Their asymmetry is physical rather than semantic:
`r` can cohere locally, while nearby `r` changes the ease with which `q` moves.
No site or quantity is named as a wall, path, switch, or gate.

## Space and neighborhood

Let the candidate space be a finite three-dimensional periodic lattice

\[
G=(V,E),
\]

with six face-adjacent neighbors at every site: positive and negative motion
along each of three axes. Periodicity removes a privileged outer wall. Axes
exist only to define adjacency; no axis has a preferred meaning or direction.

The equations depend only on a site's present state and the state of its six
neighbors. Coordinates, if used by an implementation, are not part of site
state.

## Site state and capacity

At site \(i\),

\[
x_i=(q_i,r_i),
\qquad
q_i\ge 0,
\qquad
r_i\ge 0,
\qquad
q_i+r_i\le 1.
\]

The remaining capacity is

\[
v_i=1-q_i-r_i.
\]

`q`, `r`, and `v` are not symbols interpreted by the candidate. They are the
observer's names for bounded local quantities.

In a closed trial, the totals

\[
Q=\sum_{i\in V}q_i
\qquad\text{and}\qquad
R=\sum_{i\in V}r_i
\]

must remain constant to numerical precision. Unoccupied capacity is neither
created nor destroyed independently; it follows from the two conserved totals.

## Local potential

Let \(T>0\) set the scale of bounded agitation, \(\gamma\ge 0\) set local
competition between the two contents, \(\lambda\ge 0\) set nearest-neighbor
cohesion of `r`, \(z=6\), and \(\epsilon>0\) prevent a singular logarithm.

The local free quantity is

\[
F = \sum_{i\in V}
T\left[
q_i\ln(q_i+\epsilon)
+r_i\ln(r_i+\epsilon)
+v_i\ln(v_i+\epsilon)
\right]
+\gamma\sum_{i\in V}q_ir_i
-\frac{\lambda}{z}\sum_{(i,j)\in E}r_ir_j.
\]

The corresponding local potentials are

\[
\mu_i^q
=T\ln\!\left(\frac{q_i+\epsilon}{v_i+\epsilon}\right)
+\gamma r_i,
\]

\[
\mu_i^r
=T\ln\!\left(\frac{r_i+\epsilon}{v_i+\epsilon}\right)
+\gamma q_i
-\frac{\lambda}{z}\sum_{j\in N(i)}r_j.
\]

These terms provide only local mixing, exclusion, and cohesion. They contain no
target geometry.

## Edge permeability

For adjacent sites \(i\) and \(j\), define

\[
\bar v_{ij}=\frac{v_i+v_j}{2}.
\]

The ease of local passage is

\[
K_{ij}^q
=D_q\,\bar v_{ij}\,
\exp[-\rho(r_i+r_j)],
\]

\[
K_{ij}^r
=D_r\,\bar v_{ij},
\]

where \(D_q,D_r>0\) and \(\rho\ge 0\). Both values are symmetric across an
edge: \(K_{ij}=K_{ji}\).

This is the entire Level 0 ancestor of a later permeability relation. Present
local `r` changes `q` passage; no separate gate variable exists. Cohesive `r`
geometry must form before any surrounding relation can be claimed.

## Local transfer

For content \(s\in\{q,r\}\), a proposed signed transfer from \(i\) to \(j\) is

\[
\widehat\delta_{ij}^s
=\Delta t\,K_{ij}^s(\mu_i^s-\mu_j^s)
+\sigma_s\xi_{ij}^s.
\]

The edge fluctuation is bounded, has zero mean, and is antisymmetric:

\[
\xi_{ji}^s=-\xi_{ij}^s,
\qquad
|\xi_{ij}^s|\le 1.
\]

The accepted transfer is the proposed transfer clipped only to what the source
contains and the destination can hold. For `q`,

\[
\delta_{ij}^q
=\operatorname{clip}
\left(
\widehat\delta_{ij}^q,
-\min(q_j,v_i),
\min(q_i,v_j)
\right),
\]

followed by

\[
q_i' = q_i-\delta_{ij}^q,
\qquad
q_j' = q_j+\delta_{ij}^q.
\]

The same rule applies to `r` with `q` replaced by `r`. Every accepted edge
transfer is therefore exactly conservative and cannot exceed local capacity.

An implementation must advance disjoint edge matchings so that no site is
written by two transfers in the same substep. Matching order, axis order, and
whether `q` or `r` advances first must rotate without favoring one site or
direction. This schedule is numerical machinery, not candidate state.

## Dissipation, fluctuation, and continuity

With \(\sigma_q=\sigma_r=0\) and a sufficiently small \(\Delta t\), transfer
follows decreasing local potential. With bounded nonzero \(\sigma\), local
fluctuation may interrupt, dissolve, or rearrange a configuration without
changing `Q` or `R`.

There is no age field, fitness value, reward, or expiration counter. Local
history persists only as the current distribution of conserved contents. A
coherent arrangement remains when present local relations continue to support
it and changes when they do not.

No exact parameter values are chosen in this document. They must be selected as
one coherent dimensionless regime before any run, not tuned after viewing a
desired form.

## Initial condition

The initial condition must satisfy all of the following:

1. fixed global totals `Q` and `R`;
2. the same sampling law at every site;
3. bounded local variation with no encoded shape;
4. no privileged axis, coordinate, seed site, corridor, or surrounding shell;
5. no externally imposed source, sink, or throughput direction.

The fluctuation source may choose a particular history, but it must not encode a
target configuration. Reproducibility and physical entropy are later
implementation choices; neither changes the equations.

## Level 1 claim boundary

The observer may describe a Level 1 relation only after the trial if the state
itself supports all of these findings:

1. local `r` coherence forms without a prepared shape;
2. a connected region of relatively greater `q` permeability is bounded by
   that coherence;
3. actual `q` transfer differs along the resulting local geometry;
4. the surrounding `r` does not become the transferred `q`;
5. the relation persists across more than one local update;
6. changed local conditions can rearrange or dissolve it.

Thresholds used to describe those findings belong only to external analysis.
They must not enter the equations or runtime state.

A global source-to-sink test is unnecessary here. Local transfer across the
connected high-permeability edges is sufficient; adding a designated inlet or
outlet would install the higher-level relation the trial is meant to earn.

## Falsification

Experiment 02 fails to produce the proposed bridge if any of the following is
true:

- coherence requires a prepared boundary or seed geometry;
- permeability changes only because `q` is removed rather than transferred;
- the apparent path exists only at the periodic seam or from update-order bias;
- the relation disappears within one local update;
- a semantic gate, direction, target, or region is required in code;
- the result cannot be expressed entirely through the Level 0 state and local
  equations above.

Failure is a valid result. The equations must not be revised after a run merely
to make the requested higher-level form appear.

## Implementation boundary

If these equations are accepted, a later inactive candidate may place bounded
tiles of sites in cache-aligned ordinary memory. That mapping does not turn a
site into a cache line or make the equations direct transistor control.

No implementation begins from this document alone.
