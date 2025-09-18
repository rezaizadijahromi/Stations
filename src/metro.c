#include "../include/metro.h"
#include "../include/utils.h"

static Station *g_stations = NULL;
static size_t g_count = 0;
static size_t g_cap = 0;

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

int metro_read_stations(const char *filename)
{
    if (!filename)
        return -1;
    FILE *f = fopen(filename, "rb");
    if (!f)
    {
        free(g_stations);
        g_stations = NULL;
        g_count = g_cap = 0;
    }
    free(g_stations);
    g_stations = NULL;
    g_count = g_cap = 0;

    g_cap = 32;
    g_stations = (Station *)malloc(g_cap * sizeof *g_stations);
    if (!g_stations)
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

        if (g_count == g_cap)
        {
            size_t new_cap = g_cap * 2;
            Station *tmp = (Station *)realloc(g_stations, new_cap * sizeof *g_stations);
            if (!tmp)
            {
                fclose(f);
                metro_free_stations();
                return -1;
            }
            g_stations = tmp;
            g_cap = new_cap;
        }
        g_stations[g_count++] = s;
    }
    fclose(f);
    return 0;
}

size_t metro_station_count(void)
{
    return g_count;
}

const Station *metro_station_at(size_t index)
{
    if (index >= g_count)
        return NULL;
    return &g_stations[index];
}

const Station *metro_find_by_id(uint16_t id)
{
    for (size_t i = 0; i < g_count; ++i)
        if (g_stations[i].id == id)
            return &g_stations[i];
    return NULL;
}

void metro_free_station(void)
{
    free(g_stations);
    g_stations = NULL;
    g_count = g_cap = 0;
}