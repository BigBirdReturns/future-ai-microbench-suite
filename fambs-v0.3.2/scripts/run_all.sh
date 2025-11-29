#!/usr/bin/env sh
# Usage: ./scripts/run_all.sh path_to_emulator path_to_binary > results.jsonl

EMU="$1"
BIN="$2"

if [ -z "$EMU" ] || [ -z "$BIN" ]; then
  echo "Usage: $0 path_to_emulator path_to_binary" 1>&2
  exit 1
fi

"$EMU" "$BIN"
