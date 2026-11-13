# Local mechanics

## Passage

The local half-height is

```text
h(x) = h_throat + (h_open - h_throat)
       * (1 - exp(-((x - x_center) / width)^4))
```

The initial values are:

- `h_open = 7.0`
- `h_throat = 2.25`
- free carrier height = `8.0`

The throat/free-height ratio is therefore `0.5625`.

## Medium transfer

The apparatus supplies a local parabolic transfer field inside the passage:

```text
u_x(x, y) = u_max * max(0, 1 - (y / h(x))^2)
```

No site receives a destination coordinate. The field is a property of the test
apparatus and present location.

## Boundary relation

Adjacent boundary points are joined by Hooke relations:

```text
F_s = k_s * (distance - rest_distance) * unit_direction
```

Each point also receives a local curvature response from its two boundary
neighbors:

```text
F_b = k_b * (previous + next - 2 * current)
```

There is no fixed coordinate template and no global shape target.

## Short-range exclusion

Non-neighboring boundary points, carried particles, dissolved particles, and
passage walls exert bounded repulsion only inside a local cutoff. Equal and
opposite responses are applied where both participants are dynamic.

## Update

The first candidate uses overdamped deterministic motion:

```text
position_next = position + dt * total_local_response
```

This apparatus approximation isolates elementary deformation-and-passage
mechanics before additional fluid layers are introduced.

## Mechanical observations

The final standard-output summary contains only:

- bounded step count;
- carrier centroid;
- carrier width and height;
- maximum adjacent-boundary separation;
- carried-particle retention;
- dissolved-particle passage count;
- a mechanically derived outcome label.

These values report the candidate's mechanics; interpretation remains outside
the runtime.
