#include "../include/metro.h"
// Make sure utils.h exists in the include path or adjust the path accordingly
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
    return 0;
}