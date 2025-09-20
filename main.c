#include "./include/metro.h"
#include "./include/train.h"
#include "./include/os.h"

// #include <stdio.h>
// #include <stdlib.h>

int main(void)
{
    Station *stations = NULL;
    size_t n = 0;

    int rc = metro_read_stations("stations.tsv", &stations, &n);
    if (rc != 0)
    {
        fprintf(stderr, "Failed to read stations.tsv (rc=%d)\n", rc);
        return 1;
    }

    if (n == 0)
    {
        uint16_t id_index = 1;
        if (metro_append_station("stations.tsv", &id_index, "Centrale") != 0)
            return 1;
        if (metro_append_station("stations.tsv", &id_index, "Gessate") != 0)
            return 1;
        if (metro_append_station("stations.tsv", &id_index, "Cernusco") != 0)
            return 1;

        if (metro_read_stations("stations.tsv", &stations, &n) != 0 || n == 0)
        {
            fprintf(stderr, "Still empty after append?\n");
            return 1;
        }
    }

    for (size_t i = 0; i < n; ++i)
    {
        printf("%u\t%s\n",
               (unsigned)stations[i].id, stations[i].station_name);
    }

    Train t = {.id = 1, .index = 0};
    int done = 0;
    TrainCtx ctx = {.t = &t, .route = stations, .n = n, .done_flag = &done};

    OsScheduler sched;
    os_init(&sched);
    os_add(&sched, train_task, &ctx, 2000);

    os_run_until(&sched, stop_when_done, &done);

    os_free(&sched);
    free(stations);

    return 0;
}
