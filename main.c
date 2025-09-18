#include "./include/metro.h"

int main()
{

    if (metro_read_stations("stations.tsv") != 0)
    {
        uint16_t id_index = 0;
        metro_append_station("stations.tsv", &id_index, "Centrale");
        metro_append_station("stations.tsv", &id_index, "Gessate");
        metro_append_station("stations.tsv", &id_index, "Cernusco");
    }
    else
    {
        printf("Load %zu stations\n", metro_station_count());
        for (size_t i = 0; i < metro_station_count(); i++)
        {
            const Station *s = metro_station_at(i);
            printf("%u\t%s\n", (unsigned)s->id, s->station_name);
        }

        metro_free_stations();
    }

    return 0;
}