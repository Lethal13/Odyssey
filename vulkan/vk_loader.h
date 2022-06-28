#ifndef ODYSSEY_VULKAN_VK_LOADER_H
#define ODYSSEY_VULKAN_VK_LOADER_H

#ifndef VK_NO_PROTOTYPES
    #define VK_NO_PROTOTYPES
#endif

#include "vulkan/vulkan.h"
#if _WIN32 || _WIN64
    #include <windows.h>
    #include <vulkan/vulkan_win32.h>
#endif

#define VK_EXPORTED_FUNCTION( name ) PFN_##name name;
#define VK_GLOBAL_LEVEL_FUNCTION( name ) PFN_##name name;
#define VK_INSTANCE_LEVEL_FUNCTION( name ) PFN_##name name;
#define VK_DEVICE_LEVEL_FUNCTION( name ) PFN_##name name;

#define VK_INSTANCE_LEVEL_FUNCTION_LOAD( loader_instance, name )    \
    name = (PFN_##name)vkGetInstanceProcAddr(loader_instance, #name);

#define VK_DEVICE_LEVEL_FUNCTION_LOAD( loader_device, name  ) \
    name = (PFN_##name)vkGetDeviceProcAddr(loader_device, #name);

VK_EXPORTED_FUNCTION( vkGetInstanceProcAddr )
#undef VK_EXPORTED_FUNCTION

VK_GLOBAL_LEVEL_FUNCTION( vkEnumerateInstanceVersion )
VK_GLOBAL_LEVEL_FUNCTION( vkEnumerateInstanceExtensionProperties )
VK_GLOBAL_LEVEL_FUNCTION( vkEnumerateInstanceLayerProperties )
VK_GLOBAL_LEVEL_FUNCTION( vkCreateInstance )
#undef VK_GLOBAL_LEVEL_FUNCTION

VK_INSTANCE_LEVEL_FUNCTION( vkDestroyInstance )
VK_INSTANCE_LEVEL_FUNCTION( vkEnumeratePhysicalDevices )
VK_INSTANCE_LEVEL_FUNCTION( vkGetPhysicalDeviceProperties )
VK_INSTANCE_LEVEL_FUNCTION( vkGetPhysicalDeviceQueueFamilyProperties )

VK_INSTANCE_LEVEL_FUNCTION( vkCreateDebugUtilsMessengerEXT ) // VK_EXT_DEBUG_UTILS_EXTENSION_NAME
VK_INSTANCE_LEVEL_FUNCTION( vkDestroyDebugUtilsMessengerEXT ) // VK_EXT_DEBUG_UTILS_EXTENSION_NAME

VK_INSTANCE_LEVEL_FUNCTION( vkCreateWin32SurfaceKHR ) // VK_KHR_WIN32_SURFACE_EXTENSION_NAME
VK_INSTANCE_LEVEL_FUNCTION( vkDestroySurfaceKHR ) // VK_KHR_SURFACE_EXTENSION_NAME
VK_INSTANCE_LEVEL_FUNCTION( vkGetPhysicalDeviceSurfaceSupportKHR ) // VK_KHR_SURFACE_EXTENSION_NAME
#undef VK_INSTANCE_LEVEL_FUNCTION

#undef VK_DEVICE_LEVEL_FUNCTION

static VkResult LOAD_VULKAN(void)
{
#if (_WIN32 || _WIN64)
    HMODULE module = LoadLibraryA("vulkan-1.dll");
    if (!module)
        return VK_ERROR_INITIALIZATION_FAILED;

    vkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)(void(*)(void))GetProcAddress(module, "vkGetInstanceProcAddr");
#endif

    return VK_SUCCESS;
}

static void LOAD_VK_GLOBAL_FUNCTIONS(void)
{
    vkCreateInstance = (PFN_vkCreateInstance)vkGetInstanceProcAddr(0, "vkCreateInstance");
    vkEnumerateInstanceExtensionProperties = (PFN_vkEnumerateInstanceExtensionProperties)vkGetInstanceProcAddr(0, "vkEnumerateInstanceExtensionProperties");
    vkEnumerateInstanceLayerProperties = (PFN_vkEnumerateInstanceLayerProperties)vkGetInstanceProcAddr(0, "vkEnumerateInstanceLayerProperties");
    vkEnumerateInstanceVersion = (PFN_vkEnumerateInstanceVersion)vkGetInstanceProcAddr(0, "vkEnumerateInstanceVersion");
}

static void LOAD_VK_INSTANCE_FUNCTIONS(VkInstance instance)
{
    vkDestroyInstance = VK_INSTANCE_LEVEL_FUNCTION_LOAD( instance, vkDestroyInstance )
    vkEnumeratePhysicalDevices = VK_INSTANCE_LEVEL_FUNCTION_LOAD(instance, vkEnumeratePhysicalDevices )
    vkGetPhysicalDeviceProperties = VK_INSTANCE_LEVEL_FUNCTION_LOAD(instance, vkGetPhysicalDeviceProperties )
    vkGetPhysicalDeviceQueueFamilyProperties = VK_INSTANCE_LEVEL_FUNCTION_LOAD(instance, vkGetPhysicalDeviceQueueFamilyProperties )
    vkCreateDebugUtilsMessengerEXT = VK_INSTANCE_LEVEL_FUNCTION_LOAD(instance, vkCreateDebugUtilsMessengerEXT )
    vkDestroyDebugUtilsMessengerEXT = VK_INSTANCE_LEVEL_FUNCTION_LOAD(instance, vkDestroyDebugUtilsMessengerEXT )
    vkCreateWin32SurfaceKHR = VK_INSTANCE_LEVEL_FUNCTION_LOAD(instance, vkCreateWin32SurfaceKHR )
    vkDestroySurfaceKHR = VK_INSTANCE_LEVEL_FUNCTION_LOAD(instance, vkDestroySurfaceKHR )
    vkGetPhysicalDeviceSurfaceSupportKHR = VK_INSTANCE_LEVEL_FUNCTION_LOAD(instance, vkGetPhysicalDeviceSurfaceSupportKHR )
}

static void LOAD_VK_DEVICE_FUNCTIONS(VkDevice device)
{

}

#endif
