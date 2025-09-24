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

    int code = seed_green_line_all(LINES, STAS, STOPS);

    printf("%d", code);

        //    Train t1 = {.id = 1, .index = 0};
    //    Train t2 = {.id = 2, .index = 0};
    //    Train t3 = {.id = 3, .index = 0};
    //
    //    TrainCtx ctxs[] = {
    //        {.t = &t1, .route = stations, .n = n, .period_ms = 1, .starting_index = 0, .loop = 0},
    //        {.t = &t2, .route = stations, .n = n, .period_ms = 2, .starting_index = 3, .loop = 0},
    //        {.t = &t3, .route = stations, .n = n, .period_ms = 3, .starting_index = 6, .loop = 0}};
    //
    //    const size_t T = sizeof ctxs / sizeof ctxs[0];
    //
    //    // swan thread
    //    pthread_t tids[T];
    //    for (size_t i = 0; i < T; i++)
    //    {
    //        if (pthread_create(&tids[i], NULL, train_thread, &ctxs[i]) != 0)
    //        {
    //            perror("pthread_create");
    //            for (size_t j = 0; j < i; j++)
    //                pthread_join(tids[j], NULL);
    //            free(stations);
    //            return 1;
    //        }
    //    }
    //
    //    for (size_t i = 0; i < T; i++)
    //        pthread_join(tids[i], NULL);
    //
    //    pthread_mutex_destroy(&g_print_mx);
    //    free(stations);

    return 0;
}
