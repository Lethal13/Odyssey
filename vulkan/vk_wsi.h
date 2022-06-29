#ifndef ODYSSEY_VULKAN_VK_WSI_H
#define ODYSSEY_VULKAN_VK_WSI_H

typedef struct VkWSI
{
    VkSwapchainKHR swapchain; // Swapchain in an abstraction for an array of presentable images that
    // are associated with a surface.

    ods::vector<VkImage> swapchain_images;
    ods::vector<VkImageView> swapchain_image_views;
    VkFormat swapchain_image_format;
    VkExtent2D swapchain_image_extent;
} VkWSI, *pVkWSI;

typedef struct SurfaceDetails
{
    VkSurfaceCapabilitiesKHR surface_capabilities;
    ods::vector<VkSurfaceFormatKHR> surface_formats;
    ods::vector<VkPresentModeKHR> present_modes;
} SurfaceDetails, *pSurfaceDetails;

static SurfaceDetails vk_get_surface_details(VkPhysicalDevice physical_device, VkSurfaceKHR surface)
{
    SurfaceDetails surface_details;

    VK_ASSERT(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, surface,
        &surface_details.surface_capabilities));

    uint32_t available_formats_counter;
    VK_ASSERT(vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &available_formats_counter, 0));

    ODS_ASSERT(available_formats_counter != 0);

    surface_details.surface_formats.resize(available_formats_counter);
    VK_ASSERT(vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &available_formats_counter,
        surface_details.surface_formats.data()));

    uint32_t available_present_modes_counter;

    VK_ASSERT(vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface,
        &available_present_modes_counter, 0));

    ODS_ASSERT(available_present_modes_counter != 0);

    surface_details.present_modes.resize(available_present_modes_counter);

    VK_ASSERT(vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface,
        &available_present_modes_counter, surface_details.present_modes.data()));

    return surface_details;
}

static VkSurfaceFormatKHR vk_choose_swapchain_format(ods::vector<VkSurfaceFormatKHR>& available_formats)
{
    for(VkSurfaceFormatKHR available_format : available_formats)
    {
        if(available_format.format == VK_FORMAT_B8G8R8A8_SRGB &&
            available_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            return available_format;
        }
    }

    return available_formats[0];
}

// VK_PRESENT_MODE_FIFO_KHR: VSync bound, Best for mobile (battery life), Triple buffering, always supported
// VK_PRESENT_MODE_MAILBOX_KHR: Keep submitting, low latency, Not optimal for mobile (bad battery life), not always supported
// VK_PRESENT_MODE_IMMEDIATE_KHR: Low latency, high power consumption, tearing
static VkPresentModeKHR vk_choose_swapchain_present_mode(const ods::vector<VkPresentModeKHR>& available_present_modes)
{
    for (VkPresentModeKHR& available_present_mode : available_present_modes)
    {
        if (available_present_mode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            return available_present_mode;
        }
    }

    return available_present_modes[0];
}

static VkExtent2D vk_choose_swapchain_extent(const VkSurfaceCapabilitiesKHR& surface_capabilities)
{
    if(surface_capabilities.currentExtent.width != UINT32_MAX)
    {
        return surface_capabilities.currentExtent;
    }
    else
    {
        //TODO: Initialize them better?.
        int width = 0, height = 0;

        VkExtent2D current_extent = { (uint32_t)width, (uint32_t)height};

        //NOTE: Max,Min functions defined in odyssey.h
        current_extent.width = Max(surface_capabilities.minImageExtent.width,
                               Min(surface_capabilities.maxImageExtent.width, current_extent.width));

        current_extent.height = Max(surface_capabilities.minImageExtent.height,
                                Min(surface_capabilities.maxImageExtent.height, current_extent.height));

        return current_extent;
    }
}

static void vk_create_surface(VkInstance instance, VkSurfaceKHR *surface, HINSTANCE hInstance, HWND hwnd)
{
    VkWin32SurfaceCreateInfoKHR surface_create_info = {};
    surface_create_info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    surface_create_info.pNext = 0;
    surface_create_info.flags = 0;
    surface_create_info.hinstance = hInstance;
    surface_create_info.hwnd = hwnd;

    VK_ASSERT(vkCreateWin32SurfaceKHR(instance, &surface_create_info, 0, surface));

    ODS_ASSERT(*surface);
}

