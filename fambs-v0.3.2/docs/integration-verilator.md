# Integration with Verilator and Custom Emulators

The suite builds as a single static ELF with no operating system dependencies. Integration requires three pieces:

1. ELF loading
2. CSR support for `rdcycle`
3. stdout capture

## Requirements

Your emulator must:

1. Load the ELF into simulated memory at a suitable base address.
2. Start execution at the binary entry point.
3. Implement the `rdcycle` CSR (0xC00) used by `cycle_counter.h`.
4. Provide a way to capture `printf` output.

The code does not use:

- dynamic allocation
- file I O
- system calls

It is safe for bare metal ISS and Verilator based SoC models.

## Verilator Example

A typical flow looks like:

```sh
# Build the ELF
cd build
make

# Run under your Verilator model
./obj_dir/Vtop +firmware=fambs.elf > results.jsonl
```

Where your top module:

- reads the `+firmware` plusarg
- loads the ELF into memory before releasing reset
- connects a UART or simple print task to C `printf`

Any other ISS can follow the same pattern as long as it maps CSRs and stdout.

## Checking Results

After a run you should see JSON lines similar to:

```json
{"bench":"SVK","cycles":842110,"iters":1000,"notes":"sparse_vector_dot"}
{"bench":"MGT","cycles":299331,"iters":64,"notes":"micro_graph_bfs"}
...
```

You can then run:

```sh
python scripts/parse_results.py results.jsonl
```

to get a simple summary.
