#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

typedef void (*OsTaskFn)(void *ctx);
typedef struct
{
    OsTaskFn fn;
    void *ctx;
    uint32_t period_ms;
    uint64_t next_ms;
    int active;
} OsTask;

typedef struct
{
    OsTask *tasks;
    size_t count;
    size_t cap;
} OsScheduler;

void os_init(OsScheduler *s);
int os_add(OsScheduler *s, OsTaskFn fn, void *ctx, uint32_t period_ms);
void os_run_until(OsScheduler *s, int (*stop)(void *), void *stop_ctx);
void os_free(OsScheduler *s);