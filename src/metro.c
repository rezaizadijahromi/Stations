#include "../include/metro.h"
#include "../include/utils.h"

int append_station(const char *filename, const char *id, const char *name)
{
    if (!filename || !name || !id)
        return -1;

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

    if (fprintf(f, "%u\t%s\n", (unsigned)id, buf) < 0)
    {
        fclose(f);
        return -5;
    }

    fclose(f);

    return 0;
}