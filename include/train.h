#pragma once
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#include "metro.h"

typedef struct
{
    uint16_t id;
    uint16_t line_id;
    size_t index;
    int dir; // forward or backward
} Train;

typedef struct
{
    const uint16_t *route_ids; // station_ids
    size_t route_len;
    const Station *stations;
    size_t stations_n;
    Train *t;
    unsigned period_ms;
    int loop; // wrap around, 0 = stop at end
} trainCtx;

static pthread_mutex_t g_print_mx;

static inline void
train_start(Train *t, uint16_t id)
{
    t->id = id;
    t->index = 0;
}

static inline const Station *train_current(const Train *t, const Station *rout, size_t n)
{
    return (t->index < n) ? &rout[t->index] : NULL;
}

int train_move_next(Train *t, size_t n);
void train_run_route(Train *t, const Station *rout, size_t n, unsigned seconds);

// Tasks
void train_task(void *ctx_v);
int train_stop_when_done(void *p);

#ifdef __cplusplus
extern "C"
{
#endif
    void *train_thread(void *args);
#ifdef __cplusplus
}
#endif