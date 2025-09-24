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
    long pos = ftell(f);
    if (pos <= 0)
    {
        fclose(f);
        *last_id = 0;
        return 0;
    }

    while (pos > 0)
    {
        if (fseek(f, --pos, SEEK_SET) != 0)
        {
            fclose(f);
            return -1;
        }
        int ch = fgetc(f);
        if (ch != '\n' && ch != '\r')
            break;
    }

    while (pos > 0)
    {
        if (fseek(f, --pos, SEEK_SET) != 0)
        {
            fclose(f);
            return -1;
        }

        int ch = fgetc(f);
        if (ch == '\n')
        {
            pos++;
            break;
        }
    }

    if (fseek(f, pos, SEEK_SET) != 0)
    {
        fclose(f);
        return -1;
    }

    char line[512];

    if (!fgets(line, sizeof line, f))
    {
        fclose(f);
        *last_id = 0;
        return 0;
    }
    fclose(f);

    char *p = line;
    while (*p == ' ' || *p == '\t')
        ++p;
    if (!isdigit((unsigned char)*p))
    {
        last_id = 0;
        return 0;
    }

    errno = 0;
    unsigned long v = strtoul(p, NULL, 10);
    if (errno || v > 65635)
    {
        *last_id = 0;
        return 0;
    }

    *last_id = (uint16_t)v;
    return 1;
}

void utils_rstrip(char *s)
{
    if (!s)
        return;
    size_t n = strlen(s);
    while (n && (s[n - 1] == '\n' || s[n - 1] == '\r'))
        s[--n] = '\0';
}

int utils_parse_station_line(const char *line, Station *out)
{
    const char *p = line;
    while (*p == ' ' || *p == '\t')
        ++p;

    if (!isdigit((unsigned char)*p))
        return 0;

    errno = 0;
    char *end = NULL;
    unsigned long v = strtoul(p, &end, 10);
    if (p == end || errno == ERANGE || v > 0xFFFUL)
        return 0;

    out->id = (uint16_t)v;
    while (*end == ' ')
        ++end;
    if (*end == '\t')
        ++end;
    else
        while (*end == ' ')
            ++end;

    // Copy name uop to EOL
    char buf[STATION_NAME_MAX];
    size_t i = 0;

    while (i < STATION_NAME_MAX - 1 && end[i] && end[i] != '\n' && end[i] != '\r')
        buf[i] = end[i], ++i;

    buf[i] = '\0';
    utils_sanitize_single_line(buf);
    strncpy(out->station_name, buf, STATION_NAME_MAX);
    out->station_name[STATION_NAME_MAX - 1] = '\0';
    return 1;
}

int utils_parse_line(const char *line, Line *out)
{
    const char *p = line;
    while (*p == ' ' || *p == '\t')
        ++p;

    if (!isdigit((unsigned char)*p))
        return 0;

    errno = 0;
    char *end = NULL;
    unsigned long v = strtoul(p, &end, 10);
    if (p == end || errno == ERANGE || v > 0xFFFUL)
        return 0;

    out->id = (uint16_t)v;
    while (*end == ' ')
        ++end;
    if (*end == '\t')
        ++end;
    else
        while (*end == ' ')
            ++end;

    // Copy name uop to EOL
    char buf[LINE_NAME_MAX];
    size_t i = 0;

    while (i < LINE_NAME_MAX - 1 && end[i] && end[i] != '\n' && end[i] != '\r')
        buf[i] = end[i], ++i;

    buf[i] = '\0';
    utils_sanitize_single_line(buf);
    strncpy(out->line_name, buf, LINE_NAME_MAX);
    out->line_name[LINE_NAME_MAX - 1] = '\0';
    return 1;
}

int utils_write_header(FILE *f, const char *header)
{
    if (!f || !header)
        return -1;

    if (fseek(f, 0, SEEK_END) != 0)
    {
        fclose(f);
        return -1;
    }
    long sz = ftell(f);
    if (sz < 0)
        return -1;
    if (sz == 0)
    {
        if (fprintf(f, "%s\n", header) < 0)
            return -1;
    }
    return 0;
}