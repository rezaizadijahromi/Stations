#include "./include/metro.h"

int main()
{
    uint16_t id_index = 0;
    append_station("stations", &id_index, "Centrale");
    append_station("stations", &id_index, "Gessate");
    append_station("stations", &id_index, "Cernusco");

    return 0;
}