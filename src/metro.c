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

void metro_free_Lines(Line *arr)
{
    free(arr);
}