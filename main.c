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
        uint16_t id_index = 0;
        if (metro_append_station("green_line_stations.tsv", &id_index, "Cascina Gobba") != 0)
            return 1;
        if (metro_append_station("green_line_stations.tsv", &id_index, "Crescenzago") != 0)
            return 1;
        if (metro_append_station("green_line_stations.tsv", &id_index, "Cimiano") != 0)
            return 1;
        if (metro_append_station("green_line_stations.tsv", &id_index, "Udine") != 0)
            return 1;
        if (metro_append_station("green_line_stations.tsv", &id_index, "Lambrate FS") != 0)
            return 1;
        if (metro_append_station("green_line_stations.tsv", &id_index, "Piola") != 0)
            return 1;
        if (metro_append_station("green_line_stations.tsv", &id_index, "Loreto") != 0)
            return 1;
        if (metro_append_station("green_line_stations.tsv", &id_index, "Caiazzo") != 0)
            return 1;
        if (metro_append_station("green_line_stations.tsv", &id_index, "Centrale FS") != 0)
            return 1;
        if (metro_append_station("green_line_stations.tsv", &id_index, "Gioia") != 0)
            return 1;
        if (metro_append_station("green_line_stations.tsv", &id_index, "Garibaldi FS") != 0)
            return 1;
        if (metro_append_station("green_line_stations.tsv", &id_index, "Moscova") != 0)
            return 1;
        if (metro_append_station("green_line_stations.tsv", &id_index, "Lanza") != 0)
            return 1;
        if (metro_append_station("green_line_stations.tsv", &id_index, "Cadorna FN") != 0)
            return 1;
        if (metro_append_station("green_line_stations.tsv", &id_index, "Sant'Ambrogio") != 0)
            return 1;
        if (metro_append_station("green_line_stations.tsv", &id_index, "Sant'Agostino") != 0)
            return 1;
        if (metro_append_station("green_line_stations.tsv", &id_index, "Porta Genova FS") != 0)
            return 1;
        if (metro_append_station("green_line_stations.tsv", &id_index, "Romolo") != 0)
            return 1;

        if (metro_read_stations("green_line_stations.tsv", &stations, &n) != 0 || n == 0)
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
