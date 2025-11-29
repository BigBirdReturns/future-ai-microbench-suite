// Future AI Microbench Suite v0.3.1
// Common definitions and JSON printing utilities.

#ifndef BENCH_COMMON_H
#define BENCH_COMMON_H

#include <stdint.h>
#include <stdio.h>

typedef struct {
    const char *name;
    uint32_t iterations;
    uint32_t param_a;
    uint32_t param_b;
    uint32_t param_c;
} bench_config_t;

typedef struct {
    const char *bench;
    uint64_t cycles;
    uint32_t iterations;
    const char *notes;
} bench_result_t;

static inline void print_result_json(const bench_result_t *r) {
    if (!r || !r->bench) return;
    printf("{"bench":"%s","cycles":%llu,"iters":%u,"notes":"%s"}\n",
           r->bench,
           (unsigned long long)r->cycles,
           r->iterations,
           r->notes ? r->notes : "");
}


// Self check helpers
static inline void bench_check_f(const char *bench, float actual, float expected, float tol) {
    float diff = actual - expected; if (diff < 0) diff = -diff;
    if (diff <= tol) printf("# [PASS] %s sink=%.6f expected=%.6f\n", bench, actual, expected);
    else printf("# [FAIL] %s sink=%.6f expected=%.6f tol=%.6f\n", bench, actual, expected, tol);
}

static inline void bench_check_i(const char *bench, int32_t actual, int32_t expected) {
    if (actual == expected) printf("# [PASS] %s sink=%d expected=%d\n", bench, actual, expected);
    else printf("# [FAIL] %s sink=%d expected=%d\n", bench, actual, expected);
}

#endif
