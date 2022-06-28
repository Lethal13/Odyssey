#ifndef ODYSSEY_VULKAN_VK_PHYSICAL_DEVICE_H
#define ODYSSEY_VULKAN_VK_PHYSICAL_DEVICE_H

static uint32_t vk_pick_physical_device(VkInstance instance, VkPhysicalDevice *gpu, VkSurfaceKHR surface)
{
    uint32_t physical_devices_counter;
    VK_ASSERT(vkEnumeratePhysicalDevices(instance, &physical_devices_counter, nullptr));

    ods::vector<VkPhysicalDevice> physical_devices(physical_devices_counter);
    VK_ASSERT(vkEnumeratePhysicalDevices(instance, &physical_devices_counter, physical_devices.data()));

    for(VkPhysicalDevice physical_device : physical_devices)
    {
        VkPhysicalDeviceProperties physical_device_properties;
        vkGetPhysicalDeviceProperties(physical_device, &physical_device_properties);

        // TODO: Check if extensions needed by the application are available by
        // the physical device.
        // vkEnumerateDeviceExtensionProperties

        // VkPhysicalDeviceType
        // 1. VK_PHYSICAL_DEVICE_TYPE_OTHER = 0 // does not match any other available type.
        // 2. VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU = 1 // embedded.
        // 3. VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU = 2 // seperate processor.
        // 4. VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU = 3 // virtual node in a virtualization environment.
        // 5. VK_PHYSICAL_DEVICE_TYPE_CPU = 4 // typically running on the same processor as the host.

        // For now return the first discrete gpu you will find.
        // TODO: Do it, in a more sophisticated way.
        if(physical_device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        {
            if(physical_device_surface_support(physical_device, surface) == VK_TRUE)
            {
                *gpu = physical_device;
                return 0;
            }
        }
    }

    // If we do not find a discrete gpu, return the first gpu in the vector.
    *gpu = physical_devices[0];
    return 1;
}

// Get a graphics queue index that supports presentation to a chosen surface.
static uint32_t get_graphics_queue_family(VkPhysicalDevice physical_device, VkSurfaceKHR surface,
    uint32_t *graphics_queue_index)
{
    uint32_t queue_families_count;
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_families_count, 0);

    ods::vector<VkQueueFamilyProperties> queue_family_properties(queue_families_count);

    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_families_count, queue_family_properties.data());

    uint32_t current_index = 0;

    VkBool32 is_supported = VK_FALSE;

    for(VkQueueFamilyProperties queue_family_property : queue_family_properties)
    {
        // VkQueueFlags
        // 1. VK_QUEUE_GRAPHICS_BIT
        // 2. VK_QUEUE_COMPUTE_BIT
        // 3. VK_QUEUE_TRANSFER_BIT
        // 4. VK_QUEUE_SPARSE_BINDING_BIT
        // 5. VK_QUEUE_PROTECTED_BIT // 1.1 version
        //
        // #ifdef VK_ENABLE_BETA_EXTENSIONS
        // 6. VK_QUEUE_VIDEO_DECODE_BIT_KHR // VK_KHR_video_decode_queue
        // 7. VK_QUEUE_VIDEO_ENCODE_BIT_KHR // VK_KHR_video_encode_queue
        // #endif

        if(queue_family_property.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {

            vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, current_index, surface, &is_supported);
            if(is_supported == VK_TRUE)
            {
                *graphics_queue_index = current_index;
                return 0;
            }
        }

        ++current_index;
    }

    return 1;
}

#endif
