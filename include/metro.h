#ifndef METRO_H
#define METRO_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>

#define STATION_NAME_MAX 100

typedef struct
{
    uint16_t id;
    char station_name[STATION_NAME_MAX];
} Station;

int metro_append_station(const char *filename, uint16_t *id, const char *name);
int metro_read_stations(const char *filename, Station **out_arr, size_t *out_size);
void metro_free_stations(Station *arr);

#endif