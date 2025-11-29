# Design Rationale

This suite targets early phase CPU design where cores only exist in simulators and emulators. Full model inference is not practical in this environment because a single pass can take hours or days.

Instead of scaling full models down, the suite builds minimal workloads that reflect the core computational structures of modern AI systems.

## Why These Workloads

### Sparse Vector Kernel (SVK)

Sparse operations dominate embedding lookups and attention pre processing. SVK exposes:

- gather style loads
- sparse accumulation
- basic cache and memory throughput

### Micro Graph Traversal (MGT)

Graph based reasoning and retrieval use heavy pointer chasing and irregular control flow. MGT exposes:

- branch prediction behavior
- queue handling
- cache stress from poor locality

### Micro Attention Kernel (MAK)

Attention is the core primitive of transformer models. A very small attention block is enough to reveal:

- inner product throughput
- reduction patterns
- memory reuse efficiency

### Micro Retrieval Kernel (MRK)

Retrieval augmented systems use dense vector similarity and top K selection. MRK exposes:

- inner product loops
- conditional updates
- small heap like behavior

### Pointer Chase Kernel (PCK)

Randomized pointer chains with data dependent branches represent worst case access patterns for predictors and prefetchers. PCK exposes:

- effective load to use latency
- predictor sensitivity
- penalty for mis speculation

### Memory Bandwidth Kernel (MBW)

Sequential, random, and strided loads over a fixed array provide a simple view of:

- sustained bandwidth
- cache hierarchy behavior
- prefetcher strengths and weaknesses

### Micro Autonomy Loop (MAL)

Edge agents and RAG systems often follow a pattern:

1. read or embed input  
2. traverse or retrieve  
3. attend over a small context  
4. decide and write back  

MAL composes SVK, MGT, MRK, and MAK into a single loop to expose cross kernel interactions and cache reuse.

The goal is not realism at full system scale. The goal is to expose the same architectural stress factors in a form that runs comfortably on an emulator.

## Why Scalar First

Scalar kernels expose front end, memory system, and control flow behavior that dominate AI era bottlenecks long before SIMD units are added. This keeps emulator runs fast and isolates core behavior while microarchitecture is still changing.
