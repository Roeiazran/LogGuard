#pragma once

namespace logguard::cli {

// Process exit codes (docs/architecture.md §12).
inline constexpr int kExitSuccess = 0;
inline constexpr int kExitRuntimeError = 1;
inline constexpr int kExitUsageError = 2;

}  // namespace logguard::cli
