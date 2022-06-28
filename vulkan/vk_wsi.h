#ifndef ODYSSEY_VULKAN_VK_WSI_H
#define ODYSSEY_VULKAN_VK_WSI_H

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
