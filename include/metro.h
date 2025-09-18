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
int metro_read_stations(const char *filename);

// int metro_load_stations(const char *filename);

/* Number of stations currently loaded. */
size_t metro_station_count(void);

/* Get read-only pointer to station at index (0..count-1), or NULL if OOB. */
const Station *metro_station_at(size_t index);

/* Linear search by id. Returns pointer if found, else NULL. */
const Station *metro_find_by_id(uint16_t id);

/* Free the in-memory array. Call at program end or before reloading. */
void metro_free_stations(void);

#endif