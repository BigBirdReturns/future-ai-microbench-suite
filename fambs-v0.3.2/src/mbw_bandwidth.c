#include <stdint.h>
#include "bench_common.h"
#include "cycle_counter.h"

#define MBW_MAX_SIZE (1024 * 1024)
#define MBW_ARRAY_LEN (MBW_MAX_SIZE / sizeof(uint32_t))

static uint32_t mbw_buf[MBW_ARRAY_LEN];
static uint32_t mbw_idx[MBW_ARRAY_LEN];

static void mbw_init(uint32_t len) {
    if (len > MBW_ARRAY_LEN) len = MBW_ARRAY_LEN;
    for (uint32_t i = 0; i < len; ++i) {
        mbw_buf[i] = i * 17u + 3u;
        mbw_idx[i] = i;
    }
    uint32_t seed = 98765u;
    for (uint32_t i = 0; i < len; ++i) {
        seed = seed * 1664525u + 1013904223u;
        uint32_t j = seed % len;
        uint32_t tmp = mbw_idx[i];
        mbw_idx[i] = mbw_idx[j];
        mbw_idx[j] = tmp;
    }
}

static uint32_t mbw_seq(uint32_t len, uint32_t iters) {
    volatile uint32_t sink = 0;
    for (uint32_t r = 0; r < iters; ++r) {
        for (uint32_t i = 0; i < len; ++i) {
            sink += mbw_buf[i];
        }
    }
    return sink;
}

static uint32_t mbw_rand(uint32_t len, uint32_t iters) {
    volatile uint32_t sink = 0;
    for (uint32_t r = 0; r < iters; ++r) {
        for (uint32_t i = 0; i < len; ++i) {
            sink += mbw_buf[mbw_idx[i]];
        }
    }
    return sink;
}

static uint32_t mbw_stride(uint32_t len, uint32_t stride, uint32_t iters) {
    volatile uint32_t sink = 0;
    if (stride == 0) stride = 16u;
    for (uint32_t r = 0; r < iters; ++r) {
        for (uint32_t i = 0; i < len; i += stride) {
            sink += mbw_buf[i];
        }
    }
    return sink;
}

void run_mbw(const bench_config_t *cfg) {
    uint32_t size_bytes = cfg->param_a ? cfg->param_a : (256u * 1024u);
    uint32_t iters      = cfg->iterations ? cfg->iterations : 32u;

    uint32_t len = size_bytes / sizeof(uint32_t);
    if (len > MBW_ARRAY_LEN) len = MBW_ARRAY_LEN;
    if (len == 0) len = 1024u;

    mbw_init(len);

    uint64_t start, end;
    uint32_t sink;

    start = rdcycle();
    sink = mbw_seq(len, iters);
    end = rdcycle();
    bench_result_t r_seq = {
        .bench = "MBW",
        .cycles = end - start,
        .iterations = iters * len,
        .notes = "sequential"
    };
    print_result_json(&r_seq);

    start = rdcycle();
    sink ^= mbw_rand(len, iters);
    end = rdcycle();
    bench_result_t r_rand = {
        .bench = "MBW",
        .cycles = end - start,
        .iterations = iters * len,
        .notes = "random"
    };
    print_result_json(&r_rand);

    start = rdcycle();
    sink ^= mbw_stride(len, 16u, iters);
    end = rdcycle();
    bench_result_t r_str = {
        .bench = "MBW",
        .cycles = end - start,
        .iterations = iters * (len / 16u),
        .notes = "strided_64B"
    };
    print_result_json(&r_str);

    if (sink == 0xFFFFFFFFu) {
        printf("sink %u\n", sink);
    }
}

// Self check placeholder: replace with concrete expectations on first baseline
// bench_check_i("MBW_BANDWIDTH", 1, 1);
