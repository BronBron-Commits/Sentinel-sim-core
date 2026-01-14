#pragma once

// =======================================================
// simcore public API façade
// =======================================================
//
// This is the ONLY header external code should include.
//

#include <simcore/platform_guard.hpp>

// Core types
#include <simcore/sim_state.hpp>
#include <simcore/sim_input.hpp>
#include <simcore/sim_input_log.hpp>

// Core operations
#include <simcore/sim_update.hpp>
#include <simcore/sim_apply_input.hpp>
#include <simcore/sim_run_with_input.hpp>
#include <simcore/sim_initial_state.hpp>

// Snapshot / rollback
#include <simcore/sim_snapshot.hpp>
#include <simcore/sim_snapshot_ops.hpp>

// Utilities
#include <simcore/sim_hash.hpp>
#include <simcore/sim_input_io.hpp>

// Runner
#include <simcore/sim_runner.hpp>
