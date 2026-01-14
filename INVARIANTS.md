# Sentinel Sim Core — Invariants

This document defines **non-negotiable truths** about the simulation core.
If any invariant is violated, determinism is considered broken.

These are not guidelines. They are rules.

---

## Core Simulation Invariants

### 1. Tick Advancement

- The simulation advances **exactly one tick** per call to `sim_update`.
- No function other than `sim_update` may advance time.
- Time is discrete and integer-indexed.

Violation impact:
- Rollback correctness is undefined.
- Hash comparisons become meaningless.

---

### 2. Deterministic State Only

`SimState` must contain:
- No floating-point values
- No pointers
- No platform-dependent types
- No wall-clock or real-time data

Only fixed-point or integer representations are allowed.

Violation impact:
- Cross-platform determinism is lost.
- Replay and rollback may silently fail.

---

### 3. Fixed-Point Arithmetic

- All arithmetic inside the simulation core must use fixed-point math.
- Floating-point values are allowed **only**:
  - At input boundaries
  - During visualization
- Floats must never be stored in `SimState`.

Violation impact:
- Non-reproducible results across CPUs.
- Hash divergence without visible cause.

---

### 4. Hash as Source of Truth

- Simulation correctness is defined **only** by hash equality.
- Visual output is non-authoritative.
- Logs, screenshots, or intuition do not override hashes.

If hashes match, the simulation is correct.
If hashes differ, the simulation is incorrect.

No exceptions.

---

### 5. Snapshot Integrity

- Snapshots must represent a **complete** simulation state.
- Restoring a snapshot and replaying must produce an identical hash.
- Partial snapshots are forbidden.

Violation impact:
- Rollback convergence cannot be trusted.

---

### 6. Visual Layer Isolation

- Visualization must never mutate `SimState`.
- Visualization must never affect simulation timing.
- Visual code may be removed without changing simulation results.

Violation impact:
- Hidden feedback loops
- Non-deterministic behavior tied to rendering

---

## Intentional Scope Limits

This project does NOT include:
- Physics engines
- Rendering correctness
- Real-time networking
- Floating-point tolerance

Those concerns belong **above** this layer.

---

## Enforcement Philosophy

- Invariants are enforced by:
  - Tests
  - Hash verification
  - Explicit failure cases
- Silence is failure.
- Assumptions are bugs.

If an invariant feels restrictive, that is by design.
