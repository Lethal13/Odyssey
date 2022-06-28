/*
* @Author: Giannis
* @Date:   2022-06-28 00:40:49
* @Last Modified by:   Giannis
* @Last Modified time: 2022-06-29 01:00:27
*/

#include "vk_odyssey.h"

// PFN_vkDebugUtilsMessengerCallbackEXT VulkanDebugCallBack

static VkBool32 VKAPI_PTR
vkDebugUtilsMessengerCallbackEXT(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageTypes,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData)
{
    if(messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
    {
        ODS_ASSERT(1);
    }
    else
    {
        printf("%s\n",pCallbackData->pMessage);
    }
    return VK_FALSE;
}

static void vk_init(pVkOdyssey odyssey, HINSTANCE hInstance, HWND hwnd)
{
    vk_create_instance(odyssey);
    vk_create_surface(odyssey->instance, &odyssey->surface, hInstance, hwnd);
    if(vk_pick_physical_device(odyssey->instance, &odyssey->physical_device, odyssey->surface) != 0)
    {
        ODS_ASSERT(0);
    }

    vk_create_device(odyssey);
}

static void vk_deinit(pVkOdyssey odyssey)
{
    vkDeviceWaitIdle(odyssey->device);

    vkDestroyDevice(odyssey->device, 0);
    vkDestroySurfaceKHR(odyssey->instance, odyssey->surface, 0);
    vkDestroyDebugUtilsMessengerEXT(odyssey->instance, odyssey->debug_messenger, VK_NULL_HANDLE);
    vkDestroyInstance(odyssey->instance, VK_NULL_HANDLE);
}

static void vk_create_instance(pVkOdyssey odyssey)
{
    VkApplicationInfo application_info = {};
    application_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    application_info.pNext = 0;
    application_info.pApplicationName = "Odyssey";
    application_info.applicationVersion = VK_MAKE_VERSION(1, 3, 0);
    application_info.pEngineName = "Odyssey";
    application_info.engineVersion = VK_MAKE_VERSION(1, 3, 0);
    application_info.apiVersion = VK_API_VERSION_1_3;

    VkDebugUtilsMessengerCreateInfoEXT debug_utils_messenger_create_info = {};
    debug_utils_messenger_create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debug_utils_messenger_create_info.pNext = 0;
    debug_utils_messenger_create_info.flags = 0;
    debug_utils_messenger_create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    debug_utils_messenger_create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    debug_utils_messenger_create_info.pfnUserCallback = vkDebugUtilsMessengerCallbackEXT;
    debug_utils_messenger_create_info.pUserData = 0;

    VkInstanceCreateInfo instance_create_info = {};
    instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instance_create_info.pNext = &debug_utils_messenger_create_info;
    instance_create_info.flags = 0;
    instance_create_info.pApplicationInfo = &application_info;
    instance_create_info.enabledLayerCount = (uint64_t)ArraySize(instance_layers_list);
    instance_create_info.ppEnabledLayerNames = &instance_layers_list[0];
    instance_create_info.enabledExtensionCount = (uint64_t)ArraySize(instance_extensions_list);
    instance_create_info.ppEnabledExtensionNames = &instance_extensions_list[0];

    VK_ASSERT(vkCreateInstance(&instance_create_info, VK_NULL_HANDLE, &odyssey->instance));

    LOAD_VK_INSTANCE_FUNCTIONS(odyssey->instance);

    VK_ASSERT(vkCreateDebugUtilsMessengerEXT(odyssey->instance, &debug_utils_messenger_create_info,
        VK_NULL_HANDLE, &odyssey->debug_messenger));
}


static void vk_create_device(pVkOdyssey odyssey)
{
    uint32_t result = get_graphics_queue_family(odyssey->physical_device, odyssey->surface,
        &odyssey->graphics_queue_index);

    if(result == 1)
    {
        ODS_ASSERT(0);
    }

    float queue_priority = 1.0f;

    VkDeviceQueueCreateInfo device_graphics_queue_create_info = {};
    device_graphics_queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    device_graphics_queue_create_info.pNext = 0;
    device_graphics_queue_create_info.flags = 0;
    device_graphics_queue_create_info.queueFamilyIndex = odyssey->graphics_queue_index;
    device_graphics_queue_create_info.queueCount = 1;
    device_graphics_queue_create_info.pQueuePriorities = &queue_priority;

    VkDeviceQueueCreateInfo device_queue_create_infos[] = {
        device_graphics_queue_create_info
    };

    VkPhysicalDeviceFeatures physical_device_features = {};

    VkDeviceCreateInfo device_create_info = {};
    device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device_create_info.pNext = 0;
    device_create_info.flags = 0;
    device_create_info.queueCreateInfoCount = ArraySize(device_queue_create_infos);
    device_create_info.pQueueCreateInfos = &device_queue_create_infos[0];
    // Device layers are deprecated.
    device_create_info.enabledLayerCount = ArraySize(device_layers_list);
    device_create_info.ppEnabledLayerNames = &device_layers_list[0];
    device_create_info.enabledExtensionCount = ArraySize(device_extensions_list);
    device_create_info.ppEnabledExtensionNames = &device_extensions_list[0];
    device_create_info.pEnabledFeatures = &physical_device_features;

    VK_ASSERT(vkCreateDevice(odyssey->physical_device, &device_create_info, VK_NULL_HANDLE, &odyssey->device));

    LOAD_VK_DEVICE_FUNCTIONS(odyssey->device);

    vkGetDeviceQueue(odyssey->device, odyssey->graphics_queue_index, 0, &odyssey->graphics_queue);
}
