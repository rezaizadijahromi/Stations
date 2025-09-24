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
    size_t index;
} Train;

typedef struct
{
    Train *t;
    const Station *route;
    size_t n; // Number of stations
    unsigned period_ms;
    size_t starting_index;
    int loop; // 0 = stop
    int *done_flag;
} TrainCtx;

static pthread_mutex_t g_print_mx = PTHREAD_MUTEX_INITIALIZER;

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
void *train_thread(void *args);