#include "gvulkanpipeline.h"
#include "gvulkantools.h"

namespace spcGaneshaEngine {

GVULKANPipeline::GVULKANPipeline() {
}

GVULKANPipeline::~GVULKANPipeline() {
}

void GVULKANPipeline::createPipeline(GVULKANDevice& vulkanDevice, GVULKANSwapChain& swapChain, GRenderGraph& renderGraph, VkDescriptorSetLayout descriptorsetLayout) {
    VkPipelineVertexInputStateCreateInfo vertexInputInfo = createVertexInput();
    
    VkPipelineInputAssemblyStateCreateInfo inputAssembly = { };
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;
    
    VkExtent2D extent = swapChain.getExtent();
    VkRect2D scissor = { };
    scissor.offset = {0, 0};
    scissor.extent = extent;
    VkViewport viewport = { };
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<TFloat>(extent.width);
    viewport.height = static_cast<TFloat>(extent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    VkPipelineViewportStateCreateInfo viewportState = { };
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;

    VkPipelineRasterizationStateCreateInfo rasterizer = createRasterizer();
    
    VkPipelineMultisampleStateCreateInfo multisampling = { };
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    
    VkPipelineColorBlendAttachmentState colorBlendAttachment = { };
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;
    VkPipelineColorBlendStateCreateInfo colorBlending = { };
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f; // Optional
    colorBlending.blendConstants[1] = 0.0f; // Optional
    colorBlending.blendConstants[2] = 0.0f; // Optional
    colorBlending.blendConstants[3] = 0.0f; // Optional
    
    std::vector<VkDynamicState> dynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };
    VkPipelineDynamicStateCreateInfo dynamicState = { };
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = static_cast<TUInt>(dynamicStates.size());
    dynamicState.pDynamicStates = dynamicStates.data();
    
    pipelineLayout = createPipelineLayout(vulkanDevice.getLogicalDevice(), descriptorsetLayout);
    
    VkPipelineDepthStencilStateCreateInfo depthStencil = { };
    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.depthTestEnable = VK_TRUE;
    depthStencil.depthWriteEnable = VK_TRUE;
    depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
    depthStencil.depthBoundsTestEnable = VK_FALSE;
    depthStencil.stencilTestEnable = VK_FALSE;

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = renderGraph.getShadersArray().size();
    pipelineInfo.pStages = renderGraph.getShadersArray().data();
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = &depthStencil;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.layout = pipelineLayout;
    pipelineInfo.renderPass = swapChain.getRenderPass();
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
    
    VkResult result = vkCreateGraphicsPipelines(vulkanDevice.getLogicalDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline);
    if (result != VK_SUCCESS) {
        GLOG_ERROR("error while creating pipeline itself\n");
    }
}

void GVULKANPipeline::destroyPipeline(GVULKANDevice& vulkanDevice) {
    VkDevice logicalDevice = vulkanDevice.getLogicalDevice();
    
    vkDestroyPipeline(logicalDevice, graphicsPipeline, nullptr);
    vkDestroyPipelineLayout(logicalDevice, pipelineLayout, nullptr);
}

VkPipeline GVULKANPipeline::getGraphicsPipeline() {
    return graphicsPipeline;
}

VkPipelineLayout GVULKANPipeline::getPipelineLayout() {
    return pipelineLayout;
}

#pragma mark - Routine -

VkPipelineVertexInputStateCreateInfo GVULKANPipeline::createVertexInput() {
    VkPipelineVertexInputStateCreateInfo vertexInput = { };
    
    vertexInput.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInput.vertexBindingDescriptionCount = 1;
    vertexInput.vertexAttributeDescriptionCount = static_cast<TUInt>(attributeDescriptions.size());
    vertexInput.pVertexBindingDescriptions = &bindingDescription;
    vertexInput.pVertexAttributeDescriptions = attributeDescriptions.data();
    
    return vertexInput;
}

VkPipelineRasterizationStateCreateInfo GVULKANPipeline::createRasterizer() {
    VkPipelineRasterizationStateCreateInfo rasterizer = { };
    
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;
    
    return rasterizer;
}

VkPipelineLayout GVULKANPipeline::createPipelineLayout(VkDevice device, VkDescriptorSetLayout descriptorsetLayout) {
    VkPipelineLayoutCreateInfo pipelineLayoutInfo = { };
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1; // Optional
    pipelineLayoutInfo.pSetLayouts = &descriptorsetLayout;
    pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
    
    VkPipelineLayout newPipelineLayout;
    if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &newPipelineLayout) != VK_SUCCESS) {
        GLOG_ERROR("error while creating pipeline layout\n");
    }
    
    return newPipelineLayout;
}

//VkPipelineShaderStageCreateInfo GVULKANPipeline::createVertexShaders(const TStringsArray& shadersArray, VkDevice device) {
//    VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
//    
//    for (const std::string& shaderFullFilePath : shadersArray) {
//        vertShaderStageInfo = createShader(shaderFullFilePath, VK_SHADER_STAGE_VERTEX_BIT, device);
//    }
//    
//    return vertShaderStageInfo;
//}
//
//VkPipelineShaderStageCreateInfo GVULKANPipeline::createFragmentShaders(const TStringsArray& shadersArray, VkDevice device) {
//    VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
//    
//    for (const std::string& shaderFullFilePath : shadersArray) {
//        fragShaderStageInfo = createShader(shaderFullFilePath, VK_SHADER_STAGE_FRAGMENT_BIT, device);
//    }
//    
//    return fragShaderStageInfo;
//}
//
//VkPipelineShaderStageCreateInfo GVULKANPipeline::createShader(const std::string& shaderFile, const VkShaderStageFlagBits stage, VkDevice device) {
//    GVULKANTools tools;
//    const std::vector<uint8_t> code = tools.readFile(shaderFile);
//    VkShaderModuleCreateInfo createInfo = { };
//    
//    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
//    createInfo.codeSize = code.size();
//    createInfo.pCode = reinterpret_cast<const TUInt*>(code.data());
//    
//    VkShaderModule shaderModule;
//    if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
//        GLOG_ERROR("No chance to create shader module\n");
//    }
//    
//    VkPipelineShaderStageCreateInfo shaderStageInfo{};
//    shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
//    shaderStageInfo.stage = stage;
//    shaderStageInfo.module = shaderModule;
//    shaderStageInfo.pName = "main";
//    
//    return shaderStageInfo;
//}

}
