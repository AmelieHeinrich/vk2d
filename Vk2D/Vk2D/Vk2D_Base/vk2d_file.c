#include "vk2d_file.h"

#include "vk2d_log.h"

#include <stdlib.h>

char* vk2d_read_file(const char* path)
{
    FILE *fp;
    long lSize;
    char *buffer;

    fp = fopen(path, "rb");
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

u32* vk2d_read_spirv(const char* path, u32* psize)
{
    FILE *fp;
    char *buffer;

    fp = fopen(path, "rb");
    if (!fp)
        vk2d_log_fatal("Vk2D FILE SYSTEM", "Failed to open file!");

    long currentpos = ftell(fp);
	fseek(fp, 0, SEEK_END);
	long size = ftell(fp);
	fseek(fp, currentpos, SEEK_SET);

    u32 filesizepadded = (
			size % 4 == 0 ? size * 4 : (size + 1) * 4) / 4;

    vk2d_new(buffer, filesizepadded);
    if (!buffer)
    {
        fclose(fp);
        vk2d_log_fatal("Vk2D FILE SYSTEM", "Failed to mem alloc the file buffer!");
    }

    fread(buffer, size, sizeof(char), fp);
    fclose(fp);

    for (u32 i = size; i < filesizepadded; i++) {
		buffer[i] = 0;
	}

    *psize = filesizepadded;
    return (u32*)buffer;
}