#include "../include/utils.h"

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

void utils_sanitize_single_line(char *s)
{
    if (!s)
        return;
    for (; *s; s++)
    {
        if (*s == '\n' || *s == '\r' || *s == '\t')
            *s = ' ';
    }
}

int utils_get_last_station_id(const char *filename, uint16_t *last_id)
{
    if (!filename || !last_id)
        return -1;

    FILE *f = fopen(filename, "rb");
    if (!f)
    {
        *last_id = 0;
        return 0;
    }

    if (fseek(f, 0, SEEK_END) != 0)
    {
        fclose(f);
        return -1;
    }
    long sz = ftell(f);
    if (sz <= 0)
    {
        fclose(f);
        *last_id = 0;
        return 0;
    }

    if (fseek(f, 0, SEEK_SET) != 0)
    {
        fclose(f);
        return -1;
    }

    char line[512];
    uint16_t last = 0;
    int found = 0;

    while (fgets(line, sizeof line, f))
    {
        char *p = line;
        while (*p == ' ' || *p == '\t')
            ++p;

        if (!isdigit((unsigned char)*p))
            continue;

        char *ends = NULL;

        unsigned long v = strtoul(p, NULL, 10);
        if (v <= 0xFFFFUL && ends != p)
        {
            last = (uint16_t)v;
            found = 1;
        }
    }

    fclose(f);
    if (!found)
    {
        *last_id = 0;
        return 0;
    }

    *last_id = last;
    return 1;
}