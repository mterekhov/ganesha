#include "grendergraph.h"
#include "gspritenode.h"
#include "gvulkantools.h"

namespace spcGaneshaEngine {

GRenderGraph::GRenderGraph(GLog& log) : log(log) {
}

GRenderGraph::~GRenderGraph() {
    
}

void GRenderGraph::createGraph(GVULKANDevice& vulkanDevice, VkCommandPool commandPool) {
    descriptorsetsPool = createDescriptorsetsPool(vulkanDevice.getLogicalDevice());
    descriptorsetsLayout = createDescriptorsetsLayout(vulkanDevice.getLogicalDevice());
    descriptorset = createDescriptorset(vulkanDevice.getLogicalDevice());
    materialsService = new GMaterialsService(log, commandPool);
}

void GRenderGraph::destroyGraph(VkDevice device) {
    for (auto node:graphNodeArray) {
        node->node->destroyNode(device);
        delete node->node;
        delete node;
    }
    
    for (auto shader:shadersArray) {
        vkDestroyShaderModule(device, shader.module, nullptr);
    }
    
    materialsService->destroyMaterials(device);
    delete materialsService;
    
    vkDestroyDescriptorPool(device, descriptorsetsPool, nullptr);
    vkDestroyDescriptorSetLayout(device, descriptorsetsLayout, nullptr);
}

void GRenderGraph::loadContent(GGaneshaContent& contentLoader, GVULKANDevice& vulkanDevice, VkCommandPool commandPool) {
    //  loading shaders
    std::vector<VkPipelineShaderStageCreateInfo> shadersArray;
    for (auto shader:contentLoader.getVertexShadersArray()) {
        shadersArray.push_back(createShader(shader, VK_SHADER_STAGE_VERTEX_BIT, vulkanDevice.getLogicalDevice()));
    }
    for (auto shader:contentLoader.getFragmetShadersArrray()) {
        shadersArray.push_back(createShader(shader, VK_SHADER_STAGE_FRAGMENT_BIT, vulkanDevice.getLogicalDevice()));
    }
    
    //  creating sprites
    for (auto spriteMaterial:contentLoader.getSpritesMaterialsArray()) {
        GGraphNode *newSprite = createSpriteNode(spriteMaterial, vulkanDevice, commandPool);
        pushNode(newSprite);
    }
}

GGraphNode *GRenderGraph::createSpriteNode(const std::string& materialFilePath, GVULKANDevice& vulkanDevice, VkCommandPool commandPool) {
    //  creating mesh
    GVULKANImage *material = materialsService->findMaterial(materialFilePath);
    if (material == 0) {
        material = materialsService->createMaterial(materialFilePath, vulkanDevice);
        attachImageToDescriptorset(*material, descriptorset, 2, vulkanDevice.getLogicalDevice());
    }
    GSpriteNode *spriteMesh = new GSpriteNode(material, vulkanDevice, commandPool, log);
    
    //  creating mesh instance
    GGraphNode *meshInstance = new GGraphNode(spriteMesh);
    return meshInstance;
}

void GRenderGraph::pushNode(GGraphNode *node) {
    graphNodeArray.push_back(node);
}

std::vector<GGraphNode *>& GRenderGraph::getNodeArray() {
    return graphNodeArray;
}

VkPipelineShaderStageCreateInfo GRenderGraph::createShader(const std::string& shaderFile, const VkShaderStageFlagBits stage, VkDevice device) {
    GVULKANTools tools;
    const std::vector<uint8_t> code = tools.readFile(shaderFile);
    VkShaderModuleCreateInfo createInfo = { };
    
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const TUInt*>(code.data());
    
    VkShaderModule shaderModule;
    if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        log.error("No chance to create shader module\n");
    }
    
    VkPipelineShaderStageCreateInfo shaderStageInfo{};
    shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStageInfo.stage = stage;
    shaderStageInfo.module = shaderModule;
    shaderStageInfo.pName = "main";
    
    return shaderStageInfo;
}

