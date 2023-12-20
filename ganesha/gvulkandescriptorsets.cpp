#include "gvulkandescriptorsets.h"

namespace spcGaneshaEngine {

GVULKANDescriptorsets::GVULKANDescriptorsets(GLog& log) : log(log) {
}

GVULKANDescriptorsets::~GVULKANDescriptorsets() {
    
}

void GVULKANDescriptorsets::createDescriptorsets(GVULKANDevice& vulkanDevice, std::vector<GVULKANBuffer>& buffersArray) {
    layout = createLayout(vulkanDevice.getLogicalDevice());
    pool = createDescriptorPool(static_cast<TUInt>(buffersArray.size()), vulkanDevice.getLogicalDevice());
    descriptorsetArray = createNewDescriptorsets(buffersArray, layout, vulkanDevice.getLogicalDevice());
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

VkDescriptorPool GVULKANDescriptorsets::createDescriptorPool(const TUInt descriptorsPoolSize, VkDevice device) {
    poolSize = descriptorsPoolSize;
    
    VkDescriptorPoolSize newPoolSize = { };
    newPoolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    newPoolSize.descriptorCount = poolSize;
    
    VkDescriptorPoolCreateInfo poolInfo = { };
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = &newPoolSize;
    poolInfo.maxSets = poolSize;
    
    VkDescriptorPool newPool;
    if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &newPool) != VK_SUCCESS) {
        log.error("failed to create descriptor pool\n");
    }
    
    return newPool;
}

VkDescriptorSetLayout GVULKANDescriptorsets::createLayout(VkDevice device) {
    VkDescriptorSetLayoutBinding layoutBinding = { };
    layoutBinding.binding = 0;
    layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    layoutBinding.descriptorCount = 1;
    layoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    
    VkDescriptorSetLayoutCreateInfo layoutInfo = { };
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = 1;
    layoutInfo.pBindings = &layoutBinding;
    
    VkDescriptorSetLayout newLayout;
    if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &newLayout) != VK_SUCCESS) {
        log.error("failed to create descriptor set layout\n");
    }
    
    return newLayout;
}

std::vector<VkDescriptorSet> GVULKANDescriptorsets::createNewDescriptorsets(std::vector<GVULKANBuffer>& buffersArray, VkDescriptorSetLayout descriptorsetLayout, VkDevice device) {
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
        VkDescriptorBufferInfo bufferInfo = { };
        bufferInfo.buffer = buffersArray[i].getBuffer();
        bufferInfo.offset = 0;
        bufferInfo.range = buffersArray[i].getBufferSize();
        
        VkWriteDescriptorSet descriptorsetWrite = { };
        descriptorsetWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorsetWrite.dstSet = newDescriptorsetArray[i];
        descriptorsetWrite.dstBinding = 0;
        descriptorsetWrite.dstArrayElement = 0;
        descriptorsetWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorsetWrite.descriptorCount = 1;
        descriptorsetWrite.pBufferInfo = &bufferInfo;
        descriptorsetWrite.pImageInfo = nullptr; // Optional
        descriptorsetWrite.pTexelBufferView = nullptr; // Optional
        
        vkUpdateDescriptorSets(device, 1, &descriptorsetWrite, 0, nullptr);
    }
    
    return newDescriptorsetArray;
}

}
