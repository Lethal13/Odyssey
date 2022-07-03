/*
* @Author: Giannis
* @Date:   2022-06-28 00:40:49
* @Last Modified by:   Giannis
* @Last Modified time: 2022-07-03 05:31:06
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


// TODO: Add asserts to every object that is being created.
static void vk_init(pVkOdyssey odyssey, HINSTANCE hInstance, HWND hwnd)
{
    vk_create_instance(odyssey);
    vk_create_surface(odyssey->instance, &odyssey->surface, hInstance, hwnd);
    if(vk_pick_physical_device(odyssey->instance, &odyssey->physical_device, odyssey->surface) != 0)
    {
        ODS_ASSERT(0);
    }

    vk_create_device(odyssey);

    vk_init_wsi(odyssey->surface, odyssey->physical_device, odyssey->device,
        &odyssey->wsi[0], VK_NULL_HANDLE);

    odyssey->current_wsi = 0;
    odyssey->current_frame = 0;
    odyssey->total_frames = (uint32_t)odyssey->wsi[0].swapchain_images.size();

    vk_create_render_pass(odyssey->device, &odyssey->render_pass,
        odyssey->wsi[odyssey->current_wsi].swapchain_image_format);
    vk_create_framebuffers(odyssey->device, odyssey->render_pass,
        odyssey->wsi[odyssey->current_wsi].swapchain_image_views,
        odyssey->swapchain_framebuffers, odyssey->wsi[odyssey->current_wsi].swapchain_image_extent);

    VkShaderModule vertex_shader_module;
    VkShaderModule fragment_shader_module;
    vk_create_shader_module(odyssey->device, &vertex_shader_module, "data/shaders/vert.spv");
    vk_create_shader_module(odyssey->device, &fragment_shader_module, "data/shaders/frag.spv");

    vk_create_graphics_pipeline(odyssey->device, odyssey->render_pass, &odyssey->graphics_pipeline,
        VK_NULL_HANDLE, vertex_shader_module, fragment_shader_module,
        odyssey->wsi[odyssey->current_wsi].swapchain_image_extent);


    vk_create_command_pool(odyssey->device, &odyssey->command_pool, odyssey->graphics_queue_index);

    vk_create_command_buffers(odyssey->device, odyssey->command_pool, odyssey->command_buffers,
        odyssey->total_frames);

    vk_create_sync_objects(odyssey);
}

static void vk_deinit(pVkOdyssey odyssey)
{
    vkDeviceWaitIdle(odyssey->device);

    vk_destroy_sync_objects(odyssey);

    vkDestroyCommandPool(odyssey->device, odyssey->command_pool, 0);

    vk_destroy_graphics_pipeline(odyssey->device, &odyssey->graphics_pipeline);

    for(VkFramebuffer vk_framebuffer : odyssey->swapchain_framebuffers)
    {
        vkDestroyFramebuffer(odyssey->device, vk_framebuffer, 0);
    }
    vkDestroyRenderPass(odyssey->device, odyssey->render_pass, 0);

    vk_deinit_wsi(odyssey->device, &odyssey->wsi[0]);

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

static void vk_create_command_pool(VkDevice device, VkCommandPool *command_pool, uint32_t queue_family_index)
{
    VkCommandPoolCreateInfo command_pool_create_info = {};
    command_pool_create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    command_pool_create_info.pNext = 0;
    command_pool_create_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    command_pool_create_info.queueFamilyIndex = queue_family_index;

    VK_ASSERT(vkCreateCommandPool(device, &command_pool_create_info, 0, command_pool));
}

static void vk_create_command_buffers(VkDevice device, VkCommandPool command_pool,
    ods::vector<VkCommandBuffer>& command_buffers, uint32_t command_buffers_size)
{
    command_buffers.resize(command_buffers_size);

    VkCommandBufferAllocateInfo command_buffer_allocate_info = {};
    command_buffer_allocate_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    command_buffer_allocate_info.pNext = 0;
    command_buffer_allocate_info.commandPool = command_pool;
    command_buffer_allocate_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    command_buffer_allocate_info.commandBufferCount = command_buffers_size;

    VK_ASSERT(vkAllocateCommandBuffers(device, &command_buffer_allocate_info, command_buffers.data()));
}

static void vk_command_buffers_recording(VkCommandBuffer command_buffer, VkRenderPass render_pass,
    VkFramebuffer framebuffer, VkExtent2D extent, VkPipeline graphics_pipeline)
{
    vkResetCommandBuffer(command_buffer, 0);

    VkCommandBufferBeginInfo command_buffer_begin_info = {};
    command_buffer_begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    command_buffer_begin_info.pNext = 0;
    command_buffer_begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    command_buffer_begin_info.pInheritanceInfo = 0;

    VK_ASSERT(vkBeginCommandBuffer(command_buffer, &command_buffer_begin_info));

    VkRenderPassBeginInfo render_pass_begin_info = {};
    render_pass_begin_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    render_pass_begin_info.renderPass = render_pass;
    render_pass_begin_info.framebuffer = framebuffer;
    render_pass_begin_info.renderArea.offset = {0, 0};
    render_pass_begin_info.renderArea.extent = extent;

    VkClearValue clear_value = {{{0.0f, 0.0f, 0.0f, 1.0f}}};

    render_pass_begin_info.clearValueCount = 1;
    render_pass_begin_info.pClearValues = &clear_value;

    VkViewport viewport = {};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)extent.width;
    viewport.height = (float)extent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor = {};
    scissor.offset = {0, 0};
    scissor.extent = extent;

    // VkPipelineViewportStateCreateInfo will be ignored based on spec, because
    // we have enable dynamic_state_viewport and dynamic_state_scissor.
    // The numberr of viewports/scissor rectangles used by pipeline are still
    // specified by the viewportCount and scissorCount.
    vkCmdSetViewport(command_buffer, 0, 1, &viewport);
    vkCmdSetScissor(command_buffer, 0, 1, &scissor);

    vkCmdBeginRenderPass(command_buffer, &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphics_pipeline);

    vkCmdDraw(command_buffer, 3, 1, 0, 0);
    vkCmdEndRenderPass(command_buffer);
    VK_ASSERT(vkEndCommandBuffer(command_buffer));
}

static void vk_create_sync_objects(pVkOdyssey odyssey)
{
    VkSemaphoreCreateInfo semaphore_create_info = {};
    semaphore_create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fence_create_info = {};
    fence_create_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fence_create_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    odyssey->image_available_semaphores.resize(odyssey->total_frames);
    odyssey->render_finished_semaphores.resize(odyssey->total_frames);
    odyssey->in_flight_fences.resize(odyssey->total_frames);

    for(size_t i = 0; i < odyssey->total_frames; ++i)
    {
        VK_ASSERT(vkCreateSemaphore(odyssey->device, &semaphore_create_info, 0, &odyssey->image_available_semaphores[i]));
        VK_ASSERT(vkCreateSemaphore(odyssey->device, &semaphore_create_info, 0, &odyssey->render_finished_semaphores[i]));
        VK_ASSERT(vkCreateFence(odyssey->device, &fence_create_info, 0, &odyssey->in_flight_fences[i]));
    }
}

static void vk_destroy_sync_objects(pVkOdyssey odyssey)
{
    for(size_t i = 0; i < odyssey->total_frames; ++i)
    {
        vkDestroySemaphore(odyssey->device, odyssey->image_available_semaphores[i], 0);
        vkDestroySemaphore(odyssey->device, odyssey->render_finished_semaphores[i], 0);
        vkDestroyFence(odyssey->device, odyssey->in_flight_fences[i], 0);
    }
}

static void vk_create_framebuffers(VkDevice device, VkRenderPass render_pass, ods::vector<VkImageView> images_views,
    ods::vector<VkFramebuffer>& framebuffers, VkExtent2D extent)
{
    // TODO: Maybe create/use other function instead of resize, because
    // the vector might have already framebuffer and we want to wipe them out.
    // Resize doesnt wipe out previous objects in vector.
    framebuffers.resize(images_views.size());

    for(size_t i = 0; i < framebuffers.size(); ++i)
    {
        VkImageView attachments[] = {
            images_views[i]
        };

        VkFramebufferCreateInfo framebuffer_create_info = {};
        framebuffer_create_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebuffer_create_info.pNext = 0;
        framebuffer_create_info.flags = 0;
        framebuffer_create_info.renderPass = render_pass;
        framebuffer_create_info.attachmentCount = ArraySize(attachments);
        framebuffer_create_info.pAttachments = &attachments[0];
        framebuffer_create_info.width = extent.width;
        framebuffer_create_info.height = extent.height;
        framebuffer_create_info.layers = 1;

        VK_ASSERT(vkCreateFramebuffer(device, &framebuffer_create_info, nullptr, &framebuffers[i]));
    }

}

static void vk_reinit_wsi(pVkOdyssey odyssey)
{
    vkDeviceWaitIdle(odyssey->device);

    uint32_t current_index = odyssey->current_wsi;

    if(current_index == 0)
    {
        vk_init_wsi(odyssey->surface, odyssey->physical_device, odyssey->device,
            &odyssey->wsi[current_index + 1], odyssey->wsi[current_index].swapchain);

        vk_deinit_wsi(odyssey->device, &odyssey->wsi[current_index]);
        for(VkFramebuffer vk_framebuffer : odyssey->swapchain_framebuffers)
        {
            vkDestroyFramebuffer(odyssey->device, vk_framebuffer, 0);
        }
        vkDestroyRenderPass(odyssey->device, odyssey->render_pass, 0);

        vk_create_render_pass(odyssey->device, &odyssey->render_pass, odyssey->wsi[current_index + 1].swapchain_image_format);
        vk_create_framebuffers(odyssey->device, odyssey->render_pass, odyssey->wsi[current_index + 1].swapchain_image_views,
            odyssey->swapchain_framebuffers, odyssey->wsi[current_index + 1].swapchain_image_extent);

    }
    else
    {
        vk_init_wsi(odyssey->surface, odyssey->physical_device, odyssey->device,
            &odyssey->wsi[current_index - 1], odyssey->wsi[current_index].swapchain);

        vk_deinit_wsi(odyssey->device, &odyssey->wsi[current_index]);
        for(VkFramebuffer vk_framebuffer : odyssey->swapchain_framebuffers)
        {
            vkDestroyFramebuffer(odyssey->device, vk_framebuffer, 0);
        }
        vkDestroyRenderPass(odyssey->device, odyssey->render_pass, 0);

        vk_create_render_pass(odyssey->device, &odyssey->render_pass, odyssey->wsi[current_index - 1].swapchain_image_format);
        vk_create_framebuffers(odyssey->device, odyssey->render_pass, odyssey->wsi[current_index - 1].swapchain_image_views,
            odyssey->swapchain_framebuffers, odyssey->wsi[current_index - 1].swapchain_image_extent);

    }

    odyssey->current_wsi = (odyssey->current_wsi + 1) % ArraySize(odyssey->wsi);
}

static void vk_draw_frame(pVkOdyssey odyssey)
{
    vkWaitForFences(odyssey->device, 1, &odyssey->in_flight_fences[odyssey->current_frame], VK_TRUE, UINT64_MAX);

    uint32_t image_index;

    VkResult result = vkAcquireNextImageKHR(odyssey->device, odyssey->wsi[odyssey->current_wsi].swapchain, UINT64_MAX,
        odyssey->image_available_semaphores[odyssey->current_frame], VK_NULL_HANDLE, &image_index);

    if(result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        vk_reinit_wsi(odyssey);
        // TODO + NOTE: Check if we should increment odyssey->current_frame variable.
        return;
    }
    else if( result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    {
        ODS_ASSERT(1);
    }


    vk_command_buffers_recording(odyssey->command_buffers[image_index], odyssey->render_pass,
        odyssey->swapchain_framebuffers[image_index],
        odyssey->wsi[odyssey->current_wsi].swapchain_image_extent, odyssey->graphics_pipeline.pipeline);

    VkSubmitInfo submit_info = {};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore wait_semaphores[] = {
        odyssey->image_available_semaphores[odyssey->current_frame]
    };

    VkPipelineStageFlags wait_stages[] = {
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
    };

    submit_info.waitSemaphoreCount = ArraySize(wait_semaphores);
    submit_info.pWaitSemaphores = wait_semaphores;
    submit_info.pWaitDstStageMask = wait_stages;
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &odyssey->command_buffers[image_index];

    VkSemaphore signal_semaphores [] = {
        odyssey->render_finished_semaphores[odyssey->current_frame]
    };
    submit_info.signalSemaphoreCount = ArraySize(signal_semaphores);
    submit_info.pSignalSemaphores = signal_semaphores;

    vkResetFences(odyssey->device, 1, &odyssey->in_flight_fences[odyssey->current_frame]);

    VK_ASSERT(vkQueueSubmit(odyssey->graphics_queue, 1, &submit_info, odyssey->in_flight_fences[odyssey->current_frame]));

    VkPresentInfoKHR present_info = {};
    present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present_info.waitSemaphoreCount = ArraySize(signal_semaphores);
    present_info.pWaitSemaphores = signal_semaphores;

    VkSwapchainKHR swapchains[] = {
        odyssey->wsi[odyssey->current_wsi].swapchain
    };

    present_info.swapchainCount = ArraySize(swapchains);
    present_info.pSwapchains = swapchains;
    present_info.pImageIndices = &image_index;
    present_info.pResults = nullptr;

    result = vkQueuePresentKHR(odyssey->graphics_queue, &present_info);

    if(result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        vk_reinit_wsi(odyssey);
    }
    else if( result != VK_SUCCESS)
    {
        ODS_ASSERT(1);
    }

    ++odyssey->current_frame;
    odyssey->current_frame = (odyssey->current_frame) % odyssey->total_frames;
}
