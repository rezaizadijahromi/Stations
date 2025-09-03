#ifndef METRO_H
#define METRO_H

#include <stdint.h>

#define STATION_NAME_MAX 100

typedef struct
{
    uint16_t id;
    char station_name[STATION_NAME_MAX];
} Station;

int append_station(const char *filename, const char *id, const char *name);

#endif