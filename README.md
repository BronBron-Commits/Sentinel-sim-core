Sentinel Sim Core

A deterministic simulation core with input logging, rewind, replay, and network rollback verification.
This project is intentionally minimal and correctness‑first: every simulation step can be reproduced bit‑for‑bit across runs, machines, and peers.


---

What this is

Sentinel Sim Core is a lockstep‑friendly simulation kernel designed to prove that:

The same inputs always produce the same state

State can be snapshotted and restored exactly

Divergent timelines can be rewound and resimulated

Network delay can be corrected via rollback

Determinism can be verified, not assumed


All correctness is enforced using hash convergence tests, not visual inspection.


---

What this is not

Not a game engine

Not a physics engine

Not a renderer

Not real‑time network code


Those layers are deliberately excluded so determinism is never compromised by timing, floating‑point drift, or platform behavior.


---

Core features

Deterministic simulation

Fixed‑step update loop

Fixed‑point state representation

Bit‑stable hashing of full simulation state


Snapshot & rewind

Capture full simulation state at any tick

Restore state exactly

Verify correctness via hash equality


Input logging & replay

Record time‑stamped input events

Replay logs deterministically

Binary file serialization supported


Network rollback simulation

Two peers simulate independently

Inputs are delayed to simulate latency

Divergence is injected intentionally

Rollback + resimulation restores convergence



---

Why this matters

This is the hard part underneath:

Multiplayer lockstep engines

Rollback netcode (fighting games, RTS)

Distributed simulations

Digital twins

Verification & validation systems

Defense / aerospace / autonomy simulators


If this layer is wrong, everything built on top is wrong.


---

Project layout

include/        # Public simulation headers
src/            # Core implementation
tests/          # Determinism, rewind, replay, rollback tests
visual/         # SDL visualizers (optional, non-authoritative)
build/          # CMake build output


---

Build

cmake -S . -B build
cmake --build build


---

Run core simulation

./build/sentinel-sim-core

Outputs tick count and deterministic state hash.


---

Run tests

ctest --test-dir build --output-on-failure

Tests included:

determinism

rewind

input replay

input file replay

network rollback


All tests must converge on identical hashes.


---

Visual demos (non-authoritative)

These are demonstrations, not validation.

./build/sentinel-sim-visual
./build/sentinel-sim-side-by-side

Side‑by‑side mode intentionally injects divergence, then proves rollback convergence.


---

Why hashes matter

Every test ends with:

left_hash == right_hash

No screenshots. No heuristics. No tolerance.

If the hash matches, the simulation is correct.


---

Comparable systems

Conceptually similar to:

GGPO rollback model (core logic only)

RTS lockstep simulators

Deterministic physics cores

Defense simulation kernels


But implemented as a small, auditable, correctness‑first C++ core.


---

License

MIT
