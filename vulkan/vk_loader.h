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

VK_INSTANCE_LEVEL_FUNCTION( vkGetDeviceProcAddr )
VK_INSTANCE_LEVEL_FUNCTION( vkDestroyInstance )
VK_INSTANCE_LEVEL_FUNCTION( vkEnumeratePhysicalDevices )
VK_INSTANCE_LEVEL_FUNCTION( vkGetPhysicalDeviceProperties )
VK_INSTANCE_LEVEL_FUNCTION( vkGetPhysicalDeviceQueueFamilyProperties )

VK_INSTANCE_LEVEL_FUNCTION( vkCreateDebugUtilsMessengerEXT ) // VK_EXT_DEBUG_UTILS_EXTENSION_NAME
VK_INSTANCE_LEVEL_FUNCTION( vkDestroyDebugUtilsMessengerEXT ) // VK_EXT_DEBUG_UTILS_EXTENSION_NAME

VK_INSTANCE_LEVEL_FUNCTION( vkCreateWin32SurfaceKHR ) // VK_KHR_WIN32_SURFACE_EXTENSION_NAME
VK_INSTANCE_LEVEL_FUNCTION( vkDestroySurfaceKHR ) // VK_KHR_SURFACE_EXTENSION_NAME
VK_INSTANCE_LEVEL_FUNCTION( vkGetPhysicalDeviceSurfaceSupportKHR ) // VK_KHR_SURFACE_EXTENSION_NAME
VK_INSTANCE_LEVEL_FUNCTION( vkGetPhysicalDeviceSurfaceCapabilitiesKHR ) // VK_KHR_SURFACE_EXTENSION_NAME
VK_INSTANCE_LEVEL_FUNCTION( vkGetPhysicalDeviceSurfaceFormatsKHR ) // VK_KHR_SURFACE_EXTENSION_NAME
VK_INSTANCE_LEVEL_FUNCTION( vkGetPhysicalDeviceSurfacePresentModesKHR ) // VK_KHR_SURFACE_EXTENSION_NAME
VK_INSTANCE_LEVEL_FUNCTION( vkCreateDevice )
#undef VK_INSTANCE_LEVEL_FUNCTION

