#ifndef ODYSSEY_VULKAN_VK_PIPELINE_H
#define ODYSSEY_VULKAN_VK_PIPELINE_H

typedef struct GraphicsPipeline
{
    VkPipeline pipeline;
    // VkPipelineCache pipeline_cache;
    VkPipelineLayout pipeline_layout;
} GraphicsPipeline, *pGraphicsPipeline;

static void vk_create_shader_module(VkDevice device, VkShaderModule *shader_module, const char *file_name)
{
    ods::File shader_code;
    uint32_t result = PLATFORM_READ_FILE(file_name, &shader_code);

    ODS_ASSERT(result == 0);

    VkShaderModuleCreateInfo shader_module_create_info = {};
    shader_module_create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shader_module_create_info.pNext = 0;
    shader_module_create_info.flags = 0;
    shader_module_create_info.codeSize = shader_code.size;
    shader_module_create_info.pCode = (uint32_t*)shader_code.data;

    VK_ASSERT(vkCreateShaderModule(device, &shader_module_create_info, 0, shader_module));

    PLATFORM_FREE_FILE_MEMORY(&shader_code);
}

static void vk_create_graphics_pipeline(VkDevice device, VkRenderPass render_pass, pGraphicsPipeline pipeline,
    VkPipelineCache pipeline_cache,
    VkShaderModule vertex_shader_module, VkShaderModule fragment_shader_module, VkExtent2D extent)
{
    VkPipelineShaderStageCreateInfo vertex_shader_stage_create_info = {};
    vertex_shader_stage_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertex_shader_stage_create_info.pNext = 0;
    vertex_shader_stage_create_info.flags = 0;
    vertex_shader_stage_create_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertex_shader_stage_create_info.module = vertex_shader_module;
    vertex_shader_stage_create_info.pName = "main";
    vertex_shader_stage_create_info.pSpecializationInfo = 0;

    VkPipelineShaderStageCreateInfo frag_shader_stage_create_info = {};
    frag_shader_stage_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    frag_shader_stage_create_info.pNext = 0;
    frag_shader_stage_create_info.flags = 0;
    frag_shader_stage_create_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    frag_shader_stage_create_info.module = fragment_shader_module;
    frag_shader_stage_create_info.pName = "main";
    frag_shader_stage_create_info.pSpecializationInfo = 0;

    VkPipelineShaderStageCreateInfo pipeline_shader_stages [] = {
        vertex_shader_stage_create_info,
        frag_shader_stage_create_info
    };

    VkPipelineVertexInputStateCreateInfo vertex_input_state_create_info = {};
    vertex_input_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertex_input_state_create_info.pNext = 0;
    vertex_input_state_create_info.vertexBindingDescriptionCount = 0;
    vertex_input_state_create_info.pVertexBindingDescriptions = 0;
    vertex_input_state_create_info.vertexAttributeDescriptionCount = 0;
    vertex_input_state_create_info.pVertexAttributeDescriptions = 0;

    VkPipelineInputAssemblyStateCreateInfo input_assembly_state_create_info = {};
    input_assembly_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    input_assembly_state_create_info.pNext = 0;
    input_assembly_state_create_info.flags = 0;
    input_assembly_state_create_info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    input_assembly_state_create_info.primitiveRestartEnable = VK_FALSE;

    VkPipelineTessellationStateCreateInfo tesselation_state_create_info = {};
    tesselation_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
    tesselation_state_create_info.pNext = 0;
    tesselation_state_create_info.flags = 0;
    tesselation_state_create_info.patchControlPoints = 0;

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
    VkPipelineViewportStateCreateInfo viewport_state_create_info = {};
    viewport_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewport_state_create_info.pNext = 0;
    viewport_state_create_info.flags = 0;
    viewport_state_create_info.viewportCount = 1;
    viewport_state_create_info.pViewports = 0;
    viewport_state_create_info.scissorCount = 1;
    viewport_state_create_info.pScissors = 0;

    VkPipelineRasterizationStateCreateInfo rasterization_state_create_info = {};
    rasterization_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterization_state_create_info.pNext = 0;
    rasterization_state_create_info.flags = 0;
    rasterization_state_create_info.depthClampEnable = VK_FALSE;
    rasterization_state_create_info.rasterizerDiscardEnable = VK_FALSE;
    rasterization_state_create_info.polygonMode = VK_POLYGON_MODE_FILL;
    rasterization_state_create_info.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterization_state_create_info.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterization_state_create_info.depthBiasEnable = VK_FALSE;
    rasterization_state_create_info.depthBiasConstantFactor = 0.0f;
    rasterization_state_create_info.depthBiasClamp = 0.0f;
    rasterization_state_create_info.depthBiasSlopeFactor = 0.0f;
    rasterization_state_create_info.lineWidth = 1.0f;

    // TODO: Add option for multisampling.
    VkPipelineMultisampleStateCreateInfo multisample_state_create_info = {};
    multisample_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisample_state_create_info.pNext = 0;
    multisample_state_create_info.flags = 0;
    multisample_state_create_info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisample_state_create_info.sampleShadingEnable = VK_FALSE;
    multisample_state_create_info.minSampleShading = 1.0f; // Optional.
    multisample_state_create_info.pSampleMask = 0; // Optional.
    multisample_state_create_info.alphaToCoverageEnable = VK_FALSE; // Optional.
    multisample_state_create_info.alphaToOneEnable = VK_FALSE; // Optional.

    // TODO: Add depth/stencil buffer.
    VkPipelineDepthStencilStateCreateInfo depth_stencil_state_create_info = {};
    depth_stencil_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;

    // Fragment color is being passed unmodified.
    VkPipelineColorBlendAttachmentState color_blend_attachment_state = {};
    color_blend_attachment_state.blendEnable = VK_FALSE;
    color_blend_attachment_state.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
    color_blend_attachment_state.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
    color_blend_attachment_state.colorBlendOp = VK_BLEND_OP_ADD; // Optional
    color_blend_attachment_state.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
    color_blend_attachment_state.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
    color_blend_attachment_state.alphaBlendOp = VK_BLEND_OP_ADD; // Optional
    color_blend_attachment_state.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
        VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

/*
    // Alpha Blending.
    // finalColor.rgb = newAlpha * newColor + (1 - newAlpha) * oldColor;
    // finalColor.a = newAlpha.a;
    color_blend_attachment_state.blendEnable = VK_TRUE;
    color_blend_attachment_state.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    color_blend_attachment_state.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    color_blend_attachment_state.colorBlendOp = VK_BLEND_OP_ADD;
    color_blend_attachment_state.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    color_blend_attachment_state.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    color_blend_attachment_state.alphaBlendOp = VK_BLEND_OP_ADD;
*/

    VkPipelineColorBlendStateCreateInfo color_blend_state_create_info = {};
    color_blend_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    color_blend_state_create_info.pNext = 0;
    color_blend_state_create_info.flags = 0;
    color_blend_state_create_info.logicOpEnable = VK_FALSE;
    // color_blend_state_create_info.logicOp
    color_blend_state_create_info.attachmentCount = 1;
    color_blend_state_create_info.pAttachments = &color_blend_attachment_state;
    color_blend_state_create_info.blendConstants[0] = 0.0f; // Optional
    color_blend_state_create_info.blendConstants[1] = 0.0f; // Optional
    color_blend_state_create_info.blendConstants[2] = 0.0f; // Optional
    color_blend_state_create_info.blendConstants[3] = 0.0f; // Optional

/*
    VkDynamicState dynamic_states [] = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR,
        VK_DYNAMIC_STATE_LINE_WIDTH
    };
*/

    VkDynamicState dynamic_states [] = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };

    VkPipelineDynamicStateCreateInfo dynamic_state_create_info = {};
    dynamic_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamic_state_create_info.pNext = 0;
    dynamic_state_create_info.flags = 0;
    dynamic_state_create_info.dynamicStateCount = ArraySize(dynamic_states);
    dynamic_state_create_info.pDynamicStates = &dynamic_states[0];

    VkPipelineLayoutCreateInfo pipeline_layout_create_info = {};
    pipeline_layout_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipeline_layout_create_info.pNext = 0;
    pipeline_layout_create_info.flags = 0;
    // check VkPhysicalDeviceLimites::maxBoundDescriptorSets
    pipeline_layout_create_info.setLayoutCount = 0; // Optional.
    pipeline_layout_create_info.pSetLayouts = 0; // Optional.
    pipeline_layout_create_info.pushConstantRangeCount = 0; // Optional.
    pipeline_layout_create_info.pPushConstantRanges = 0; // Optional.

    VK_ASSERT(vkCreatePipelineLayout(device, &pipeline_layout_create_info, 0, &pipeline->pipeline_layout));


    VkGraphicsPipelineCreateInfo graphics_pipeline_create_info = {};
    graphics_pipeline_create_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    graphics_pipeline_create_info.pNext = 0;
    graphics_pipeline_create_info.flags = 0;
    graphics_pipeline_create_info.stageCount = ArraySize(pipeline_shader_stages);
    graphics_pipeline_create_info.pStages = &pipeline_shader_stages[0];
    graphics_pipeline_create_info.pVertexInputState = &vertex_input_state_create_info;
    graphics_pipeline_create_info.pInputAssemblyState = &input_assembly_state_create_info;
    graphics_pipeline_create_info.pTessellationState = &tesselation_state_create_info;
    graphics_pipeline_create_info.pViewportState = &viewport_state_create_info;
    graphics_pipeline_create_info.pRasterizationState = &rasterization_state_create_info;
    graphics_pipeline_create_info.pMultisampleState = &multisample_state_create_info;
    graphics_pipeline_create_info.pDepthStencilState = &depth_stencil_state_create_info;
    graphics_pipeline_create_info.pColorBlendState = &color_blend_state_create_info;
    graphics_pipeline_create_info.pDynamicState = &dynamic_state_create_info;
    graphics_pipeline_create_info.layout = pipeline->pipeline_layout;
    graphics_pipeline_create_info.renderPass = render_pass;
    graphics_pipeline_create_info.subpass = 0;
    graphics_pipeline_create_info.basePipelineHandle = VK_NULL_HANDLE;
    graphics_pipeline_create_info.basePipelineIndex = 0;

    VK_ASSERT(vkCreateGraphicsPipelines(device, pipeline_cache, 1, &graphics_pipeline_create_info, 0,
        &pipeline->pipeline));

    vkDestroyShaderModule(device, fragment_shader_module, 0);
    vkDestroyShaderModule(device, vertex_shader_module, 0);
}

static void vk_destroy_graphics_pipeline(VkDevice device, pGraphicsPipeline pipeline)
{
    vkDestroyPipeline(device, pipeline->pipeline, 0);
    vkDestroyPipelineLayout(device, pipeline->pipeline_layout, 0);
}

#endif
