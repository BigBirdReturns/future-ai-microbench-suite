#include <stdint.h>
#include "bench_common.h"
#include "cycle_counter.h"

void run_svk(const bench_config_t *cfg);
void run_mgt(const bench_config_t *cfg);
void run_mrk(const bench_config_t *cfg);
void run_mak(const bench_config_t *cfg);

void run_mal(const bench_config_t *cfg) {
    uint32_t iters = cfg->iterations ? cfg->iterations : 64u;

    bench_config_t svk_cfg = { .name = "SVK", .iterations = 100, .param_a = 1024, .param_b = 64, .param_c = 0 };
    bench_config_t mgt_cfg = { .name = "MGT", .iterations = 8,   .param_a = 256,  .param_b = 1024, .param_c = 0 };
    bench_config_t mrk_cfg = { .name = "MRK", .iterations = 16,  .param_a = 128,  .param_b = 32,   .param_c = 3 };
    bench_config_t mak_cfg = { .name = "MAK", .iterations = 8,   .param_a = 8,    .param_b = 32,   .param_c = 0 };

    uint64_t start = rdcycle();
    for (uint32_t i = 0; i < iters; ++i) {
        run_svk(&svk_cfg);
        run_mgt(&mgt_cfg);
        run_mrk(&mrk_cfg);
        run_mak(&mak_cfg);
    }
    uint64_t end = rdcycle();

    bench_result_t r = {
        .bench = "MAL",
        .cycles = end - start,
        .iterations = iters,
        .notes = "micro_autonomy_loop"
    };
    print_result_json(&r);
}

// Self check placeholder: replace with concrete expectations on first baseline
// bench_check_i("MAL_MICRO_AUTONOMY", 1, 1);
