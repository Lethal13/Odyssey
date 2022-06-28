#ifndef ODYSSEY_VULKAN_VK_DEBUG_UTILS_H
#define ODYSSEY_VULKAN_VK_DEBUG_UTILS_H

// TODO: Move elsewhere the debug definition.
#define VULKAN_ODYSSEY_DEBUG
#ifdef VULKAN_ODYSSEY_DEBUG

#include <stdio.h> //printf

#define VK_ASSERT(result)                                                               \
    if (result < 0)                                                                     \
    {                                                                                   \
        switch (result)                                                                 \
        {                                                                               \
        case VK_SUCCESS:                                                                \
            printf("Success\n");                                                        \
            break;                                                                      \
        case VK_NOT_READY:                                                              \
            printf("A fence or query has not yet completed\n");                         \
            break;                                                                      \
        case VK_TIMEOUT:                                                                \
            printf("A wait operation has not completed in the specified time\n");       \
            break;                                                                      \
        case VK_EVENT_SET:                                                              \
            printf("An event is signaled\n");                                           \
            break;                                                                      \
        case VK_EVENT_RESET:                                                            \
            printf("An event is unsignaled\n");                                         \
            break;                                                                      \
        case VK_INCOMPLETE:                                                             \
            printf("A return array was too small for the result\n");                    \
            break;                                                                      \
        case VK_ERROR_OUT_OF_HOST_MEMORY:                                               \
            printf("VK_ERROR_OUT_OF_HOST_MEMORY\n");                                    \
            break;                                                                      \
        case VK_ERROR_OUT_OF_DEVICE_MEMORY:                                             \
            printf("VK_ERROR_OUT_OF_DEVICE_MEMORY\n");                                  \
            break;                                                                      \
        case VK_ERROR_INITIALIZATION_FAILED:                                            \
            printf("VK_ERROR_INITIALIZATION_FAILED\n");                                 \
            break;                                                                      \
        case VK_ERROR_DEVICE_LOST:                                                      \
            printf("VK_ERROR_DEVICE_LOST\n");                                           \
            break;                                                                      \
        case VK_ERROR_MEMORY_MAP_FAILED:                                                \
            printf("VK_ERROR_MEMORY_MAP_FAILED\n");                                     \
            break;                                                                      \
        case VK_ERROR_LAYER_NOT_PRESENT:                                                \
            printf("VK_ERROR_LAYER_NOT_PRESENT\n");                                     \
            break;                                                                      \
        case VK_ERROR_EXTENSION_NOT_PRESENT:                                            \
            printf("VK_ERROR_EXTENSION_NOT_PRESENT\n");                                 \
            break;                                                                      \
        case VK_ERROR_FEATURE_NOT_PRESENT:                                              \
            printf("VK_ERROR_FEATURE_NOT_PRESENT\n");                                   \
            break;                                                                      \
        case VK_ERROR_INCOMPATIBLE_DRIVER:                                              \
            printf("VK_ERROR_INCOMPATIBLE_DRIVER\n");                                   \
            break;                                                                      \
        case VK_ERROR_TOO_MANY_OBJECTS:                                                 \
            printf("VK_ERROR_TOO_MANY_OBJECTS\n");                                      \
            break;                                                                      \
        case VK_ERROR_FORMAT_NOT_SUPPORTED:                                             \
            printf("VK_ERROR_FORMAT_NOT_SUPPORTED\n");                                  \
            break;                                                                      \
        case VK_ERROR_FRAGMENTED_POOL:                                                  \
            printf("VK_ERROR_FRAGMENTED_POOL\n");                                       \
            break;                                                                      \
        case VK_ERROR_UNKNOWN:                                                          \
            printf("VK_ERROR_UNKNOWN\n");                                               \
            break;                                                                      \
        case VK_ERROR_OUT_OF_POOL_MEMORY:                                               \
            printf("VK_ERROR_OUT_OF_POOL_MEMORY\n");                                    \
            break;                                                                      \
        case VK_ERROR_INVALID_EXTERNAL_HANDLE:                                          \
            printf("VK_ERROR_INVALID_EXTERNAL_HANDLE\n");                               \
            break;                                                                      \
        case VK_ERROR_FRAGMENTATION:                                                    \
            printf("VK_ERROR_FRAGMENTATION\n");                                         \
            break;                                                                      \
        case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS:                                   \
            printf("VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS\n");                        \
            break;                                                                      \
        case VK_ERROR_SURFACE_LOST_KHR:                                                 \
            printf("VK_ERROR_SURFACE_LOST_KHR\n");                                      \
            break;                                                                      \
        case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:                                         \
            printf("VK_ERROR_NATIVE_WINDOW_IN_USE_KHR\n");                              \
            break;                                                                      \
        case  VK_SUBOPTIMAL_KHR:                                                        \
            printf("VK_SUBOPTIMAL_KHR\n");                                              \
            break;                                                                      \
        case  VK_ERROR_OUT_OF_DATE_KHR:                                                 \
            printf("VK_ERROR_OUT_OF_DATE_KHR\n");                                       \
            break;                                                                      \
        case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:                                         \
            printf("VK_ERROR_INCOMPATIBLE_DISPLAY_KHR\n");                              \
            break;                                                                      \
        case VK_ERROR_VALIDATION_FAILED_EXT:                                            \
            printf("VK_ERROR_VALIDATION_FAILED_EXT\n");                                 \
            break;                                                                      \
        case VK_ERROR_INVALID_SHADER_NV:                                                \
            printf("VK_ERROR_INVALID_SHADER_NV\n");                                     \
            break;                                                                      \
        case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT:                     \
            printf("VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT\n");          \
            break;                                                                      \
        case VK_ERROR_NOT_PERMITTED_EXT:                                                \
            printf("VK_ERROR_NOT_PERMITTED_EXT\n");                                     \
            break;                                                                      \
        case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT:                              \
            printf("VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT\n");                   \
            break;                                                                      \
        default:                                                                        \
            printf("Unknown Vulkan Error.\n");                                          \
        }                                                                               \
    }                                                                                   \

#else


#define VK_ASSERT(result)

#endif /* VULKAN_DEBUG */


#endif /* Header */
