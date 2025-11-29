#!/usr/bin/env python3
"""Parse and summarize FAMBS JSONL results.

Usage:
  python scripts/parse_results.py results.jsonl
"""

import json
import sys
from collections import defaultdict


def load_results(path):
    results = []
    with open(path, "r") as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith("#"):
                continue
            try:
                obj = json.loads(line)
            except json.JSONDecodeError:
                continue
            if "bench" in obj and "cycles" in obj and "iters" in obj:
                results.append(obj)
    return results


def summarize(results):
    by_bench = defaultdict(list)
    for r in results:
        by_bench[r["bench"]].append(r)

    print("Benchmark summary")
    print("=================")
    for bench, rows in sorted(by_bench.items()):
        total_cycles = sum(int(r["cycles"]) for r in rows)
        total_iters = sum(int(r["iters"]) for r in rows)
        avg = total_cycles / total_iters if total_iters else 0.0
        print(f"{bench:4s}  runs={len(rows):3d}  total_cycles={total_cycles:10d}  avg_cycles_per_iter={avg:10.1f}")


def main():
    if len(sys.argv) < 2:
        print("Usage: parse_results.py results.jsonl", file=sys.stderr)
        sys.exit(1)

    path = sys.argv[1]
    results = load_results(path)
    if not results:
        print("No valid JSON lines found", file=sys.stderr)
        sys.exit(1)

    summarize(results)


if __name__ == "__main__":
    main()
