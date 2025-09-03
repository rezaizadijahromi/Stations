#include <stdio.h>
#include <string.h>

#include "utils.h"

#ifdef _WIN32
#define UTILS_IS_SEP(c) ((c) == '\\' || (c) == '/')
static const char UTILS_SEP = '\\';
#else
#define UTILS_IS_SEP(c) ((c) == '/')
static const char UTILS_SEP = '/';
#endif

int utils_file_exists_readable(const char *path)
{
    if (!path)
        return 0;
    FILE *fp = fopen(path, "rb");
    if (fp)
    {
        fclose(fp);
        return 1;
    }
    return 0;
}

int utils_join_path(char *out, size_t out_sz, const char *dir, const char *filename)
{
    if (!out || out_sz == 0 || !filename)
        return -1;

    if (!dir || !*dir)
    {
        int n = snprintf(out, out_sz, "%s", filename);
        return (n < 0 || (size_t)n >= out_sz) ? -1 : 0;
    }

    size_t L = strlen(dir);
    int needs_sep = !(L && UTILS_IS_SEP(dir[L - 1]));
    char sep_s[2] = {UTILS_SEP, 0};
    int n = snprintf(out, out_sz, "%s%s%s", dir, needs_sep ? sep_s : "", filename);
    return (n < 0 || (size_t)n >= out_sz) ? -1 : 0;
}

int utils_has_file_in_dir(const char *dir, const char *filename)
{
    char path[1024];
    if (utils_join_path(path, sizeof path, dir, filename) != 0)
        return 0;
    return utils_file_exists_readable(path);
}