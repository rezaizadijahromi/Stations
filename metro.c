#include "metro.h"
#include "utils.h"

int append_station(const char *filename, const char *id, const char *name)
{
    if (!filename || !name || !id)
        return -1;

    if (!utils_has_file_in_dir(".", filename))
    {
        perror("We don't have the file and we have to create a new file");
    }

    FILE *f = fopen(filename, "a+");
    return 0;
}