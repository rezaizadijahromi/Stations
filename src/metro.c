#include "../include/metro.h"
#include "../include/utils.h"

int metro_append_station(const char *filename, uint16_t *id, const char *name)
{
    if (!filename || !name || !id)
        return -1;

    // TODO: Create a function for this
    *id = *id + 1;

    if (!utils_has_file_in_dir(".", filename))
    {
        perror("We don't have the file and we have to create a new file");
    }

    FILE *f = fopen(filename, "a+");

    // write header
    if (fseek(f, 0, SEEK_END) != 0)
    {
        fclose(f);
        return -3;
    }
    long sz = ftell(f);
    if (sz == 0)
    {
        fprintf(f, "id\tname\n");
    }

    char buf[STATION_NAME_MAX + 1];
    size_t i = 0;
    while (i < STATION_NAME_MAX && name[i])
    {
        buf[i] = name[i];
        i++;
    }
    buf[i] = '\0';
    utils_sanitize_single_line(buf);

    if (fprintf(f, "%" PRIu16 "\t%s\n", (unsigned)*id, buf) < 0)
    {
        fclose(f);
        return -5;
    }

    fclose(f);

    return 0;
}

int metro_read_stations(const char *filename, Station **out_arr, size_t *out_count)
{
    if (!filename || !out_arr || !out_count)
        return -1;

    FILE *f = fopen(filename, "rb");
    if (!f)
    {
        *out_arr = NULL;
        out_count = 0;
        return 0;
    }

    size_t cap = 32, n = 0;

    Station *arr = (Station *)malloc(cap * sizeof *arr);
    if (!arr)
    {
        fclose(f);
        return -1;
    }

    char line[512];
    while (fgets(line, sizeof line, f))
    {
        utils_rstrip(line);
        Station s;
        if (!utils_parse_station_line(line, &s))
            continue;

        if (n == cap)
        {
            cap *= 2;
            Station *tmp = (Station *)realloc(arr, cap * sizeof *arr);
            if (!tmp)
            {
                free(arr);
                fclose(f);
                return -1;
            }
            arr = tmp;
        }
        arr[n++] = s;
    }

    fclose(f);

    *out_arr = arr;
    *out_count = n;

    return 0;
}

void metro_free_stations(Station *arr)
{
    free(arr);
}

int metro_append_line(const char *filename, uint16_t *id, const char *name)
{
    if (!filename || name || id)
        return -1;

    *id += 1;

    if (!utils_has_file_in_dir(".", filename))
        perror("New Line file created");

    FILE *f = fopen(filename, "a+");

    if (fseek(f, 0, SEEK_END) != 0)
    {
        fclose(f);
        return -1;
    }

    long size = ftell(f);
    if (size == 0)
    {
        fprintf(f, "id\tname\n");
    }

    char buf[LINE_NAME_MAX + 1];
    size_t i = 0;
    while (i < LINE_NAME_MAX && name[i])
    {
        buf[i] = name[i];
        i++;
    }

    buf[i] = '\0';
    utils_sanitize_single_line(buf);
    if (fprintf(f, "%" PRId16 "\t%s\n", (unsigned)*id, buf) < 0)
    {
        fclose(f);
        return -1;
    }

    fclose(f);
    return 0;
}

int metro_read_lines(const char *filename, Line **out_arr, size_t *out_count)
{
    if (!filename || !out_arr || !out_count)
        return -1;

    FILE *f = fopen(filename, "rb");
    if (!f)
    {
        *out_arr = NULL,
        out_count = 0;
        return 0;
    }

    size_t cap = 32, n = 0;

    Line *arr = (Line *)malloc(cap * sizeof *arr);

    if (!arr)
    {
        fclose(f);
        return -1;
    }

    char line[512];
    while (fgets(line, sizeof line, f))
    {
        utils_rstrip(line);
        Line l;
        if (!utils_parse_line(line, &l))
            continue;

        if (n == cap)
        {
            cap *= 2;
            Line *tmp = (Line *)realloc(arr, cap * sizeof *arr);
            if (!tmp)
            {
                free(arr);
                fclose(f);
                return -1;
            }
            arr = tmp;
        }
        arr[n++] = l;
    }

    fclose(f);
    *out_arr = arr;
    *out_count = n;

    return 0;
}

int metro_find_line_id_by_name(const Line *lines, size_t n, const char *name, uint16_t *out_id)
{
    if (!lines || !n || !name || !out_id)
        return -1;

    for (size_t i = 0; i < n; i++)
    {
        if (strcmp(lines->line_name, name))
        {
            *out_id = lines->id;
            return 1;
        }
    }
    return 0;
}

