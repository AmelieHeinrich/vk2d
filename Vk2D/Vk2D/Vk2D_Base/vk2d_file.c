#include "vk2d_file.h"

#include "vk2d_log.h"

#include <stdlib.h>

char* vk2d_read_file(const char* path)
{
    FILE* fp = fopen(path, "rb");

    if (!fp)
    {
        vk2d_log_fatal("Vk2D FILE SYSTEM", "Failed to open file!");
        return NULL;
    }

    long currentpos = ftell(fp);
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, currentpos, SEEK_SET);

    u32 filesizepadded = (
        size % 4 == 0 ? size * 4 : (size + 1) * 4) / 4;

    vk2d_new(char* buffer, filesizepadded);
    if (!buffer)
    {
        fclose(fp);
        free(buffer);
        vk2d_log_fatal("Vk2D FILE SYSTEM", "Failed to mem alloc the file buffer!");
        return NULL;
    }

    fread(buffer, size, sizeof(char), fp);
    fclose(fp);

    return buffer;
}

u32* vk2d_read_spirv(const char* path, u32* psize)
{
    FILE* fp = fopen(path, "rb");

    if (!fp)
    {
        vk2d_log_fatal("Vk2D FILE SYSTEM", "Failed to open file!");
        return NULL;
    }

    long currentpos = ftell(fp);
	fseek(fp, 0, SEEK_END);
	long size = ftell(fp);
	fseek(fp, currentpos, SEEK_SET);

    u32 filesizepadded = (
			size % 4 == 0 ? size * 4 : (size + 1) * 4) / 4;

    vk2d_new(char* buffer, filesizepadded);
    if (!buffer)
    {
        fclose(fp);
        free(buffer);
        vk2d_log_fatal("Vk2D FILE SYSTEM", "Failed to mem alloc the file buffer!");
        return NULL;
    }

    fread(buffer, size, sizeof(char), fp);
    fclose(fp);

    for (u32 i = size; i < filesizepadded; i++) {
		buffer[i] = 0;
	}

    *psize = filesizepadded;
    return (u32*)buffer;
}