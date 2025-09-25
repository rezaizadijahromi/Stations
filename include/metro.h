#ifndef METRO_H
#define METRO_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>

#define STATION_NAME_MAX 100
#define LINE_NAME_MAX 100

typedef struct
{
    uint16_t id;
    char station_name[STATION_NAME_MAX];
} Station;

typedef struct
{
    uint16_t id;
    char line_name[LINE_NAME_MAX];
} Line;

typedef struct
{
    uint16_t line_id;
    uint16_t order_id;
    uint16_t station_id;
} LineStop;

int metro_append_station(const char *filename, uint16_t *id, const char *name);
int metro_find_stations_id_by_name(const Station *stations, size_t n, const char *name, uint16_t *out_id);
int metro_ensure_stations(const char *filename, const char *name, uint16_t *id);
int metro_read_stations(const char *filename, Station **out_arr, size_t *out_size);
void metro_free_stations(Station *arr);

int metro_append_line(const char *filename, uint16_t *id, const char *name);
int metro_read_lines(const char *filename, Line **out_arr, size_t *out_size);
int metro_find_line_id_by_name(const Line *lines, size_t n, const char *name, uint16_t *out_id);
int metro_ensure_line(const char *filename, const char *line_name, uint16_t *out_id);
void metro_free_lines(Line *arr);

void metro_free_line_stops(LineStop *p);
int metro_read_line_stops(const char *filename, LineStop **out_arr, size_t *out_size);
int metro_append_line_stops(const char *filename, uint16_t line_id, uint16_t order_index, uint16_t station_id);
int metro_line_order_taken(const LineStop *stops, size_t n, uint16_t line_id, uint16_t order_index);
int metro_append_line_stop_next(const char *file, const LineStop *stops, size_t nstops, uint16_t line_id, uint16_t station_id, uint16_t *out_order_index);
int metro_get_line_station_ids(const LineStop *stops, size_t nstops, uint16_t line_id, uint16_t **out_ids, size_t *out_n);

#endif