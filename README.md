# Future AI Microbench Suite (FAMBS) v0.3.2

Small deterministic microbenchmarks for evaluating next generation CPU cores under emulator constraints.

This suite models the core workload shapes used by modern AI systems at the edge and on prem:

- sparse vector operations
- graph traversal
- small attention blocks
- vector retrieval (RAG-style similarity)
- irregular control flow
- composite autonomy loops
- bandwidth saturation

All kernels are:
- scalar C
- deterministic
- bare-metal friendly
- small enough for slow cycle-accurate emulators

---

## Workloads

### 1. Sparse Vector Kernel (SVK)
Sparse dot product over a dense vector.

Stresses:
- indirect loads
- basic cache behavior
- load/store throughput

### 2. Micro Graph Traversal (MGT)
Breadth-first search on a small synthetic graph.

Stresses:
- pointer chasing
- branch prediction
- queue operations

### 3. Micro Attention Kernel (MAK)
Tiny self-attention block with 8 tokens and 32 dimensions (configurable).

Stresses:
- fused multiply-add loops
- reduction patterns
- memory reuse

### 4. Micro Retrieval Kernel (MRK)
Brute-force vector similarity with top-K selection.

Stresses:
- inner-product loops
- conditional moves and branches
- small heap-like updates

### 5. Pointer Chase Kernel (PCK)
Scrambled pointer chain with data-dependent branches.

Stresses:
- speculative execution
- branch predictor behavior
- memory latency

### 6. Memory Bandwidth Kernel (MBW)
Sequential, random, and strided load patterns over a fixed buffer.

Stresses:
- sustained bandwidth
- cache hierarchy
- prefetch effectiveness

### 7. Micro Autonomy Loop (MAL)
Composite loop calling SVK, MGT, MRK, and MAK in sequence.

Stresses:
- cross-kernel cache reuse
- realistic instruction mix
- simple agent-style control flow

---

## Build

Requires a bare-metal RISC-V toolchain.

```sh
cd build
make
````

Produces a static ELF such as `fambs.elf`.

---

## Run

```sh
scripts/run_all.sh path_to_emulator build/fambs.elf > results.jsonl
```

---

## Example Output

```json
{"bench":"SVK","cycles":842110,"iters":1000,"notes":"sparse_vector_dot"}
```

---

## Documentation

See `docs/` for:

* design rationale
* Verilator integration
* reference results

---

## License

MIT
