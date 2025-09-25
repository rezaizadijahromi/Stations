#include "../include/metro.h"
#include "../include/seed_m2.h"

static int append_route(const char *stations_file,
                        const char *line_stops_file,
                        uint16_t line_id,
                        const char *const station_names[], size_t count)
{

    LineStop *stops = NULL;
    size_t nstops = 0;
    if (metro_read_line_stops(line_stops_file, &stops, &nstops) < 0)
        return -1;

    for (size_t i = 0; i < count; ++i)
    {
        uint16_t sid = 0;
        if (metro_ensure_stations(stations_file, station_names[i], &sid) != 0)
        {
            metro_free_line_stops(stops);
            return -1;
        }
        if (metro_append_line_stop_next(line_stops_file, stops, nstops, line_id, sid, NULL) != 0)
        {
            metro_free_line_stops(stops);
            return -1;
        }

        LineStop ls = {.line_id = line_id, .order_id = (uint16_t)0, .station_id = sid};
        (void)ls;
        nstops++;
    }
    metro_free_line_stops(stops);
    return 0;
}

int seed_green_line_all(const char *lines_file,
                        const char *stations_file,
                        const char *line_stops_file)
{
    static const char *M2_GESSATE_TO_FAMAGOSTA[] = {
        "Gessate", "Cascina Antonietta", "Gorgonzola", "Villa Pompea", "Bussero",
        "Cassina de' Pecchi", "Villa Fiorita", "Cernusco sul Naviglio",
        "Cascina Burrona", "Vimodrone", "Cascina Gobba",
        "Crescenzago", "Cimiano", "Udine", "Lambrate FS", "Piola",
        "Loreto", "Caiazzo", "Centrale FS", "Gioia", "Garibaldi FS",
        "Moscova", "Lanza", "Cadorna FN", "Sant'Ambrogio", "Sant'Agostino",
        "Porta Genova FS", "Romolo", "Famagosta"};

    uint16_t lid_gessate = 0;

    if (metro_ensure_line(lines_file, "M2 Gessate \xE2\x86\x92 Famagosta", &lid_gessate) != 0)
        return -1;

    if (append_route(stations_file, line_stops_file, lid_gessate,
                     M2_GESSATE_TO_FAMAGOSTA,
                     sizeof M2_GESSATE_TO_FAMAGOSTA / sizeof M2_GESSATE_TO_FAMAGOSTA[0]) != 0)
        return -1;

    return 0;
}