#include <stdint.h>
#include <math.h>
#include "bench_common.h"
#include "cycle_counter.h"

#define MAK_MAX_TOKENS 8
#define MAK_MAX_DIM    32

static float Q[MAK_MAX_TOKENS][MAK_MAX_DIM];
static float K[MAK_MAX_TOKENS][MAK_MAX_DIM];
static float V[MAK_MAX_TOKENS][MAK_MAX_DIM];
static float O[MAK_MAX_TOKENS][MAK_MAX_DIM];
static float scores[MAK_MAX_TOKENS][MAK_MAX_TOKENS];
static float weights[MAK_MAX_TOKENS][MAK_MAX_TOKENS];

static void mak_init(uint32_t tokens, uint32_t dim) {
    if (tokens > MAK_MAX_TOKENS) tokens = MAK_MAX_TOKENS;
    if (dim > MAK_MAX_DIM) dim = MAK_MAX_DIM;

    for (uint32_t t = 0; t < tokens; ++t) {
        for (uint32_t d = 0; d < dim; ++d) {
            float val = 0.01f * (float)((t + 1) * (d + 3));
            Q[t][d] = val;
            K[t][d] = val * 0.9f;
            V[t][d] = val * 1.1f;
        }
    }
}

static void mak_attention(uint32_t tokens, uint32_t dim) {
    for (uint32_t i = 0; i < tokens; ++i) {
        for (uint32_t j = 0; j < tokens; ++j) {
            float acc = 0.0f;
            for (uint32_t d = 0; d < dim; ++d) {
                acc += Q[i][d] * K[j][d];
            }
            scores[i][j] = acc / (float)dim;
        }
    }
    for (uint32_t i = 0; i < tokens; ++i) {
        float maxv = scores[i][0];
        for (uint32_t j = 1; j < tokens; ++j) {
            if (scores[i][j] > maxv) maxv = scores[i][j];
        }
        float sum = 0.0f;
        for (uint32_t j = 0; j < tokens; ++j) {
            float e = scores[i][j] - maxv;
            float v = expf(e);
            weights[i][j] = v;
            sum += v;
        }
        float inv = 1.0f / sum;
        for (uint32_t j = 0; j < tokens; ++j) {
            weights[i][j] *= inv;
        }
    }
    for (uint32_t i = 0; i < tokens; ++i) {
        for (uint32_t d = 0; d < dim; ++d) {
            float acc = 0.0f;
            for (uint32_t j = 0; j < tokens; ++j) {
                acc += weights[i][j] * V[j][d];
            }
            O[i][d] = acc;
        }
    }
}

void run_mak(const bench_config_t *cfg) {
    uint32_t tokens = cfg->param_a ? cfg->param_a : 8u;
    uint32_t dim    = cfg->param_b ? cfg->param_b : 32u;
    uint32_t iters  = cfg->iterations ? cfg->iterations : 256u;

    if (tokens > MAK_MAX_TOKENS) tokens = MAK_MAX_TOKENS;
    if (dim > MAK_MAX_DIM) dim = MAK_MAX_DIM;

    mak_init(tokens, dim);

    volatile float sink = 0.0f;
    uint64_t start = rdcycle();
    for (uint32_t i = 0; i < iters; ++i) {
        mak_attention(tokens, dim);
        sink += O[0][0];
    }
    uint64_t end = rdcycle();

    bench_result_t r = {
        .bench = "MAK",
        .cycles = end - start,
        .iterations = iters,
        .notes = "micro_attention"
    };
    print_result_json(&r);

    if (sink == 9999.0f) {
        printf("sink %f\n", sink);
    }
}

// Self check placeholder: replace with concrete expectations on first baseline
// bench_check_i("MAK_MICRO_ATTENTION", 1, 1);