std::vector<VkPipelineShaderStageCreateInfo>& GRenderGraph::getShadersArray() {
    return shadersArray;
}

#pragma mark - Descriptor sets -

VkDescriptorSet GRenderGraph::getDescriptorset() {
    return descriptorset;
}

VkDescriptorSetLayout GRenderGraph::getDescriptoprsetsLayout() {
    return descriptorsetsLayout;
}

VkDescriptorPool GRenderGraph::createDescriptorsetsPool(VkDevice device) {
    std::array<VkDescriptorPoolSize, 3> poolSizesArray;
    poolSizesArray[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizesArray[0].descriptorCount = 1;
    
    poolSizesArray[1].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizesArray[1].descriptorCount = 1;
    
    poolSizesArray[2].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizesArray[2].descriptorCount = 1;
    
    VkDescriptorPoolCreateInfo poolInfo = { };
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = poolSizesArray.size();
    poolInfo.pPoolSizes = poolSizesArray.data();
    poolInfo.maxSets = 1;
    
    VkDescriptorPool newPool;
    vkCreateDescriptorPool(device, &poolInfo, nullptr, &newPool);

    return newPool;
}

VkDescriptorSetLayout GRenderGraph::createDescriptorsetsLayout(VkDevice device) {
    VkDescriptorSetLayoutBinding projectionBinding = { };
    projectionBinding.binding = 0;
    projectionBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    projectionBinding.descriptorCount = 1;
    projectionBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    
    VkDescriptorSetLayoutBinding modelBinding = { };
    modelBinding.binding = 1;
    modelBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    modelBinding.descriptorCount = 1;
    modelBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    
    VkDescriptorSetLayoutBinding fragmentBinding = { };
    fragmentBinding.binding = 2;
    fragmentBinding.descriptorCount = 1;
    fragmentBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    fragmentBinding.pImmutableSamplers = nullptr;
    fragmentBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    
    std::array<VkDescriptorSetLayoutBinding, 3> bindingsArray = { projectionBinding, modelBinding, fragmentBinding };
    
    VkDescriptorSetLayoutCreateInfo layoutInfo = { };
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = bindingsArray.size();
    layoutInfo.pBindings = bindingsArray.data();
    
    VkDescriptorSetLayout newLayout;
    vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &newLayout);
    
    return newLayout;
}

VkDescriptorSet GRenderGraph::createDescriptorset(VkDevice device) {
    VkDescriptorSetAllocateInfo allocInfo = { };
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorsetsPool;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &descriptorsetsLayout;
    VkDescriptorSet newDescriptorset;
    vkAllocateDescriptorSets(device, &allocInfo, &newDescriptorset);
    
    return newDescriptorset;
}

void GRenderGraph::attachBufferToDescriptorset(GVULKANBuffer& buffer, VkDescriptorSet dstDescriptorset, TUInt bindingIndex, VkDevice device) {
    VkDescriptorBufferInfo bufferInfo = { };
    bufferInfo.buffer = buffer.getBuffer();
    bufferInfo.offset = 0;
    bufferInfo.range = buffer.getBufferSize();
    
    VkWriteDescriptorSet writeDescriptorset;
    writeDescriptorset.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorset.dstSet = dstDescriptorset;
    writeDescriptorset.dstBinding = bindingIndex;
    writeDescriptorset.dstArrayElement = 0;
    writeDescriptorset.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    writeDescriptorset.descriptorCount = 1;
    writeDescriptorset.pBufferInfo = &bufferInfo;
    
    vkUpdateDescriptorSets(device, 1, &writeDescriptorset, 0, nullptr);
}

