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

static inline void train_start(Train *t, uint16_t id)
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