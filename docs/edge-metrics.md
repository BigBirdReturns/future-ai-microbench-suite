# Edge Metrics Guide

How to interpret FAMBS results for edge and efficiency focused deployments.

## What Edge Systems Care About

Edge and embedded systems usually care less about peak throughput and more about:

1. Latency for batch equal to one
2. Energy and power envelope
3. Memory footprint and bandwidth
4. Predictable behavior under load

FAMBS does not replace full system analysis, but it gives useful signals in these areas.

## Latency

From each benchmark you can derive:

- cycles per iteration
- relative cost between kernels

For an edge pitch, highlight:

- SVK and MRK for simple inference style work
- PCK for worst case latency under irregular access
- MAL as a coarse proxy for an agent loop

## Bandwidth and Locality

MBW reveals:

- sequential versus random throughput
- cost of strided patterns

Report:

- cycles per element for each pattern
- ratio of random to sequential

This says more about edge readiness than a single peak number.

## Instruction Mix and Balance

MAL plus individual kernels show whether:

- control heavy code (MGT, PCK) is comfortable
- math heavy code (MAK, MRK) is comfortable
- mixed loops (MAL) expose hidden stalls

Together, these results let you tell a grounded story about how a core behaves on realistic small AI era workloads at the edge.
