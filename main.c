#include "./include/metro.h"
#include "./include/train.h"
#include "./include/os.h"

// #include <stdio.h>
// #include <stdlib.h>

int main(void)
{
    const char *route_file = "green_line_stations.tsv";

    Station *stations = NULL;
    size_t n = 0;

    int rc = metro_read_stations("green_line_stations.tsv", &stations, &n);
    if (rc != 0)
    {
        fprintf(stderr, "Failed to read stations.tsv (rc=%d)\n", rc);
        return 1;
    }

    if (n == 0)
    {
        uint16_t id_index = 0;
        if (metro_append_station(route_file, &id_index, "Cascina Gobba") != 0)
            return 1;
        if (metro_append_station(route_file, &id_index, "Crescenzago") != 0)
            return 1;
        if (metro_append_station(route_file, &id_index, "Cimiano") != 0)
            return 1;
        if (metro_append_station(route_file, &id_index, "Udine") != 0)
            return 1;
        if (metro_append_station(route_file, &id_index, "Lambrate FS") != 0)
            return 1;
        if (metro_append_station(route_file, &id_index, "Piola") != 0)
            return 1;
        if (metro_append_station(route_file, &id_index, "Loreto") != 0)
            return 1;
        if (metro_append_station(route_file, &id_index, "Caiazzo") != 0)
            return 1;
        if (metro_append_station(route_file, &id_index, "Centrale FS") != 0)
            return 1;
        if (metro_append_station(route_file, &id_index, "Gioia") != 0)
            return 1;
        if (metro_append_station(route_file, &id_index, "Garibaldi FS") != 0)
            return 1;
        if (metro_append_station(route_file, &id_index, "Moscova") != 0)
            return 1;
        if (metro_append_station(route_file, &id_index, "Lanza") != 0)
            return 1;
        if (metro_append_station(route_file, &id_index, "Cadorna FN") != 0)
            return 1;
        if (metro_append_station(route_file, &id_index, "Sant'Ambrogio") != 0)
            return 1;
        if (metro_append_station(route_file, &id_index, "Sant'Agostino") != 0)
            return 1;
        if (metro_append_station(route_file, &id_index, "Porta Genova FS") != 0)
            return 1;
        if (metro_append_station(route_file, &id_index, "Romolo") != 0)
            return 1;

        if (metro_read_stations(route_file, &stations, &n) != 0 || n == 0)
        {
            fprintf(stderr, "Still empty after append?\n");
            return 1;
        }
    }

    puts("Rout:");
    for (size_t i = 0; i < n; ++i)
        printf("  %u\t%s\n", (unsigned)stations[i].id, stations[i].station_name);

    Train t1 = {.id = 1, .index = 0};
    Train t2 = {.id = 2, .index = 0};
    Train t3 = {.id = 3, .index = 0};

    TrainCtx ctxs[] = {
        {.t = &t1, .route = stations, .n = n, .period_ms = 1500, .starting_index = 0, .loop = 0},
        {.t = &t2, .route = stations, .n = n, .period_ms = 1000, .starting_index = 3, .loop = 0},
        {.t = &t3, .route = stations, .n = n, .period_ms = 2000, .starting_index = 6, .loop = 0}};

    const size_t T = sizeof ctxs / sizeof ctxs[0];

    // swan thread
    pthread_t tids[T];
    for (size_t i = 0; i < T; i++)
    {
        if (pthread_create(&tids[i], NULL, train_thread, &ctxs[i]) != 0)
        {
            perror("pthread_create");
            for (size_t j = 0; j < i; j++)
                pthread_join(tids[j], NULL);
            free(stations);
            return 1;
        }
    }

    for (size_t i = 0; i < T; i++)
        pthread_join(tids[i], NULL);

    int done = 0;

    OsScheduler sched;
    os_init(&sched);
    for (size_t i = 0; i < sizeof(ctxs) / sizeof(ctxs[0]); i++)
    {
        os_add(&sched, train_task, &ctxs[i], 2000);
        os_run_until(&sched, train_stop_when_done, &done);
    }
    os_free(&sched);

    pthread_mutex_destroy(&g_print_mx);
    free(stations);

    return 0;
}
