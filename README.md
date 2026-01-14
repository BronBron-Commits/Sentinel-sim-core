# Sentinel Sim Core

A **deterministic simulation core** with input logging, rewind, replay, and rollback verification.

Sentinel Sim Core is correctness-first by design:
**the same inputs always produce the same state**, bit-for-bit, across runs, machines, and peers.

Determinism is **verified**, not assumed.

---

## What this is

A lockstep-friendly simulation kernel that guarantees:

* Deterministic fixed-step simulation
* Exact snapshot & restore
* Input logging and replay
* Rollback convergence under simulated network delay
* Hash-based correctness verification

All correctness is enforced through **hash equality**, never visual output.

---

## What this is not

* Not a game engine
* Not a physics engine
* Not a renderer
* Not real-time network code

Those layers are intentionally excluded to preserve determinism.

---

## Core principles

* No floating point in simulation state
* No platform headers in `simcore`
* No wall-clock time
* No hidden side effects
* Hash equality is the source of truth

If hashes diverge, the simulation is wrong.

---

## Project layout

```
include/simcore/   # Public, platform-agnostic API
src/               # Core implementation
tests/             # Determinism, rewind, replay, rollback tests
visual/            # Optional SDL/X11 demos (non-authoritative)
```

Consumers include **only**:

```cpp
#include <simcore/simcore.hpp>
```

---

## Build & test

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

All tests must converge on identical hashes.

---

## Visual demos (non-authoritative)

```bash
./build/sentinel-sim-visual
./build/sentinel-sim-side-by-side
```

Side-by-side mode intentionally injects divergence, then proves rollback convergence.

---

## Why this matters

This is the foundation beneath:

* Multiplayer lockstep engines
* Rollback netcode
* Distributed simulations
* Digital twins
* Verification-critical systems

If this layer is wrong, **everything above it is wrong**.

---

## License

MIT

---
