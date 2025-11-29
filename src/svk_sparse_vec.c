#include <stdint.h>
#include "bench_common.h"
#include "cycle_counter.h"

#define SVK_MAX_NNZ 256
#define SVK_DIM     2048

static uint16_t svk_indices[SVK_MAX_NNZ];
static float    svk_values[SVK_MAX_NNZ];
static float    svk_dense[SVK_DIM];

static void svk_init(uint32_t dim, uint32_t nnz) {
    if (dim > SVK_DIM) dim = SVK_DIM;
    if (nnz > SVK_MAX_NNZ) nnz = SVK_MAX_NNZ;

    for (uint32_t i = 0; i < dim; ++i) {
        svk_dense[i] = 0.1f * (float)((i % 7) + 1);
    }
    for (uint32_t k = 0; k < nnz; ++k) {
        svk_indices[k] = (uint16_t)((k * 17u) % dim);
        svk_values[k]  = 0.01f * (float)(k + 1);
    }
}

static float svk_dot(uint32_t nnz) {
    float acc = 0.0f;
    for (uint32_t k = 0; k < nnz; ++k) {
        uint16_t idx = svk_indices[k];
        acc += svk_values[k] * svk_dense[idx];
    }
    return acc;
}

void run_svk(const bench_config_t *cfg) {
    uint32_t dim   = cfg->param_a ? cfg->param_a : SVK_DIM;
    uint32_t nnz   = cfg->param_b ? cfg->param_b : SVK_MAX_NNZ;
    uint32_t iters = cfg->iterations ? cfg->iterations : 1000u;

    svk_init(dim, nnz);

    volatile float sink = 0.0f;
    uint64_t start = rdcycle();
    for (uint32_t i = 0; i < iters; ++i) {
        sink += svk_dot(nnz);
    }
    uint64_t end = rdcycle();

    bench_result_t r = {
        .bench = "SVK",
        .cycles = end - start,
        .iterations = iters,
        .notes = "sparse_vector_dot"
    };
    print_result_json(&r);

    if (sink == 123456.0f) {
        printf("sink %f\n", sink);
    }
}

// Self check placeholder: replace with concrete expectations on first baseline
// bench_check_i("SVK_SPARSE_VEC", 1, 1);
