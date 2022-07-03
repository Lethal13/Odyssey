#ifndef ODYSSEY_VULKAN_VK_RENDER_PASS_H
#define ODYSSEY_VULKAN_VK_RENDER_PASS_H

static void vk_create_render_pass(VkDevice device, VkRenderPass *render_pass, VkFormat format)
{
    VkAttachmentDescription color_attachment_description = {};
    color_attachment_description.flags = 0;
    color_attachment_description.format = format;
    color_attachment_description.samples = VK_SAMPLE_COUNT_1_BIT;
    color_attachment_description.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    color_attachment_description.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    color_attachment_description.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    color_attachment_description.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    // VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
    // VK_IMAGE_LAYOUT_UNDEFINED
    // VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
    color_attachment_description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    color_attachment_description.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference color_attachment_reference = {};
    color_attachment_reference.attachment = 0;
    color_attachment_reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass_description = {};
    subpass_description.flags = 0;
    subpass_description.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    // Each element of the pInputAttachments array corresponds to an input attachment
    // index in a fragment shader
    // subpass_description.inputAttachmentCount
    // subpass_description.pInputAttachments
    // Each element of the pColorAttachments array corresponds to an output location
    // in the shader.
    subpass_description.colorAttachmentCount = 1;
    subpass_description.pColorAttachments = &color_attachment_reference;
    subpass_description.pResolveAttachments = 0;
    subpass_description.pDepthStencilAttachment = 0;
    subpass_description.preserveAttachmentCount = 0;
    subpass_description.pPreserveAttachments = 0;

    // NOTE + TODO: Check subpass dependency again.
    VkSubpassDependency subpass_dependency = {};
    subpass_dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    subpass_dependency.dstSubpass = 0;
    subpass_dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    subpass_dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    subpass_dependency.srcAccessMask = 0;
    subpass_dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    // TODO: Check dependency flags.
    subpass_dependency.dependencyFlags = 0;

    VkRenderPassCreateInfo render_pass_create_info = {};
    render_pass_create_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    render_pass_create_info.pNext = 0;
    render_pass_create_info.flags = 0;
    render_pass_create_info.attachmentCount = 1;
    render_pass_create_info.pAttachments = &color_attachment_description;
    render_pass_create_info.subpassCount = 1;
    render_pass_create_info.pSubpasses = &subpass_description;
    render_pass_create_info.dependencyCount = 1;
    render_pass_create_info.pDependencies = &subpass_dependency;

    VK_ASSERT(vkCreateRenderPass(device, &render_pass_create_info, 0, render_pass));
}

#endif
