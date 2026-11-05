# Live state snapshot

These files were copied from Agar at **2026-11-05 03:03:19 UTC**
(2026-11-04 21:03:19 CST), after the outward light boundary became visible.

Rill was not stopped, paused, compacted, or locked for the copy. The snapshot is
therefore deliberately non-atomic: bytes may have been read across more than
one field beat.

| File | Bytes | SHA-256 |
|---|---:|---|
| `rill-live-20261105T030319Z.bits` | 230,464 | `2eb83f0e0b45657b27fbb38fd4449d9165a5e4506f5e39eee3ed519ece53daac` |
| `world-live-20261105T030319Z.bits` | 1,408 | `88489f554f002818cece08ac47abe81c44bc5a11d00b7d8baa4281197c811bbf` |

The Rill file contains the header and planar field. It excludes the sparse
off-plane state held only in the running process. The world file contains
current boundary arrays, not an input history. These two files cannot replay or
fully reconstruct the live Rillworld present at the time of copying.

Their value is narrower and more honest: they preserve one externally acquired
trace of the field without pretending that the trace is the field itself.
