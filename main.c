#include "./include/metro.h"
#include "./include/train.h"
#include "./include/os.h"
#include "./include/seed_m2.h"

// #include <stdio.h>
// #include <stdlib.h>

int main(void)
{

    const char *LINES = "lines.tsv";
    const char *STAS = "stations.tsv";
    const char *STOPS = "line_stops.tsv";

    if (seed_green_line_all(LINES, STAS, STOPS) < 0)
    {
        perror("Something went wrong in seed green");
    }

    // Load tables into memory
    Line *lines = NULL;
    size_t nlines = 0;
    Station *stations = NULL;
    size_t nstas = 0;
    LineStop *stops = NULL;
    size_t nstops = 0;
    if (metro_read_lines(LINES, &lines, &nlines) != 0 ||
        metro_read_stations(STAS, &stations, &nstas) != 0 ||
        metro_read_line_stops(STOPS, &stops, &nstops) != 0)
    {
        fprintf(stderr, "read failed\n");
        return 1;
    }

    uint16_t line_id = 0;

    if (metro_find_line_id_by_name(lines, nlines, "M2 Gessate \xE2\x86\x92 Famagosta", &line_id) != 1)
    {
        fprintf(stderr, "line not found\n");
        metro_free_lines(lines);
        metro_free_stations(stations);
        metro_free_line_stops(stops);
        return 1;
    }

    uint16_t *route_ids = NULL;
    size_t route_len = 0;
    if (metro_get_line_station_ids(stops, nstops, line_id, &route_ids, &route_len) != 0 || route_len == 0)
    {
        fprintf(stderr, "empty route\n");
        metro_free_lines(lines);
        metro_free_stations(stations);
        metro_free_line_stops(stops);
        return 1;
    }

    Train t1 = {.id = 1, .line_id = line_id, .index = 0, .dir = +1};
    Train t2 = {.id = 2, .line_id = line_id, .index = 3, .dir = +1};
    Train t3 = {.id = 3, .line_id = line_id, .index = route_len - 1, .dir = -1};

    trainCtx ctxs[] = {
        {.route_ids = route_ids, .route_len = route_len, .stations = stations, .stations_n = nstas, .t = &t1, .period_ms = 1, .loop = 0},
        {.route_ids = route_ids, .route_len = route_len, .stations = stations, .stations_n = nstas, .t = &t2, .period_ms = 10, .loop = 0},
        {.route_ids = route_ids, .route_len = route_len, .stations = stations, .stations_n = nstas, .t = &t3, .period_ms = 3, .loop = 0},
    };

    const size_t T = sizeof ctxs / sizeof ctxs[0];
    pthread_t tids[T];
    for (size_t i = 0; i < T; i++)
    {
        if (pthread_create(&tids[i], NULL, train_thread, &ctxs[i]) != 0)
        {
            perror("pthread_create");
            for (size_t j = 0; j < i; j++)
                pthread_join(tids[j], NULL);
            free(route_ids);
            metro_free_line_stops(stops);
            metro_free_stations(stations);
            metro_free_lines(lines);
            return 1;
        }
    }

    for (size_t i = 0; i < T; i++)
        pthread_join(tids[i], NULL);

    free(route_ids);
    metro_free_line_stops(stops);
    metro_free_stations(stations);
    metro_free_lines(lines);

    return 0;
}
