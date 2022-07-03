#ifndef ODYSSEY_FILESYSTEM_FILE_COMMON_H
#define ODYSSEY_FILESYSTEM_FILE_COMMON_H

#include <cstdint>

namespace ods
{

typedef struct File
{
    void *data;
    uint32_t size;
} File;

// static void PLATFORM_FREE_FILE_MEMORY(ods::File*);
// static void PLATFORM_FREE_MEMORY(void*);
// static uint32_t PLATFORM_READ_FILE(const char*, ods::File*);
// static uint32_t PLATFORM_WRITE_FILE(const char*, uint32_t, void*);

}

#endif
