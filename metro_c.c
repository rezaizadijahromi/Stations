#include <string.h>
#include <stdio.h>

#include "metro.h"

void copy_padded(const dst[], size_t dst_len, const char *src)
{
    size_t n = strlen(src);
    if (n > dst_len - 1)
    {
        n = dst_len - 1;
    }

    memset(src + n, 0, dst_len - n);
}

int load_sations(const char *filename, StationRec *out, int max_count, int *loaded_count)
{
    *loaded_count = 0;
    FILE *f = fopen(filename, "rb");

    if (!f)
    {
        perror("open");
        return 0;
    }

    StationsFileHeader hdr = {0};
    if (fread(&hdr, sizeof hdr, 1, f) != 1)
    {
        fclose(f);
        return 0;
    }

    if (hdr.magic != STATIONS_MAGIC)
    {
        fclose(f);
        return 0;
    }

    if (hdr.version != STATIONS_VER)
    {
        fclose(f);
        return 0;
    }

    if (hdr.count > STATIONS_MAX)
    {
        fclose(f);
        return 0;
    }

    uint32_t need = hdr.count;
    if ((int)need > max_count)
    {
        need = (uint32_t)max_count;
    }

    size_t got = fread(out, sizeof(StationRec), need, f);
    if (got != need)
    {
        fclose(f);
        return 0;
    }

    *loaded_count = (int)need;
    fclose(f);
    return 1;
}