void GRenderGraph::attachImageToDescriptorset(GVULKANImage& image, VkDescriptorSet dstDescriptorset, TUInt bindingIndex, VkDevice device) {
    VkDescriptorImageInfo imageInfo = { };
    imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    imageInfo.imageView = image.getImageView();
    imageInfo.sampler = image.getSampler();

    VkWriteDescriptorSet writeDescriptorset;
    writeDescriptorset.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorset.dstSet = dstDescriptorset;
    writeDescriptorset.dstBinding = bindingIndex;
    writeDescriptorset.dstArrayElement = 0;
    writeDescriptorset.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    writeDescriptorset.descriptorCount = 1;
    writeDescriptorset.pImageInfo = &imageInfo;
    
    vkUpdateDescriptorSets(device, 1, &writeDescriptorset, 0, nullptr);
}

//std::vector<VkDescriptorSet> GVULKANDescriptorsets::createNewDescriptorsets(std::vector<GVULKANBuffer>& projectionBuffersArray,
//                                                                            std::vector<GVULKANBuffer>& modelBuffersArray,
//                                                                            GVULKANImage& image,
//                                                                            VkDescriptorSetLayout descriptorsetLayout,
//                                                                            VkDevice device) {
//    std::vector<VkDescriptorSetLayout> layoutsArray(poolSize, descriptorsetLayout);
//
//    VkDescriptorSetAllocateInfo allocInfo = { };
//    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
//    allocInfo.descriptorPool = pool;
//    allocInfo.descriptorSetCount = poolSize;
//    allocInfo.pSetLayouts = layoutsArray.data();
//
//    std::vector<VkDescriptorSet> newDescriptorsetArray;
//    newDescriptorsetArray.resize(poolSize);
//    if (vkAllocateDescriptorSets(device, &allocInfo, newDescriptorsetArray.data()) != VK_SUCCESS) {
//        log.error("failed to allocate descriptor sets\n");
//    }
//
//    for (size_t i = 0; i < poolSize; i++) {
//        VkDescriptorBufferInfo projectionBufferInfo = { };
//        projectionBufferInfo.buffer = projectionBuffersArray[i].getBuffer();
//        projectionBufferInfo.offset = 0;
//        projectionBufferInfo.range = projectionBuffersArray[i].getBufferSize();
//
//        VkDescriptorBufferInfo modelBufferInfo = { };
//        modelBufferInfo.buffer = modelBuffersArray[i].getBuffer();
//        modelBufferInfo.offset = 0;
//        modelBufferInfo.range = modelBuffersArray[i].getBufferSize();
//
//        VkDescriptorImageInfo imageInfo = { };
//        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
//        imageInfo.imageView = image.getImageView();
//        imageInfo.sampler = image.getSampler();
//
//        std::array<VkWriteDescriptorSet, 3> descriptorWritesArray = { };
//        descriptorWritesArray[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
//        descriptorWritesArray[0].dstSet = newDescriptorsetArray[i];
//        descriptorWritesArray[0].dstBinding = 0;
//        descriptorWritesArray[0].dstArrayElement = 0;
//        descriptorWritesArray[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
//        descriptorWritesArray[0].descriptorCount = 1;
//        descriptorWritesArray[0].pBufferInfo = &projectionBufferInfo;
//
//        descriptorWritesArray[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
//        descriptorWritesArray[1].dstSet = newDescriptorsetArray[i];
//        descriptorWritesArray[1].dstBinding = 1;
//        descriptorWritesArray[1].dstArrayElement = 0;
//        descriptorWritesArray[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
//        descriptorWritesArray[1].descriptorCount = 1;
//        descriptorWritesArray[1].pBufferInfo = &modelBufferInfo;
//
//        descriptorWritesArray[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
//        descriptorWritesArray[2].dstSet = newDescriptorsetArray[i];
//        descriptorWritesArray[2].dstBinding = 2;
//        descriptorWritesArray[2].dstArrayElement = 0;
//        descriptorWritesArray[2].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
//        descriptorWritesArray[2].descriptorCount = 1;
//        descriptorWritesArray[2].pImageInfo = &imageInfo;
//
//        vkUpdateDescriptorSets(device, descriptorWritesArray.size(), descriptorWritesArray.data(), 0, nullptr);
//    }
//
//    return newDescriptorsetArray;
//}
//
};  //  spcGaneshaEngine
