# Future AI Microbench Suite (FAMBS) v0.3.1

Small, deterministic microbenchmarks for evaluating next generation CPU cores under emulator constraints.

This suite models the workload shapes that modern AI systems use at the edge and on prem:

- sparse vector operations (embeddings, sparse activations)
- graph traversal patterns (knowledge graphs, agent reasoning)
- small attention blocks (transformer core)
- vector retrieval (RAG, similarity search)
- irregular control flow (branch predictor stress)
- combined autonomy loop (realistic instruction mix)
- memory bandwidth saturation (cache hierarchy stress)

All kernels are:

- scalar C
- deterministic
- small enough for slow cycle accurate emulators
- bare metal friendly

## Workloads

### 1. Sparse Vector Kernel (SVK)

Sparse dot product over a dense vector.

Stresses:

- indirect loads
- basic cache behavior
- load store throughput

### 2. Micro Graph Traversal (MGT)

Breadth first search on a small synthetic graph.

Stresses:

- pointer chasing
- branch prediction
- queue operations

### 3. Micro Attention Kernel (MAK)

Tiny self attention block with eight tokens and thirty two dimensions (configurable).

Stresses:

- fused multiply add loops
- reduction patterns
- memory reuse

### 4. Micro Retrieval Kernel (MRK)

Brute force vector similarity with top K selection.

Stresses:

- inner product loops
- conditional moves and branches
- small heap like updates

### 5. Pointer Chase Kernel (PCK)

Scrambled pointer chain with data dependent branches.

Stresses:

- speculative execution
- branch predictor
- memory latency

### 6. Memory Bandwidth Kernel (MBW)

Sequential, random, and strided load patterns over a fixed buffer.

Stresses:

- sustained bandwidth
- cache hierarchy
- prefetch effectiveness

### 7. Micro Autonomy Loop (MAL)

Composite loop that calls SVK, MGT, MRK, and MAK in sequence.

Stresses:

- cross kernel cache reuse
- global instruction mix
- simple agent style control flow

## Build

Requires a bare metal RISC V toolchain.

```sh
cd build
make
```

This produces a static ELF binary, for example `fambs.elf`.

## Run

With a generic emulator:

```sh
scripts/run_all.sh path_to_emulator build/fambs.elf > results.jsonl
```

Each benchmark prints one JSON object:

```json
{"bench":"SVK","cycles":842110,"iters":1000,"notes":"sparse_vector_dot"}
```

You can post process these with your own tooling or with `scripts/parse_results.py`.

## Documentation

- `docs/design-rationale.md` explains how each kernel maps to AI era workloads.
- `docs/integration-verilator.md` shows how to plug the ELF into Verilator or custom ISS.
- `docs/reference-results.md` contains baseline JSON lines for a simple ISS.

## License

MIT
