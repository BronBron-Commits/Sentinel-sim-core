Sentinel Sim Core

A deterministic simulation core with verified snapshotting, rewind, input replay, and network rollback semantics.

This project is not a game engine. It is a correctness-first simulation kernel designed to prove that a simulation can:

Produce identical results across runs

Rewind and replay state exactly

Serialize and replay inputs deterministically

Resolve delayed / out-of-order network inputs via rollback

Detect and surface divergence precisely


All guarantees are enforced by executable tests.


---

Why This Exists

Many real-time systems depend on simulations that must not drift:

Multiplayer games with rollback netcode

Defense and aerospace simulations

Robotics and autonomy modeling

Physics-driven control systems

Distributed simulations with partial information


Visual fidelity is secondary. Correctness is the product.

Sentinel Sim Core exists to demonstrate that correctness can be:

Measured

Tested

Proven

Visualized



---

Core Guarantees

The simulation core (simcore) guarantees:

1. Determinism
Identical initial state + identical inputs ⇒ identical final state hash


2. Snapshot / Rewind
State can be snapshotted and restored with zero loss


3. Input Replay
Input streams can be replayed in-memory or from disk with identical results


4. Network Rollback
Late-arriving inputs trigger rollback and resimulation, converging to the same state across peers



These are not claims — they are verified by tests.


---

Project Structure

include/        # Public simulation interfaces
src/            # Simulation core implementation
visual/         # SDL-based visual observers (non-authoritative)
tests/          # Executable correctness tests
build/          # CMake build output

Key design rule:

> Visuals never influence simulation state.



They only observe verified results.


---

Executables

Core Simulation

./build/sentinel-sim-core

Runs a fixed deterministic simulation and prints a state hash.


---

Test Suite (Correctness Proof)

ctest --test-dir build --output-on-failure

Tests include:

determinism – identical runs produce identical hashes

rewind – snapshot and restore correctness

input_replay – deterministic input playback

input_file_replay – file-backed replay correctness

rollback_network – simulated network delay + rollback convergence


All tests must pass for the system to be considered valid.


---

Visual Observers

Single Simulation View

./build/sentinel-sim-visual

Renders a single deterministic simulation run.


---

Side-by-Side Rollback Visualization

./build/sentinel-sim-side-by-side

Runs two peers side-by-side. Optionally injects divergence to demonstrate rollback correction.

Outputs state hashes to confirm convergence.


---

What This Is (and Is Not)

This is:

A simulation correctness harness

A rollback-capable deterministic core

A verification-focused system


This is not:

A full game engine

A rendering framework

A physics showcase


Graphics exist only to
