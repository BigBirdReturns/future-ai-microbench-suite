# Reference Results

These are example outputs from a simple RV64GC ISS build with `-O2`. They are not performance targets. They exist only to confirm that:

- benchmarks run to completion
- cycle counting works
- JSON output is well formed

```json
{"bench":"SVK","cycles":842110,"iters":1000,"notes":"sparse_vector_dot"}
{"bench":"MGT","cycles":299331,"iters":64,"notes":"micro_graph_bfs"}
{"bench":"MAK","cycles":560912,"iters":256,"notes":"micro_attention"}
{"bench":"MRK","cycles":410556,"iters":256,"notes":"micro_retrieval_topk"}
{"bench":"PCK","cycles":215774,"iters":256,"notes":"pointer_chase"}
{"bench":"MBW","cycles":601233,"iters":32,"notes":"bandwidth_mix"}
{"bench":"MAL","cycles":1902212,"iters":64,"notes":"micro_autonomy_loop"}
```

Replace these with your own baseline once you have:

```sh
./scripts/run_all.sh your_emulator build/fambs.elf > baseline.jsonl
cp baseline.jsonl docs/baseline.jsonl
```
