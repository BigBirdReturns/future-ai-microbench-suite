#ifndef FAMBS_PERF_EVENT_H
#define FAMBS_PERF_EVENT_H

#include <stdint.h>

/* Simple instrumentation hooks for microarchitectural event tracing.
 *
 * By default these are no-ops. Integrators can redefine the macros
 * or provide their own implementation to connect to ISS trace ports,
 * custom CSRs, or probe buffers.
 *
 * Example integration on an ISS:
 *   - map FAMBS_EVENT_BRANCH_MISPRED to internal branch unit statistics
 *   - log begin/end events with cycle timestamps
 *
 * Example: connect to custom trace CSR
 *   #define FAMBS_TRACE_CSR 0x8F0
 *   #define fambs_perf_region_begin(id, label, cyc) \
 *       __asm__ volatile("csrw %0, %1" :: "i"(FAMBS_TRACE_CSR), "r"((id) | ((cyc) << 8)))
 */

typedef enum {
    FAMBS_EVENT_NONE = 0,
    FAMBS_EVENT_SVK,
    FAMBS_EVENT_MGT,
    FAMBS_EVENT_MAK,
    FAMBS_EVENT_MRK,
    FAMBS_EVENT_PCK,
    FAMBS_EVENT_MBW,
    FAMBS_EVENT_MAL,
    FAMBS_EVENT_USER0,
    FAMBS_EVENT_USER1,
    FAMBS_EVENT_MAX
} fambs_event_id_t;

/* Region hooks.
 * These are deliberately weak and side effect free by default.
 * Architects can replace them with inline asm or ISS callbacks.
 */
static inline void fambs_perf_region_begin(fambs_event_id_t id, const char *label, uint64_t cycle)
{
    (void)id;
    (void)label;
    (void)cycle;
}

static inline void fambs_perf_region_end(fambs_event_id_t id, uint64_t cycle)
{
    (void)id;
    (void)cycle;
}

/* Point hook for one-shot events, for example a branch mispredict
 * or a cache miss sample. Integrators can use this to drive
 * sampling based analysis.
 */
static inline void fambs_perf_point(fambs_event_id_t id, const char *label, uint64_t cycle)
{
    (void)id;
    (void)label;
    (void)cycle;
}

#endif /* FAMBS_PERF_EVENT_H */