static void vk_create_swapchain(VkSurfaceKHR surface, VkPhysicalDevice physical_device, VkDevice device,
    pVkWSI wsi, VkSwapchainKHR old_swapchain)
{
    SurfaceDetails surface_details = vk_get_surface_details(physical_device, surface);

    VkSurfaceFormatKHR surface_format = vk_choose_swapchain_format(surface_details.surface_formats);
    VkPresentModeKHR present_mode = vk_choose_swapchain_present_mode(surface_details.present_modes);
    VkExtent2D extent = vk_choose_swapchain_extent(surface_details.surface_capabilities);

    uint32_t image_count = surface_details.surface_capabilities.minImageCount + 1;

    if(surface_details.surface_capabilities.minImageCount > 0 &&
        image_count >surface_details.surface_capabilities.maxImageCount)
    {
        image_count = surface_details.surface_capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR swapchain_create_info = {};
    swapchain_create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchain_create_info.pNext = 0;
    swapchain_create_info.flags = 0;
    swapchain_create_info.surface = surface;
    swapchain_create_info.minImageCount = image_count;
    swapchain_create_info.imageFormat = surface_format.format;
    swapchain_create_info.imageColorSpace = surface_format.colorSpace;
    swapchain_create_info.imageExtent = extent;
    swapchain_create_info.imageArrayLayers = 1;
    swapchain_create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    swapchain_create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    swapchain_create_info.queueFamilyIndexCount = 0;
    swapchain_create_info.pQueueFamilyIndices = 0;
    // VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR
    swapchain_create_info.preTransform = surface_details.surface_capabilities.currentTransform;
    swapchain_create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapchain_create_info.presentMode = present_mode;
    // NOTE: clipped
    // VK_TRUE = better performance
    // VK_FALSE = worse performance but we can read from the clipped
    // pixels in case we use them in shaders for example.
    swapchain_create_info.clipped = VK_FALSE;
    swapchain_create_info.oldSwapchain = old_swapchain;

    VK_ASSERT(vkCreateSwapchainKHR(device, &swapchain_create_info, 0, &wsi->swapchain));

    vkGetSwapchainImagesKHR(device, wsi->swapchain, &image_count, 0);
    wsi->swapchain_images.resize(image_count);
    vkGetSwapchainImagesKHR(device, wsi->swapchain, &image_count, wsi->swapchain_images.data());

    wsi->swapchain_image_format = surface_format.format;
    wsi->swapchain_image_extent = extent;

}

static void vk_create_swapchain_image_views(VkDevice device, pVkWSI wsi)
{
    wsi->swapchain_image_views.resize(wsi->swapchain_images.size());

    for(size_t i = 0; i < wsi->swapchain_image_views.size(); ++i)
    {
        VkImageViewCreateInfo image_view_create_info = {};
        image_view_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        image_view_create_info.pNext = 0;
        image_view_create_info.flags = 0;
        image_view_create_info.image = wsi->swapchain_images[i];
        image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        image_view_create_info.format = wsi->swapchain_image_format;
        image_view_create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        image_view_create_info.subresourceRange.baseMipLevel = 0;
        image_view_create_info.subresourceRange.levelCount = 1;
        image_view_create_info.subresourceRange.baseArrayLayer = 0;
        image_view_create_info.subresourceRange.layerCount = 1;

        VK_ASSERT(vkCreateImageView(device, &image_view_create_info, 0, &wsi->swapchain_image_views[i]));
    }
}

static void vk_init_wsi(VkSurfaceKHR surface, VkPhysicalDevice physical_device, VkDevice device,
    pVkWSI wsi, VkSwapchainKHR old_swapchain)
{
    vk_create_swapchain(surface, physical_device, device, wsi, old_swapchain);
    vk_create_swapchain_image_views(device, wsi);
}

static void vk_deinit_wsi(VkDevice device, pVkWSI wsi)
{
    for(VkImageView image_view : wsi->swapchain_image_views)
    {
        vkDestroyImageView(device, image_view, 0);
    }
    vkDestroySwapchainKHR(device, wsi->swapchain, 0);
}

static uint32_t physical_device_surface_support(VkPhysicalDevice physical_device, VkSurfaceKHR surface)
{
    uint32_t queue_families_count;

    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_families_count, 0);

    ods::vector<VkQueueFamilyProperties> queue_family_properties(queue_families_count);

    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_families_count,
        queue_family_properties.data());

    uint32_t current_index = 0;
    VkBool32 is_supported = VK_FALSE;

    for(VkQueueFamilyProperties queue_family_property : queue_family_properties)
    {
        if(queue_family_property.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, current_index, surface, &is_supported);

            if(is_supported == VK_TRUE)
            {
                return is_supported;
            }
        }

        ++current_index;
    }

    return is_supported;
}



#endif
