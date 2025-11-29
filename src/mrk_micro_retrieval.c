#include <stdint.h>
#include "bench_common.h"
#include "cycle_counter.h"

#define MRK_MAX_DB   256
#define MRK_MAX_DIM  32
#define MRK_MAX_TOPK 8

static float db[MRK_MAX_DB][MRK_MAX_DIM];
static float query[MRK_MAX_DIM];

static void mrk_init(uint32_t db_size, uint32_t dim) {
    if (db_size > MRK_MAX_DB) db_size = MRK_MAX_DB;
    if (dim > MRK_MAX_DIM) dim = MRK_MAX_DIM;

    for (uint32_t i = 0; i < db_size; ++i) {
        for (uint32_t d = 0; d < dim; ++d) {
            db[i][d] = 0.01f * (float)((i + 5) * (d + 7));
        }
    }
    for (uint32_t d = 0; d < dim; ++d) {
        query[d] = 0.02f * (float)(d + 11);
    }
}

static float mrk_dot(const float *a, const float *b, uint32_t dim) {
    float acc = 0.0f;
    for (uint32_t d = 0; d < dim; ++d) {
        acc += a[d] * b[d];
    }
    return acc;
}

static void mrk_topk(uint32_t db_size, uint32_t dim, uint32_t topk, float *scores_out) {
    if (topk > MRK_MAX_TOPK) topk = MRK_MAX_TOPK;

    float best_scores[MRK_MAX_TOPK];
    uint16_t best_idx[MRK_MAX_TOPK];

    for (uint32_t k = 0; k < topk; ++k) {
        best_scores[k] = -1e30f;
        best_idx[k] = 0;
    }

    for (uint32_t i = 0; i < db_size; ++i) {
        float s = mrk_dot(query, db[i], dim);
        uint32_t pos = topk;
        while (pos > 0 && s > best_scores[pos - 1]) {
            pos--;
        }
        if (pos < topk) {
            for (uint32_t j = topk - 1; j > pos; --j) {
                best_scores[j] = best_scores[j - 1];
                best_idx[j]    = best_idx[j - 1];
            }
            best_scores[pos] = s;
            best_idx[pos]    = (uint16_t)i;
        }
    }

    for (uint32_t k = 0; k < topk; ++k) {
        scores_out[k] = best_scores[k];
    }
}

void run_mrk(const bench_config_t *cfg) {
    uint32_t db_size = cfg->param_a ? cfg->param_a : 256u;
    uint32_t dim     = cfg->param_b ? cfg->param_b : 32u;
    uint32_t topk    = cfg->param_c ? cfg->param_c : 3u;
    uint32_t iters   = cfg->iterations ? cfg->iterations : 256u;

    if (db_size > MRK_MAX_DB) db_size = MRK_MAX_DB;
    if (dim > MRK_MAX_DIM) dim = MRK_MAX_DIM;
    if (topk > MRK_MAX_TOPK) topk = MRK_MAX_TOPK;

    mrk_init(db_size, dim);

    float scores[MRK_MAX_TOPK];
    volatile float sink = 0.0f;

    uint64_t start = rdcycle();
    for (uint32_t i = 0; i < iters; ++i) {
        mrk_topk(db_size, dim, topk, scores);
        sink += scores[0];
    }
    uint64_t end = rdcycle();

    bench_result_t r = {
        .bench = "MRK",
        .cycles = end - start,
        .iterations = iters,
        .notes = "micro_retrieval_topk"
    };
    print_result_json(&r);

    if (sink == 1234.5f) {
        printf("sink %f\n", sink);
    }
}

// Self check placeholder: replace with concrete expectations on first baseline
// bench_check_i("MRK_MICRO_RETRIEVAL", 1, 1);
