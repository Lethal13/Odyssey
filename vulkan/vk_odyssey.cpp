/*
* @Author: Giannis
* @Date:   2022-06-28 00:40:49
* @Last Modified by:   Giannis
* @Last Modified time: 2022-06-28 23:54:28
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
}

static void vk_deinit(pVkOdyssey odyssey)
{
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
