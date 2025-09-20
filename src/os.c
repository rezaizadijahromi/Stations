#include "../include/os.h"

#ifdef _WIN32
#include <windows.h>
static uint64_t now_ms(void) { return GetTickCount64(); }
static void sleep_ms(unsigned ms) { sleep(ms); }
#else
#include <time.h>
#include <linux/time.h>
static uint64_t now_ms(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}

static void sleep_ms(unsigned ms)
{
    struct timespec ts = {ms / 1000, (ms % 1000) * 1000000L};
    nanosleep(&ts, NULL);
}
#endif

void os_init(OsScheduler *s)
{
    s->tasks = NULL;
    s->count = 0;
    s->cap = 0;
}

static int ensure_cap(OsScheduler *s, size_t need)
{
    if (s->cap >= need)
        return 0;

    size_t nc = s->cap ? s->cap * 2 : 4;
    while (nc < need)
        nc *= 2;

    OsTask *tmp = (OsTask *)realloc(s->tasks, nc * sizeof *tmp);
    if (!tmp)
        return -1;
    s->tasks = tmp;
    s->cap = nc;
    return 0;
}

void os_add(OsScheduler *s, OsTaskFn fn, void *ctx, uint32_t period_ms)
{
    if (ensure_cap(s, s->count + 1) != 0)
        return -1;

    OsTask *t = &s->tasks[s->count++];
    t->fn = fn;
    t->ctx = ctx;
    t->period_ms = period_ms;
    t->next_ms = now_ms() + period_ms;
    t->active = 1;
    return (int)(s->count - 1);
}

void os_run_until(OsScheduler *s, int (*stop)(void *), void *stop_ctx)
{
    for (;;)
    {
        if (stop && stop(stop_ctx))
            return;

        uint64_t t = now_ms();
        uint64_t soonest = UINT64_MAX;

        for (size_t i = 0; i < s->count; i++)
        {
            OsTask *task = &s->tasks[i];
            if (!task->active)
                continue;

            if (t >= task->next_ms)
            {
                task->fn(task->ctx);
                task->next_ms += task->period_ms;
            }
            if (task->next_ms < soonest)
                soonest = task->next_ms;
        }

        t = now_ms();
        if (soonest > t)
            sleep_ms((unsigned)(soonest - t));
    }
}

void os_free(OsScheduler *s)
{
    free(s->tasks);
    s->tasks = NULL;
    s->count = s->cap = 0;
}