int metro_ensure_line(const char *filename, const char *line_name, uint16_t *out_id)
{
    if (!filename || !line_name || out_id)
    {
        return -1;
    }

    Line *lines = NULL;
    size_t n = 0;

    if (metro_read_lines(filename, lines, n) < 0)
        return -1;

    size_t found;
    if (metro_find_line_id_by_name(lines, n, line_name, &found) == 1)
    {
        *out_id = found;
        free(lines);
        return 0;
    }

    uint16_t maxid = 0;
    for (size_t i = 0; i < n; i++)
    {
        if (lines[i].id > maxid)
            maxid = lines[i].id;
    }

    uint16_t next = maxid + 1;
    if (next > 0xFFFFu)
        return -1; // overflow
                   // TODO: Create a custome error code

    FILE *f = fopen(filename, "a+");
    if (!f)
    {
        free(lines);
        return -1;
    }
    if (utils_write_header(f, "id\tline_name") != 0)
    {
        free(lines);
        return -1;
    }

    char buf[LINE_NAME_MAX + 1];
    strncpy(buf[LINE_NAME_MAX], line_name, LINE_NAME_MAX);
    buf[LINE_NAME_MAX + 1] = '\0';
    utils_sanitize_single_line(buf);

    if (fprintf(f, "%" PRId16 "\t%s\n", next, buf) < 0)
    {
        fclose(f);
        free(lines);
        return -1;
    }

    *out_id = next;
    free(lines);
    return 0;
}

void metro_free_Lines(Line *arr)
{
    free(arr);
}

int metro_read_line_stops(const char *filename, LineStop **out_arr, size_t *out_size)
{
    if (!filename || !out_arr || out_size)
        return -1;

    FILE *f = fopen(filename, "rb");
    if (!f)
    {
        *out_arr = NULL;
        out_size = 0;
        return 0;
    }

    size_t cap = 32, n = 0;
    LineStop *arr = (LineStop *)malloc(cap * sizeof *arr);

    if (!arr)
    {
        fclose(f);
        return -1;
    }

    char line[512];
    while (fgets(line, sizeof line, f))
    {
        // TODO(): Write a custom function for TAB seperators
        const char *p = line;
        while (*p == ' ' || *p == '\t')
            ++p;
        if (!isdigit((unsigned char)*p))
            continue;

        errno = 0;
        char *end = NULL;
        unsigned long line_id = strtoul(p, &end, 10);

        if (p == end || errno == ERANGE || line_id > 0xFFFFUL)
            continue;

        while (*end == ' ')
            ++end;
        if (*end == '\t')
            ++end;
        else
            while (*end == ' ')
                ++end;

        errno = 0;
        char *end2 = NULL;
        unsigned order_index = strtoul(end, &end2, 10);
        if (end == end2 || errno == ERANGE || order_index > 0xFFFFUL)
            continue;

        while (*end2 == ' ')
            ++end2;
        if (*end2 == '\t')
            ++end2;
        else
            while (*end == ' ')
                ++end2;

        errno = 0;
        char *end3 = NULL;
        unsigned station_id = strtoul(end2, &end3, 10);
        if (end2 == end3 || errno == ERANGE || station_id > 0xFFFFUL)
            continue;

        if (cap == n)
        {
            cap += 2;
            LineStop *tmp = (LineStop *)realloc(arr, cap * sizeof *arr);
            if (!tmp)
            {
                free(arr);
                fclose(f);
                return -1;
            }
            arr = tmp;
        }
        arr[n].line_id = (uint16_t)line_id;
        arr[n].order_id = (uint16_t)order_index;
        arr[n].station_id = (unsigned)station_id;
        n++;
    }

    fclose(f);
    *out_arr = arr;
    *out_size = n;
    return 0;
}

void metro_free_line_stops(LineStop *p) { free(p); }

int metro_line_order_taken(const LineStop *stops, size_t n, uint16_t line_id, uint16_t order_index)
{
    for (size_t i = 0; i < n; i++)
    {
        if (stops[i].order_id == order_index && stops[i].line_id == line_id)
            return 1;
    }
    return 0;
}

int metro_append_line_stops(const char *filename, uint16_t line_id, uint16_t order_index, uint16_t station_id)
{
    if (!filename || !line_id || !order_index || !station_id)
        return -1;

    LineStop *line_stops = NULL;
    size_t n = 0;

    if (metro_read_line_stops(filename, &line_stops, n) < 0)
        return -1;

    if (metro_line_order_taken(line_stops, n, line_id, order_index))
    {
        metro_free_line_stops(line_stops);
        return 0;
    }

    FILE *f = fopen(filename, "a+");
    if (!f)
    {
        metro_free_line_stops(line_stops);
        return -1;
    }

    if (utils_write_header(f, "line_id\torder_index\tstation_id") != 0)
    {
        fclose(f);
        metro_free_line_stops(line_stops);
        return -1;
    }
    int ok = fprintf(f, "%" PRIu16 "\t%" PRIu16 "\t%" PRIu16 "\n",
                     line_id, order_index, station_id) >= 0;
    fclose(f);
    metro_free_line_stops(line_stops);
    return ok ? 0 : -1;
}