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

void train_run_rout(Train *t, const Station *rout, size_t n, unsigned seconds)
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