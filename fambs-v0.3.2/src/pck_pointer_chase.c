#include <stdint.h>
#include "bench_common.h"
#include "cycle_counter.h"

#define PCK_POOL_SIZE 1024
#define PCK_MAX_DEPTH 256

typedef struct {
    uint16_t next;
    int32_t  payload;
} pck_node_t;

static pck_node_t pck_pool[PCK_POOL_SIZE];

static void pck_init(void) {
    uint16_t order[PCK_POOL_SIZE];
    for (uint16_t i = 0; i < PCK_POOL_SIZE; ++i) {
        order[i] = i;
    }
    uint32_t seed = 12345u;
    for (uint16_t i = 0; i < PCK_POOL_SIZE; ++i) {
        seed = seed * 1103515245u + 12345u;
        uint16_t j = (uint16_t)(seed % PCK_POOL_SIZE);
        uint16_t tmp = order[i];
        order[i] = order[j];
        order[j] = tmp;
    }

    for (uint16_t i = 0; i < PCK_POOL_SIZE; ++i) {
        pck_pool[i].payload = (int32_t)(order[i] * 7u);
        pck_pool[i].next = order[(i + 1) % PCK_POOL_SIZE];
    }
}

static int32_t pck_chase(uint16_t start, uint32_t depth) {
    int32_t acc = 0;
    uint16_t idx = start;
    for (uint32_t d = 0; d < depth; ++d) {
        pck_node_t *n = &pck_pool[idx];
        acc += n->payload;
        if (n->payload & 1) {
            idx = n->next;
        } else {
            idx = (uint16_t)((idx + 13u) % PCK_POOL_SIZE);
        }
    }
    return acc;
}

void run_pck(const bench_config_t *cfg) {
    uint32_t depth = cfg->param_a ? cfg->param_a : 128u;
    uint32_t iters = cfg->iterations ? cfg->iterations : 256u;

    if (depth > PCK_MAX_DEPTH) depth = PCK_MAX_DEPTH;

    pck_init();

    volatile int32_t sink = 0;
    uint64_t start = rdcycle();
    for (uint32_t i = 0; i < iters; ++i) {
        sink += pck_chase((uint16_t)(i % PCK_POOL_SIZE), depth);
    }
    uint64_t end = rdcycle();

    bench_result_t r = {
        .bench = "PCK",
        .cycles = end - start,
        .iterations = iters,
        .notes = "pointer_chase"
    };
    print_result_json(&r);

    if (sink == 0x7FFFFFFF) {
        printf("sink %d\n", (int)sink);
    }
}

// Self check placeholder: replace with concrete expectations on first baseline
// bench_check_i("PCK_POINTER_CHASE", 1, 1);
