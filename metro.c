#
#include <string.h>
#include <stdio.h>

#include "metro.h"

void copy_padded(char dst[], size_t dst_len, const char *src)
{
    size_t n = strlen(src);
    if (n > dst_len - 1)
    {
        n = dst_len - 1;
    }
    memcpy(dst, src, n);
    memset(dst + n, 0, dst_len - n);
}

int load_stations(const char *filename, StationRec *out, int max_count, int *loaded_count)
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
        need = (uint32_t)max_count;

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

static int write_header(FILE *f, uint32_t count)
{
    StationsFileHeader hdr = {
        .magic = STATIONS_MAGIC,
        .version = STATIONS_VER,
        .reserved = 0,
        .count = count};
    if (fseek(f, 0, SEEK_SET) != 0)
        return 0;
    return fwrite(&hdr, sizeof hdr, 1, f) == 1;
}

int append_station(const char *filename, const char *id, const char *name)
{
    FILE *f = fopen(filename, "r+b");
    uint32_t count = 0;

    if (!f)
    {
        f = fopen(filename, "wb");
        if (!f)
            return 0;

        if (!write_header(f, 0))
        {
            fclose(f);
            return 0;
        }

        f = fopen(filename, "r+b");
        if (!f)
            return 0;
    }

    StationsFileHeader hdr = {0};
    if (fread(&hdr, sizeof hdr, 1, f) != 1)
    {
        fclose(f);
        return 0;
    }
    if (hdr.magic != STATIONS_MAGIC || hdr.version != STATIONS_VER)
    {
        fclose(f);
        return 0;
    }
    if (hdr.count >= STATIONS_MAX)
    {
        fclose(f);
        return 0;
    }

    count = hdr.count;

    long data_off = (long)sizeof(StationsFileHeader) + (long)count * (long)sizeof(StationRec);
    if (fseek(f, data_off, SEEK_SET) != 0)
    {
        fclose(f);
        return 0;
    }

    StationRec rec;
    copy_padded(rec.id, STATION_ID_LEN, id);
    copy_padded(rec.name, STATION_NAME_LEN, name);

    if (fwrite(&rec, sizeof rec, 1, f) != 1)
    {
        fclose(f);
        return 0;
    }

    count++;
    if (!write_header(f, count))
    {
        fclose(f);
        return 0;
    }

    fclose(f);
    return 1;
}

int load_line(const char *filename, LineStopRec *out, int max_count, int *loaded_count)
{

    *loaded_count = 0;
    FILE *f = fopen(filename, "rb");

    if (!f)
    {
        perror("open");
        return 0;
    }

    LineFileHeader hdr = {0};
    if (fread(&hdr, sizeof(hdr), 1, f) != 1)
    {
        fclose(f);
        return 0;
    }

    if (hdr.magic != LINE_MAGIC)
    {
        fclose(f);
        return 0;
    }

    if (hdr.version != LINE_VER)
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

    size_t got = fread(out, sizeof(LineStopRec), need, f);
    if (got != need)
    {
        fclose(f);
        return 0;
    }

    *loaded_count = (int)need;
    fclose(f);
    return 1;
}

int append_line_stop(const char *filename, const char *id, const char *name)
{
    FILE *f = fopen(filename, "r+b");
    uint32_t count = 0;

    if (!f)
    {
        f = fopen(filename, "wb");
        if (!f)
        {
            return 0;
        }
        if (write_header(f, 0))
        {
            fclose(f);
            return 0;
        }
        f = fopen(filename, "r+b");
        if (!f)
            return 0;
    }

    LineFileHeader hdr = {0};
    if (fread(&hdr, sizeof hdr, 1, f) != 1)
    {
        fclose(f);
        return 0;
    }

    if (hdr.magic != LINE_MAGIC || hdr.version != LINE_VER)
    {
        fclose(f);
        return 0;
    }

    if (hdr.count >= STATIONS_MAX)
    {
        fclose(f);
        return 0;
    }

    count = hdr.count;

    long data_off = (long)sizeof(LineFileHeader) + (long)count * (long)sizeof(LineStopRec);
    if (fseek(f, data_off, SEEK_SET) != 0)
    {
        fclose(f);
        return 0;
    }

    LineStopRec line;
    copy_padded(line.station_id, LINE);
}