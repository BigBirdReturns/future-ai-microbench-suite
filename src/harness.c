#include <stdio.h>
#include "bench_common.h"
#include "cycle_counter.h"

void run_svk(const bench_config_t *cfg);
void run_mgt(const bench_config_t *cfg);
void run_mak(const bench_config_t *cfg);
void run_mrk(const bench_config_t *cfg);
void run_pck(const bench_config_t *cfg);
void run_mbw(const bench_config_t *cfg);
void run_mal(const bench_config_t *cfg);

static bench_config_t make_cfg(uint32_t iters, uint32_t a, uint32_t b, uint32_t c) {
    bench_config_t cfg;
    cfg.name = "";
    cfg.iterations = iters;
    cfg.param_a = a;
    cfg.param_b = b;
    cfg.param_c = c;
    return cfg;
}

int main(void) {
    bench_config_t svk_cfg = make_cfg(1000u, 2048u, 128u, 0u);
    bench_config_t mgt_cfg = make_cfg(64u,   512u,  2048u, 0u);
    bench_config_t mak_cfg = make_cfg(256u,  8u,    32u,   0u);
    bench_config_t mrk_cfg = make_cfg(256u,  256u,  32u,   3u);
    bench_config_t pck_cfg = make_cfg(256u,  128u,  0u,    0u);
    bench_config_t mbw_cfg = make_cfg(32u,   256u * 1024u, 0u, 0u);
    bench_config_t mal_cfg = make_cfg(64u,   0u,    0u,    0u);

    run_svk(&svk_cfg);
    run_mgt(&mgt_cfg);
    run_mak(&mak_cfg);
    run_mrk(&mrk_cfg);
    run_pck(&pck_cfg);
    run_mbw(&mbw_cfg);
    run_mal(&mal_cfg);

    printf("# Complete\n");
    return 0;
}
