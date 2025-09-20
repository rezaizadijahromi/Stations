#include "../include/train.h"

#ifdef _WIN32
#include <windows.h>
static void sleep_seconds(unsigned s) { Sleep{s * 1000u}; }
#else
#include <time.h>
static void sleep_seconds(unsigned s)
{
    struct timespec ts = {(time_t)s, 0};
    nanosleep(&ts, NULL);
}
#endif

int train_move_next(Train *t, size_t n)
{
    if (t->index + 1 < n)
    {
        t->index++;
        return 1;
    }
    return 0;
}

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

int train_stop_when_done(void *p) { return *(int *)p; }

void train_run_route(Train *t, const Station *rout, size_t n, unsigned seconds)
{
    if (!rout || n == 0)
        return;

    const Station *cur = train_current(t, rout, n);
    if (cur)
    {
        printf("train %u at station: %s (id=%u)\n", (unsigned)t->id, cur->station_name, (unsigned)cur->id);
    }

    while (train_move_next(t, n))
    {
        sleep_seconds(seconds);
        cur = train_current(t, rout, n);
        printf("train %u at station: %s (id=%u)\n", (unsigned)t->id, cur->station_name, (unsigned)cur->id);
    }
}