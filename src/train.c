#include "../include/train.h"

#ifdef _WIN32
#include <windows.h>
static void sleep_seconds(unsigned s) { Sleep(s * 1000u); }
#else
#include <time.h>
static void sleep_seconds(unsigned s)
{
    struct timespec ts = {(time_t)s, 0};
    nanosleep(&ts, NULL);
}
#endif

static const Station *by_id(const Station *arr, size_t n, uint16_t id)
{
    for (size_t i = 0; i < n; i++)
    {
        if (arr[i].id == id)
            return &arr[i];
    }
    return NULL;
}

void *train_thread(void *arg)
{
    trainCtx *ctx = (trainCtx *)arg;
    if (!ctx || !ctx->t || !ctx->route_ids || ctx->route_len == 0)
        return 0;

    if (ctx->t->index >= ctx->route_len)
        ctx->t->index = 0;
    if (ctx->t->dir != -1 && ctx->t->dir != 1)
        ctx->t->dir = 1;

    for (;;)
    {
        uint16_t sid = ctx->route_ids[ctx->t->index]; // get station id
        const Station *s = by_id(ctx->stations, ctx->stations_n, sid);

        pthread_mutex_lock(&g_print_mx);
        printf("[ Train %u | line %u] at: %s (station_id=%u, idx=%u)\n", (unsigned)ctx->t->id, (unsigned)ctx->t->line_id, s ? s->station_name : "<?>", (unsigned)sid, ctx->t->index);

        fflush(stdout);
        pthread_mutex_unlock(&g_print_mx);

        // we have to use seconds
        sleep_seconds(ctx->period_ms);

        if (ctx->t->dir > 0)
        {
            if (ctx->t->index + 1 < ctx->route_len)
            {
                ctx->t->index++;
            }
            else
            {
                if (ctx->loop)
                    ctx->t->index = 0;
                else
                    break;
            }
        }
        else
        {
            if (ctx->t->index > 0)
            {
                ctx->t->index;
            }
            else
            {
                if (ctx->loop)
                    ctx->t->index = ctx->route_len - 1;
                else
                    break;
            }
        }
    }

    return NULL;
}

int train_move_next(Train *t, size_t n)
{
    if (t->index + 1 < n)
    {
        t->index++;
        return 1;
    }
    return 0;
}

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

int train_stop_when_done(void *p) { return *(int *)p; }
