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
If any hash differs, determinism is broken.

---

## Building

### Requirements
- CMake
- C++17 compiler (tested with Clang)

### Build and Run Tests

```bash
cmake -S . -B build
cmake --build build

./build/test_determinism
./build/test_rewind
./build/test_input_replay
./build/test_input_file_replay

```
---

Intended Use Cases

Simulation engines

Lockstep multiplayer backends

Defense / aerospace simulation

Robotics and control systems

Replay debugging systems

Deterministic testing frameworks



---

Project Status

Stable baseline established.

The deterministic core is complete and validated. Future work will build on this foundation without weakening guarantees.


---

License

MIT

---

## Exact command to write the file (no guessing)

Run this **from the repo root**:

cat > README.md << 'EOF'
# sentinel-sim-core

A **deterministic simulation core** with snapshot/rewind, fixed-point state,
input event logging, binary replay, and hash-verified equivalence.

This project is designed to serve as a **verifiable simulation backbone**
for systems that require reproducibility, rollback, and auditability.

---

## Core Guarantees

This simulation core guarantees that:

- The same initial state + the same input stream **always produces the same result**
- Simulation state can be **snapshotted and restored**
- Inputs can be **logged, serialized, and replayed**
- Replayed runs can be **cryptographically compared** to original runs
- No floating-point nondeterminism (fixed-point arithmetic is used)

These guarantees are enforced by automated tests.

---

## Architecture Overview

**Key components:**

- **SimState**  
  Fixed-point simulation state. No nondeterministic fields.

- **Simulation Update Loop**  
  Advances state in discrete, deterministic ticks.

- **Input Event Log**  
  Timestamped input events applied at exact simulation ticks.

- **Snapshot / Rewind System**  
  Capture and restore full simulation state.

- **Binary Replay**  
  Serialize input logs to disk and replay them exactly.

- **State Hashing**  
  Hash the full simulation state to detect divergence.

---

## Determinism Model

- Fixed time step
- Fixed-point arithmetic
- Ordered input application
- No wall-clock time
- No platform-dependent math

This allows:
- Lockstep simulation
- Replay debugging
- Desync detection
- Formal verification approaches

---

## Tests (Proof of Correctness)

The following tests are included and must pass:

### Determinism Test
Runs the same simulation twice and verifies identical hashes.

### Snapshot / Rewind Test
Runs forward, snapshots, rewinds, and verifies state equivalence.

### Input Replay Test
Applies an input event log and verifies deterministic outcome.

### Binary Input Replay Test
Writes input events to disk, reloads them, replays, and verifies hashes.

Example output:

ticks=2000 hash1=0xcbf29ce484222325 hash2=0xcbf29ce484222325 full=0xcbf29ce484222325 replay=0xcbf29ce484222325 A=0x4521b390dcb54822 B=0x4521b390dcb54822

If any hash differs, determinism is broken.

---

## Building

### Requirements
- CMake
- C++17 compiler (tested with Clang)

### Build and Run Tests

cmake -S . -B build
cmake --build build

./build/test_determinism
./build/test_rewind
./build/test_input_replay
./build/test_input_file_replay


---

Intended Use Cases

Simulation engines

Lockstep multiplayer backends

Defense / aerospace simulation

Robotics and control systems

Replay debugging systems

Deterministic testing frameworks



---

Project Status

Stable baseline established.

The deterministic core is complete and validated. Future work will build on this foundation without weakening guarantees.


---

License

MIT
