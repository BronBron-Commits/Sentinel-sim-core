
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

```sh
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

```bash
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
