#include "vk2d_file.h"

#include "vk2d_log.h"

#include <stdlib.h>

char* vk2d_read_file(const char* path)
{
    FILE *fp;
    long lSize;
    char *buffer;

    fp = fopen(path, "r");
    if (!fp)
        vk2d_log_fatal("Vk2D FILE SYSTEM", "Failed to open file!");
    
    fseek(fp, 0L, SEEK_END);
    lSize = ftell(fp);
    rewind(fp);

    buffer = calloc(1, lSize + 1);
    if (!buffer)
    {
        fclose(fp);
        vk2d_log_fatal("Vk2D FILE SYSTEM", "Failed to mem alloc the file buffer!");
    }

    fread(buffer, lSize, 1, fp);

    fclose(fp);
    return buffer;
}