VK_DEVICE_LEVEL_FUNCTION( vkGetDeviceQueue )
VK_DEVICE_LEVEL_FUNCTION( vkDestroyDevice )
VK_DEVICE_LEVEL_FUNCTION( vkDeviceWaitIdle )
VK_DEVICE_LEVEL_FUNCTION( vkCreateSwapchainKHR ) // VK_KHR_swapchain.
VK_DEVICE_LEVEL_FUNCTION( vkGetSwapchainImagesKHR ) // VK_KHR_swapchain.
VK_DEVICE_LEVEL_FUNCTION( vkDestroySwapchainKHR ) // VK_KHR_swapchain.
VK_DEVICE_LEVEL_FUNCTION( vkCreateImageView )
VK_DEVICE_LEVEL_FUNCTION( vkDestroyImageView )
VK_DEVICE_LEVEL_FUNCTION( vkCreateRenderPass )
VK_DEVICE_LEVEL_FUNCTION( vkDestroyRenderPass )
VK_DEVICE_LEVEL_FUNCTION( vkCreateFramebuffer )
VK_DEVICE_LEVEL_FUNCTION( vkDestroyFramebuffer )
VK_DEVICE_LEVEL_FUNCTION( vkCreateGraphicsPipelines )
VK_DEVICE_LEVEL_FUNCTION( vkDestroyPipeline )
VK_DEVICE_LEVEL_FUNCTION( vkCreateShaderModule )
VK_DEVICE_LEVEL_FUNCTION( vkDestroyShaderModule )
VK_DEVICE_LEVEL_FUNCTION( vkCreatePipelineLayout )
VK_DEVICE_LEVEL_FUNCTION( vkDestroyPipelineLayout )
VK_DEVICE_LEVEL_FUNCTION( vkCreateCommandPool )
VK_DEVICE_LEVEL_FUNCTION( vkDestroyCommandPool )
VK_DEVICE_LEVEL_FUNCTION( vkAllocateCommandBuffers )
VK_DEVICE_LEVEL_FUNCTION( vkResetCommandBuffer )
VK_DEVICE_LEVEL_FUNCTION( vkBeginCommandBuffer )
VK_DEVICE_LEVEL_FUNCTION( vkCmdBeginRenderPass )
VK_DEVICE_LEVEL_FUNCTION( vkCmdBindPipeline )
VK_DEVICE_LEVEL_FUNCTION( vkCmdDraw )
VK_DEVICE_LEVEL_FUNCTION( vkCmdEndRenderPass )
VK_DEVICE_LEVEL_FUNCTION( vkEndCommandBuffer )
VK_DEVICE_LEVEL_FUNCTION( vkCreateSemaphore )
VK_DEVICE_LEVEL_FUNCTION( vkDestroySemaphore )
VK_DEVICE_LEVEL_FUNCTION( vkCreateFence )
VK_DEVICE_LEVEL_FUNCTION( vkDestroyFence )
VK_DEVICE_LEVEL_FUNCTION( vkWaitForFences )
VK_DEVICE_LEVEL_FUNCTION( vkAcquireNextImageKHR ) // VK_KHR_swapchain
VK_DEVICE_LEVEL_FUNCTION( vkResetFences )
VK_DEVICE_LEVEL_FUNCTION( vkQueueSubmit )
VK_DEVICE_LEVEL_FUNCTION( vkQueuePresentKHR )
VK_DEVICE_LEVEL_FUNCTION( vkCmdSetViewport )
VK_DEVICE_LEVEL_FUNCTION( vkCmdSetScissor )
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
    vkGetDeviceProcAddr = VK_INSTANCE_LEVEL_FUNCTION_LOAD( instance, vkGetDeviceProcAddr )
    vkDestroyInstance = VK_INSTANCE_LEVEL_FUNCTION_LOAD( instance, vkDestroyInstance )
    vkEnumeratePhysicalDevices = VK_INSTANCE_LEVEL_FUNCTION_LOAD( instance, vkEnumeratePhysicalDevices )
    vkGetPhysicalDeviceProperties = VK_INSTANCE_LEVEL_FUNCTION_LOAD( instance, vkGetPhysicalDeviceProperties )
    vkGetPhysicalDeviceQueueFamilyProperties = VK_INSTANCE_LEVEL_FUNCTION_LOAD( instance, vkGetPhysicalDeviceQueueFamilyProperties )
    vkCreateDebugUtilsMessengerEXT = VK_INSTANCE_LEVEL_FUNCTION_LOAD( instance, vkCreateDebugUtilsMessengerEXT )
    vkDestroyDebugUtilsMessengerEXT = VK_INSTANCE_LEVEL_FUNCTION_LOAD( instance, vkDestroyDebugUtilsMessengerEXT )
    vkCreateWin32SurfaceKHR = VK_INSTANCE_LEVEL_FUNCTION_LOAD( instance, vkCreateWin32SurfaceKHR )
    vkDestroySurfaceKHR = VK_INSTANCE_LEVEL_FUNCTION_LOAD( instance, vkDestroySurfaceKHR )
    vkGetPhysicalDeviceSurfaceSupportKHR = VK_INSTANCE_LEVEL_FUNCTION_LOAD( instance, vkGetPhysicalDeviceSurfaceSupportKHR )
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR = VK_INSTANCE_LEVEL_FUNCTION_LOAD( instance, vkGetPhysicalDeviceSurfaceCapabilitiesKHR )
    vkGetPhysicalDeviceSurfaceFormatsKHR = VK_INSTANCE_LEVEL_FUNCTION_LOAD( instance, vkGetPhysicalDeviceSurfaceFormatsKHR )
    vkGetPhysicalDeviceSurfacePresentModesKHR = VK_INSTANCE_LEVEL_FUNCTION_LOAD( instance, vkGetPhysicalDeviceSurfacePresentModesKHR )
    vkCreateDevice = VK_INSTANCE_LEVEL_FUNCTION_LOAD( instance, vkCreateDevice )
}

