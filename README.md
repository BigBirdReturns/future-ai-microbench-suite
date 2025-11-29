# Future AI Microbench Suite (FAMBS) v0.3.2

Small deterministic microbenchmarks for evaluating next generation CPU cores under emulator constraints.

This suite models the core workload shapes used by modern AI systems at the edge and on prem:

- sparse vector operations
- graph traversal
- small attention blocks
- vector retrieval (RAG-style similarity)
- irregular control flow
- composite autonomy loops
- memory bandwidth saturation

All kernels are:

- scalar C
- deterministic
- bare metal friendly
- small enough for slow cycle accurate emulators

## Why This Matters

Modern RISC-V ISS tools run at 100–500 KIPS. Booting Linux and running full AI workloads can take hours.  
These microbenchmarks complete in seconds while exposing the same architectural behaviors:  
indirect loads, pointer chasing, branch prediction stress, memory reuse, and bandwidth patterns.

This provides meaningful architectural signal long before a full software stack is available.

---

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

Tiny self attention block with eight tokens and thirty two dimensions.

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

- cross kernel cache reuse
- realistic instruction mix
- simple agent style control flow

---

## Who This Is For

- CPU and RISC-V core architects using slow emulators or ISS
- Verification teams needing early architectural signal before full software stacks
- FPGA prototyping teams pre-Linux
- Researchers exploring ISA and microarchitecture changes
- Anyone validating new cores with limited memory models or incomplete runtimes

---

## Build

Requires:

- RISC-V GCC toolchain (for example riscv64-unknown-elf-gcc)
- GNU make

From the `build` directory:

    cd build
    make

This produces a static ELF such as `fambs.elf`.

---

## Run

With a generic emulator:

    scripts/run_all.sh path_to_emulator build/fambs.elf > results.jsonl

Example with Spike:

    spike --isa=rv64gc build/fambs.elf

Example output parsing:

    scripts/parse_results.py results.jsonl

Each benchmark prints one JSON object, for example:

    {"bench":"SVK","cycles":842110,"iters":1000,"notes":"sparse_vector_dot"}

---

## Documentation

See the `docs` directory for:

- design rationale
- Verilator integration notes
- reference results

---

## License

MIT

