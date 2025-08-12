#ifndef METRO_H
#define METRO_H

#include <stdint.h>

#define STATIONS_MAGIC 0x31544E53u // "STN1"
#define STATIONS_VER 1u
#define STATION_ID_LEN 8
#define STATION_NAME_LEN 40
#define STATIONS_MAX 5000

typedef struct
{
    uint32_t magic;
    uint16_t version;
    uint16_t reserved;
    uint32_t count;
} StationsFileHeader;

typedef struct
{
    char id[STATION_ID_LEN];
    char name[STATION_NAME_LEN];
} StationRec;

int load_stations(const char *filename, StationRec *out, int max_count, int *loaded_count);
int append_station(const char *filename, const char *id, const char *name);
void copy_padded(char dst[], size_t dst_len, const char *src);

#endif