static void LOAD_VK_DEVICE_FUNCTIONS(VkDevice device)
{
    vkGetDeviceQueue = VK_DEVICE_LEVEL_FUNCTION_LOAD( device, vkGetDeviceQueue )
    vkDestroyDevice = VK_DEVICE_LEVEL_FUNCTION_LOAD( device, vkDestroyDevice  )
    vkDeviceWaitIdle = VK_DEVICE_LEVEL_FUNCTION_LOAD( device, vkDeviceWaitIdle )
    vkCreateSwapchainKHR = VK_DEVICE_LEVEL_FUNCTION_LOAD( device, vkCreateSwapchainKHR )
    vkGetSwapchainImagesKHR = VK_DEVICE_LEVEL_FUNCTION_LOAD( device, vkGetSwapchainImagesKHR )
    vkDestroySwapchainKHR = VK_DEVICE_LEVEL_FUNCTION_LOAD( device, vkDestroySwapchainKHR )
    vkCreateImageView = VK_DEVICE_LEVEL_FUNCTION_LOAD( device, vkCreateImageView )
    vkDestroyImageView = VK_DEVICE_LEVEL_FUNCTION_LOAD( device, vkDestroyImageView )
    vkCreateRenderPass = VK_DEVICE_LEVEL_FUNCTION_LOAD( device, vkCreateRenderPass )
    vkCreateFramebuffer = VK_DEVICE_LEVEL_FUNCTION_LOAD( device, vkCreateFramebuffer )
    vkDestroyRenderPass = VK_DEVICE_LEVEL_FUNCTION_LOAD( device, vkDestroyRenderPass )
    vkDestroyFramebuffer = VK_DEVICE_LEVEL_FUNCTION_LOAD( device, vkDestroyFramebuffer )
    vkCreateGraphicsPipelines = VK_DEVICE_LEVEL_FUNCTION_LOAD( device, vkCreateGraphicsPipelines )
    vkDestroyPipeline = VK_DEVICE_LEVEL_FUNCTION_LOAD( device, vkDestroyPipeline )
    vkCreateShaderModule = VK_DEVICE_LEVEL_FUNCTION_LOAD( device, vkCreateShaderModule )
    vkDestroyShaderModule = VK_DEVICE_LEVEL_FUNCTION_LOAD( device, vkDestroyShaderModule )
    vkCreatePipelineLayout = VK_DEVICE_LEVEL_FUNCTION_LOAD( device, vkCreatePipelineLayout )
    vkDestroyPipelineLayout = VK_DEVICE_LEVEL_FUNCTION_LOAD( device, vkDestroyPipelineLayout )
    vkCreateCommandPool = VK_DEVICE_LEVEL_FUNCTION_LOAD( device, vkCreateCommandPool )
    vkDestroyCommandPool = VK_DEVICE_LEVEL_FUNCTION_LOAD( device, vkDestroyCommandPool )
    vkAllocateCommandBuffers = VK_DEVICE_LEVEL_FUNCTION_LOAD( device, vkAllocateCommandBuffers )
    vkResetCommandBuffer = VK_DEVICE_LEVEL_FUNCTION_LOAD( device, vkResetCommandBuffer )
    vkBeginCommandBuffer = VK_DEVICE_LEVEL_FUNCTION_LOAD( device, vkBeginCommandBuffer )
    vkCmdBeginRenderPass = VK_DEVICE_LEVEL_FUNCTION_LOAD( device, vkCmdBeginRenderPass )
    vkCmdBindPipeline = VK_DEVICE_LEVEL_FUNCTION_LOAD( device, vkCmdBindPipeline )
    vkCmdDraw = VK_DEVICE_LEVEL_FUNCTION_LOAD( device, vkCmdDraw )
    vkCmdEndRenderPass = VK_DEVICE_LEVEL_FUNCTION_LOAD( device, vkCmdEndRenderPass )
    vkEndCommandBuffer = VK_DEVICE_LEVEL_FUNCTION_LOAD( device, vkEndCommandBuffer )
    vkCreateSemaphore = VK_DEVICE_LEVEL_FUNCTION_LOAD( device, vkCreateSemaphore )
    vkDestroySemaphore = VK_DEVICE_LEVEL_FUNCTION_LOAD( device, vkDestroySemaphore )
    vkCreateFence = VK_DEVICE_LEVEL_FUNCTION_LOAD( device, vkCreateFence )
    vkDestroyFence = VK_DEVICE_LEVEL_FUNCTION_LOAD( device, vkDestroyFence )
    vkWaitForFences = VK_DEVICE_LEVEL_FUNCTION_LOAD( device, vkWaitForFences )
    vkAcquireNextImageKHR = VK_DEVICE_LEVEL_FUNCTION_LOAD( device, vkAcquireNextImageKHR )
    vkResetFences = VK_DEVICE_LEVEL_FUNCTION_LOAD( device, vkResetFences )
    vkQueueSubmit = VK_DEVICE_LEVEL_FUNCTION_LOAD( device, vkQueueSubmit )
    vkQueuePresentKHR = VK_DEVICE_LEVEL_FUNCTION_LOAD( device, vkQueuePresentKHR )
    vkCmdSetViewport = VK_DEVICE_LEVEL_FUNCTION_LOAD( device, vkCmdSetViewport )
    vkCmdSetScissor = VK_DEVICE_LEVEL_FUNCTION_LOAD( device, vkCmdSetScissor )
}

#endif
