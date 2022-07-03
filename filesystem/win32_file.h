#ifndef ODYSSEY_FILESYSTEM_WIN32_FILE_H
#define ODYSSEY_FILESYSTEM_WIN32_FILE_H

#include "file_common.h"

#include <windows.h>

static void PLATFORM_FREE_FILE_MEMORY(ods::File *file)
{
    if(file->data)
    {
        VirtualFree(file->data, 0, MEM_RELEASE);
    }
}

static void PLATFORM_FREE_MEMORY(void *memory)
{
    if(memory)
    {
        VirtualFree(memory, 0, MEM_RELEASE);
    }
}

static uint32_t PLATFORM_READ_FILE(const char *file_name, ods::File *result)
{
    HANDLE file_handle = CreateFile(file_name, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

    if(file_handle != INVALID_HANDLE_VALUE)
    {
        LARGE_INTEGER file_size;
        if(GetFileSizeEx(file_handle, &file_size))
        {
            // TODO: Defines for maximum values.
            uint32_t file_size_32 = file_size.LowPart;
            result->data = VirtualAlloc(0, file_size_32, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);

            if(result->data)
            {
                DWORD bytes_read;
                if(ReadFile(file_handle, result->data, file_size_32, &bytes_read, 0) &&
                  (file_size_32 == bytes_read))
                {
                    //NOTE: File read was successfull.
                    result->size = file_size_32;
                    CloseHandle(file_handle);
                    return 0;
                }
                else
                {
                    PLATFORM_FREE_FILE_MEMORY(result);
                    result->data = 0;
                    return 1;
                    //TODO: Logging.
                    //DWORD error =  GetLastError();
                }
            }
            else
            {
                // TODO: Logging
            }

        }
        else
        {
            // TODO: Logging.
        }
    }
    else
    {
        // TODO: Logging.
    }

    CloseHandle(file_handle);
    return 1;
}

static uint32_t PLATFORM_WRITE_FILE(const char* file_name, uint32_t memory_size, void *memory)
{
    HANDLE file_handle = CreateFile(file_name, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);

    if(file_handle != INVALID_HANDLE_VALUE)
    {
        DWORD bytes_written;

        //NOTE: File read successfully.
        if(WriteFile(file_handle, memory, memory_size, &bytes_written, 0))
        {
            uint32_t result = (bytes_written == memory_size);
            if(result != 0)
            {
                return 0;
            }
            else
            {
                return 1;
            }
        }
        else
        {
            // TODO: Logging.
            return 1;
        }

        CloseHandle(file_handle);
    }
    else
    {
        //TODO: Logging.
    }

    return 1;
}

#endif
