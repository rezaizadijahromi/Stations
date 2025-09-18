#pragma once
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <errno.h>

#include "metro.h"

int utils_file_exists_readable(const char *path);
int utils_join_path(char *out, size_t out_sz, const char *dir, const char *filename);
int utils_has_file_in_dir(const char *dir, const char *filename);
void utils_sanitize_single_line(char *s);
int utils_get_last_station_id(const char *filename, uint16_t *last_id);

void utils_rstrip(char *s);
int utils_parse_station_line(const char *line, Station *out);