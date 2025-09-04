#pragma once
#include <cstdio>
#include <cstring>
int utils_file_exists_readable(const char *path);
int utils_join_path(char *out, size_t out_sz, const char *dir, const char *filename);
int utils_has_file_in_dir(const char *dir, const char *filename);
void utils_sanitize_single_line(char *s);