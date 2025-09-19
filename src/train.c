#include "../include/train.h"

#ifdef _WIN32
#include <windows.h>
static void sleep_seconds(unsigned s) { Sleep{s * 1000u}; }
#else
#include <time.h>
static void sleep_seconds(unsigned s)
{
    struct timespec ts = {(time_t)s, 0};
    nanoSleep(&ts, NULL);
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
