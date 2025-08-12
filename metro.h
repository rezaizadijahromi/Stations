#ifndef METRO_H
#define METRO_H

#include <stdint.h>

#define STATIONS_MAGIC 0x31544E53u // "STN1"
#define STATIONS_VER 1u
#define STATION_ID_LEN 8
#define STATION_NAME_LEN 40
#define STATIONS_MAX 5000

#define LINE_MAGIC 0x314E494Cu
#define LINE_VER 1u
#define LINE_MAX_STOPS 5000

typedef struct
{
    uint32_t magic;
    uint16_t version;
    uint16_t reserved;
    uint32_t count;
} StationsFileHeader;

typedef struct
{
    uint32_t magic;    // LINE_MAGIC
    uint16_t version;  // LINE_VER
    uint16_t reserved; // 0
    uint32_t count;    // number of LineStopRec that follow
} LineFileHeader;

typedef struct
{
    char id[STATION_ID_LEN];
    char name[STATION_NAME_LEN];
} StationRec;

typedef struct
{
    char station_id[STATION_ID_LEN];
    uint16_t travel_to_next_min;
    uint16_t dwell_ming;
} LineStopRec;

int load_stations(const char *filename, StationRec *out, int max_count, int *loaded_count);
int append_station(const char *filename, const char *id, const char *name);
void copy_padded(char dst[], size_t dst_len, const char *src);

int load_line(const char *filename, LineStopRec *out, int max_count, int *loaded_count);
int append_line_stop(const char *filename, const char *id, const char *name);
int find_station_index(const StationRec *stations, int station_count, const char *id);

#endif