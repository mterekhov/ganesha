#include "gvulkandescriptorsets.h"

namespace spcGaneshaEngine {

GVULKANDescriptorsets::GVULKANDescriptorsets(GLog& log) : log(log) {
}

GVULKANDescriptorsets::~GVULKANDescriptorsets() {
    
}

void GVULKANDescriptorsets::createDescriptorsets(GVULKANDevice& vulkanDevice,
                                                 std::vector<GVULKANBuffer>& projectionBuffersArray,
                                                 std::vector<GVULKANBuffer>& modelBuffersArray,
                                                 GVULKANImage& image) {
    layout = createLayout(vulkanDevice.getLogicalDevice());
    pool = createDescriptorPool(static_cast<TUInt>(projectionBuffersArray.size()), vulkanDevice.getLogicalDevice());
    descriptorsetArray = createNewDescriptorsets(projectionBuffersArray, modelBuffersArray, image, layout, vulkanDevice.getLogicalDevice());
}

void GVULKANDescriptorsets::destroyDescriptorsets(GVULKANDevice& vulkanDevice) {
    vkDestroyDescriptorPool(vulkanDevice.getLogicalDevice(), pool, nullptr);
    vkDestroyDescriptorSetLayout(vulkanDevice.getLogicalDevice(), layout, nullptr);
}

std::vector<VkDescriptorSet>& GVULKANDescriptorsets::getDescriptorsetArray() {
    return descriptorsetArray;
}

VkDescriptorSetLayout GVULKANDescriptorsets::getDescriptorsetLayout() {
    return layout;
}

#pragma mark - Routine -

std::vector<VkDescriptorSet> GVULKANDescriptorsets::createNewDescriptorsets(std::vector<GVULKANBuffer>& projectionBuffersArray,
                                                                            std::vector<GVULKANBuffer>& modelBuffersArray,
                                                                            GVULKANImage& image,
                                                                            VkDescriptorSetLayout descriptorsetLayout,
                                                                            VkDevice device) {
    std::vector<VkDescriptorSetLayout> layoutsArray(poolSize, descriptorsetLayout);
    
    VkDescriptorSetAllocateInfo allocInfo = { };
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = pool;
    allocInfo.descriptorSetCount = poolSize;
    allocInfo.pSetLayouts = layoutsArray.data();
    
    std::vector<VkDescriptorSet> newDescriptorsetArray;
    newDescriptorsetArray.resize(poolSize);
    if (vkAllocateDescriptorSets(device, &allocInfo, newDescriptorsetArray.data()) != VK_SUCCESS) {
        log.error("failed to allocate descriptor sets\n");
    }
    
    for (size_t i = 0; i < poolSize; i++) {
        VkDescriptorBufferInfo projectionBufferInfo = { };
        projectionBufferInfo.buffer = projectionBuffersArray[i].getBuffer();
        projectionBufferInfo.offset = 0;
        projectionBufferInfo.range = projectionBuffersArray[i].getBufferSize();
        
        VkDescriptorBufferInfo modelBufferInfo = { };
        modelBufferInfo.buffer = modelBuffersArray[i].getBuffer();
        modelBufferInfo.offset = 0;
        modelBufferInfo.range = modelBuffersArray[i].getBufferSize();
        
        VkDescriptorImageInfo imageInfo = { };
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = image.getImageView();
        imageInfo.sampler = image.getSampler();
        
        std::array<VkWriteDescriptorSet, 3> descriptorWritesArray = { };
        descriptorWritesArray[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWritesArray[0].dstSet = newDescriptorsetArray[i];
        descriptorWritesArray[0].dstBinding = 0;
        descriptorWritesArray[0].dstArrayElement = 0;
        descriptorWritesArray[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWritesArray[0].descriptorCount = 1;
        descriptorWritesArray[0].pBufferInfo = &projectionBufferInfo;
        
        descriptorWritesArray[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWritesArray[1].dstSet = newDescriptorsetArray[i];
        descriptorWritesArray[1].dstBinding = 1;
        descriptorWritesArray[1].dstArrayElement = 0;
        descriptorWritesArray[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWritesArray[1].descriptorCount = 1;
        descriptorWritesArray[1].pBufferInfo = &modelBufferInfo;
        
        descriptorWritesArray[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWritesArray[2].dstSet = newDescriptorsetArray[i];
        descriptorWritesArray[2].dstBinding = 2;
        descriptorWritesArray[2].dstArrayElement = 0;
        descriptorWritesArray[2].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorWritesArray[2].descriptorCount = 1;
        descriptorWritesArray[2].pImageInfo = &imageInfo;
        
        vkUpdateDescriptorSets(device, descriptorWritesArray.size(), descriptorWritesArray.data(), 0, nullptr);
    }
    
    return newDescriptorsetArray;
}

VkDescriptorPool GVULKANDescriptorsets::createDescriptorPool(const TUInt descriptorsPoolSize, VkDevice device) {
    poolSize = descriptorsPoolSize;
    
    std::array<VkDescriptorPoolSize, 3> poolSizesArray;
    poolSizesArray[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizesArray[0].descriptorCount = poolSize;
    poolSizesArray[1].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizesArray[1].descriptorCount = poolSize;
    poolSizesArray[2].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizesArray[2].descriptorCount = poolSize;
    
    VkDescriptorPoolCreateInfo poolInfo = { };
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = poolSizesArray.size();
    poolInfo.pPoolSizes = poolSizesArray.data();
    poolInfo.maxSets = poolSize;
    
    VkDescriptorPool newPool;
    if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &newPool) != VK_SUCCESS) {
        log.error("failed to create descriptor pool\n");
    }
    
    return newPool;
}

VkDescriptorSetLayout GVULKANDescriptorsets::createLayout(VkDevice device) {
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
    if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &newLayout) != VK_SUCCESS) {
        log.error("failed to create descriptor set layout\n");
    }
    
    return newLayout;
}

}
