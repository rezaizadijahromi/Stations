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

    return 0;
}
