#!/usr/bin/env bash
# Format all tracked C++ files with clang-format (style from .clang-format).
#   scripts/format.sh          rewrite files in place
#   scripts/format.sh --check  only report unformatted files (used by CI); exits 1 if any
set -euo pipefail

cd "$(git rev-parse --show-toplevel)"

CLANG_FORMAT="${CLANG_FORMAT:-clang-format}"
if ! command -v "$CLANG_FORMAT" >/dev/null 2>&1; then
    echo "error: $CLANG_FORMAT not found (macOS: brew install llvm; Ubuntu: apt install clang-format)" >&2
    exit 1
fi

list_cpp_files() { git ls-files -- '*.cpp' '*.hpp' '*.h' '*.cc'; }
files=()
while IFS= read -r file; do files+=("$file"); done < <(list_cpp_files)

if [[ ${#files[@]} -eq 0 ]]; then
    echo "No C++ files found."
    exit 0
fi

if [[ "${1:-}" == "--check" ]]; then
    "$CLANG_FORMAT" --dry-run --Werror "${files[@]}"
    echo "Formatting OK (${#files[@]} files)."
else
    "$CLANG_FORMAT" -i "${files[@]}"
    echo "Formatted ${#files[@]} files."
fi
