#ifndef ODYSSEY_VULKAN_VK_ODYSSEY_H
#define ODYSSEY_VULKAN_VK_ODYSSEY_H

#include "../containers/vector.h"
#include "../filesystem/file.h"

#include "../math/vec2.h"
#include "../math/vec3.h"

#include "vk_loader.h"
#include "vk_debug_utils.h"
#include "vk_wsi.h"
#include "vk_physical_device.h"
#include "vk_render_pass.h"
#include "vk_pipeline.h"

// 1. TODO: Decouple Debug/Release Mode.
// 2. Check if the requested extensions/layers are being supported.

/*
// Instance Layers List
// 1. VK_LAYER_KHRONOS_validation: Enables validation layers (debuggin tools).
// 2. VK_LAYER_LUNARG_monitor: Shows fps counter.
*/
constexpr const char *instance_layers_list[] = {
    "VK_LAYER_KHRONOS_validation",
    "VK_LAYER_LUNARG_monitor"
};

/*
// Instance Extensions List
// 1. VK_KHR_SURFACE_EXTENSION_NAME:
// 2. VK_EXT_DEBUG_UTILS_EXTENSIONS_NAME
// 3. VK_KHR_WIN32_SURFACE_EXTENSION_NAME: Windows OS specific.
*/
constexpr const char *instance_extensions_list[] = {
    VK_KHR_SURFACE_EXTENSION_NAME,
#if _WIN32 || _WIN64
    VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#endif
    VK_EXT_DEBUG_UTILS_EXTENSION_NAME
};

/*
// Device Layers List:
//
// 1. VK_LAYER_KHRONOS_validation: For legacy reasons, only necessarly to instance layers list.
*/
constexpr const char *device_layers_list[] = {
    "VK_LAYER_KHRONOS_validation"
};

/*
// Device Extensions List:
// 1. VK_KHR_SWAPCHAIN_EXTENSION_NAME: Extension for the usage of the present queue.
*/
constexpr const char *device_extensions_list[] = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

typedef struct VkOdyssey
{
    VkInstance instance; // The interface between the application and the implementation.
    VkDebugUtilsMessengerEXT debug_messenger; // Handles debug messages (validation layers).
    VkPhysicalDevice physical_device; // Represents a single complete implementation of Vulkan available to host.
    VkSurfaceKHR surface;
    VkDevice device; // Represents an instance of that implementation (VkPhysicalDevice) with its own
    // state and resources independent of the other logical devices. It's the primary interface to the
    // physical device.

    // Graphics queue that must support presentation to a given surface.
    VkQueue graphics_queue;

    // graphics queue index that supports presentation to a chosen surface.
    uint32_t graphics_queue_index;

    VkWSI wsi[2];

    VkRenderPass render_pass; // Each render pass instance defines a set of image resourses.
    // Defines the layout of the framebuffer and the transitions?
    ods::vector<VkFramebuffer> swapchain_framebuffers;

    GraphicsPipeline graphics_pipeline;

    VkCommandPool command_pool;
    ods::vector<VkCommandBuffer> command_buffers;

    ods::vector<VkSemaphore> image_available_semaphores;
    ods::vector<VkSemaphore> render_finished_semaphores;
    ods::vector<VkFence> in_flight_fences;

    uint32_t current_wsi;
    uint32_t current_frame;
    uint32_t total_frames;
} VkOdyssey, *pVkOdyssey;

static VkBool32 VKAPI_PTR
vkDebugUtilsMessengerCallbackEXT(VkDebugUtilsMessageSeverityFlagBitsEXT,
    VkDebugUtilsMessageTypeFlagsEXT, const VkDebugUtilsMessengerCallbackDataEXT*, void*);
static void vk_init(pVkOdyssey, HINSTANCE hInstance, HWND hwnd);
static void vk_deinit(pVkOdyssey);

static void vk_create_instance(pVkOdyssey);
static void vk_create_device(pVkOdyssey);
static void vk_create_command_pool(VkDevice, VkCommandPool*, uint32_t);
static void vk_create_command_buffers(VkDevice, VkCommandPool, ods::vector<VkCommandBuffer>&, uint32_t);
static void vk_command_buffers_recording(VkCommandBuffer, VkRenderPass, VkFramebuffer,
    VkExtent2D, VkPipeline);
// https://github.com/KhronosGroup/Vulkan-Docs/wiki/Synchronization-Examples#swapchain-image-acquire-and-present
static void vk_create_sync_objects(pVkOdyssey);
static void vk_destroy_sync_objects(pVkOdyssey);
static void vk_create_framebuffers(VkDevice, VkRenderPass, ods::vector<VkImageView>,
    ods::vector<VkFramebuffer>&, VkExtent2D);
static void vk_reinit_wsi(pVkOdyssey);
static void vk_draw_frame();
#endif
