#include "./include/metro.h"

int main()
{

    Station *stations = NULL;
    size_t n = 0;

    if (metro_read_stations("stations.tsv", &stations, &n) != 0)
    {
        uint16_t id_index = 0;
        metro_append_station("stations.tsv", &id_index, "Centrale");
        metro_append_station("stations.tsv", &id_index, "Gessate");
        metro_append_station("stations.tsv", &id_index, "Cernusco");
    }
    else
    {
        for (size_t i = 0; i < n; i++)
        {
            printf("%u\t%s\n", (unsigned)stations[i].id, stations[i].station_name);
        }
    }
    metro_free_stations(stations);

    return 0;
}