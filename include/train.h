#pragma once
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
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
    int *done_flag;
} TrainCtx;

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
void train_task(void *ctx_v)
{
    TrainCtx *ctx = (TrainCtx *)ctx_v;

    if (ctx->t->index >= ctx->n)
    {
        *(ctx->done_flag) = 1;
        return;
    }

    const Station *s = &ctx->route[ctx->t->index];
    printf("Train %u at: %s (station id=%u)\n", (unsigned)ctx->t->id, s->station_name, (unsigned)s->id);

    ctx->t->index++;

    if (ctx->t->index >= ctx->n)
    {
        *(ctx->done_flag) = 1;
    }
}

int stop_when_done(void *p) { return *(int *)p; }