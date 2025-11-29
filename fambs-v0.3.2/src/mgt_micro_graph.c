#include <stdint.h>
#include "bench_common.h"
#include "cycle_counter.h"

#define MGT_MAX_NODES 1024
#define MGT_MAX_EDGES 4096

static uint16_t node_offset[MGT_MAX_NODES + 1];
static uint16_t edge_dst[MGT_MAX_EDGES];
static uint8_t  visited[MGT_MAX_NODES];
static uint16_t queue_buf[MGT_MAX_NODES];

static void mgt_init(uint32_t nodes, uint32_t edges) {
    if (nodes > MGT_MAX_NODES) nodes = MGT_MAX_NODES;
    if (edges > MGT_MAX_EDGES) edges = MGT_MAX_EDGES;

    for (uint32_t i = 0; i <= nodes; ++i) {
        node_offset[i] = 0;
    }

    uint32_t edge_idx = 0;
    for (uint32_t n = 0; n < nodes && edge_idx + 2 <= edges; ++n) {
        node_offset[n] = (uint16_t)edge_idx;
        edge_dst[edge_idx++] = (uint16_t)((n + 1) % nodes);
        edge_dst[edge_idx++] = (uint16_t)((n * 7u + 3u) % nodes);
    }
    node_offset[nodes] = (uint16_t)edge_idx;
}

static uint32_t mgt_bfs(uint32_t start, uint32_t nodes) {
    for (uint32_t i = 0; i < nodes; ++i) visited[i] = 0;

    uint32_t head = 0;
    uint32_t tail = 0;
    queue_buf[tail++] = (uint16_t)start;
    visited[start] = 1;
    uint32_t seen = 1;

    while (head < tail) {
        uint16_t n = queue_buf[head++];
        uint16_t off = node_offset[n];
        uint16_t end = node_offset[n + 1];
        for (uint16_t e = off; e < end; ++e) {
            uint16_t to = edge_dst[e];
            if (!visited[to]) {
                visited[to] = 1;
                queue_buf[tail++] = to;
                seen++;
                if (tail >= nodes) return seen;
            }
        }
    }
    return seen;
}

void run_mgt(const bench_config_t *cfg) {
    uint32_t nodes = cfg->param_a ? cfg->param_a : 512u;
    uint32_t edges = cfg->param_b ? cfg->param_b : 2048u;
    uint32_t iters = cfg->iterations ? cfg->iterations : 64u;

    if (nodes > MGT_MAX_NODES) nodes = MGT_MAX_NODES;
    if (edges > MGT_MAX_EDGES) edges = MGT_MAX_EDGES;

    mgt_init(nodes, edges);

    volatile uint32_t sink = 0;
    uint64_t start = rdcycle();
    for (uint32_t i = 0; i < iters; ++i) {
        sink += mgt_bfs(i % nodes, nodes);
    }
    uint64_t end = rdcycle();

    bench_result_t r = {
        .bench = "MGT",
        .cycles = end - start,
        .iterations = iters,
        .notes = "micro_graph_bfs"
    };
    print_result_json(&r);

    if (sink == 123456u) {
        printf("sink %u\n", sink);
    }
}

// Self check placeholder: replace with concrete expectations on first baseline
// bench_check_i("MGT_MICRO_GRAPH", 1, 1);
