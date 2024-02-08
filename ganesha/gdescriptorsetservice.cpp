#include "gdescriptorsetservice.h"

namespace spcGaneshaEngine {

GDescriptorsetService::GDescriptorsetService(GVULKANDevice& vulkanDevice) : vulkanDevice(vulkanDevice) {
    
}

GDescriptorsetService::~GDescriptorsetService() {
    
}

void GDescriptorsetService::init() {
    VkDevice device = vulkanDevice.getLogicalDevice();
    
    descriptorsetsPool = createDescriptorsetsPool(device);
    descriptorsetsLayout = createDescriptorsetsLayout(device);
    descriptorset = createDescriptorset(descriptorsetsPool, descriptorsetsLayout, device);
}

void GDescriptorsetService::destroy() {
    VkDevice device = vulkanDevice.getLogicalDevice();
    vkDestroyDescriptorPool(device, descriptorsetsPool, nullptr);
    vkDestroyDescriptorSetLayout(device, descriptorsetsLayout, nullptr);
}

VkDescriptorSetLayout GDescriptorsetService::getDescriptorsetLayout() {
    return descriptorsetsLayout;
}

VkDescriptorSet GDescriptorsetService::getDescriptorset() {
    return descriptorset;
}

void GDescriptorsetService::attachImageToDescriptorset(GVULKANImage& image, TUInt bindingIndex) {
    VkDescriptorImageInfo imageInfo = { };
    imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    imageInfo.imageView = image.getImageView();
    imageInfo.sampler = image.getSampler();

    VkWriteDescriptorSet writeDescriptorset = { };
    writeDescriptorset.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorset.dstSet = descriptorset;
    writeDescriptorset.dstBinding = bindingIndex;
    writeDescriptorset.dstArrayElement = 0;
    writeDescriptorset.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    writeDescriptorset.descriptorCount = 1;
    writeDescriptorset.pImageInfo = &imageInfo;
    
    vkUpdateDescriptorSets(vulkanDevice.getLogicalDevice(), 1, &writeDescriptorset, 0, nullptr);
}

void GDescriptorsetService::attachBufferToDescriptorset(GVULKANBuffer& buffer, TUInt bindingIndex) {
    VkDescriptorBufferInfo bufferInfo = { };
    bufferInfo.buffer = buffer.getBuffer();
    bufferInfo.offset = 0;
    bufferInfo.range = buffer.getBufferSize();
    
    VkWriteDescriptorSet writeDescriptorset = { };
    writeDescriptorset.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorset.dstSet = descriptorset;
    writeDescriptorset.dstBinding = bindingIndex;
    writeDescriptorset.dstArrayElement = 0;
    writeDescriptorset.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    writeDescriptorset.descriptorCount = 1;
    writeDescriptorset.pBufferInfo = &bufferInfo;
    
    vkUpdateDescriptorSets(vulkanDevice.getLogicalDevice(), 1, &writeDescriptorset, 0, nullptr);
}

VkDescriptorSet GDescriptorsetService::createDescriptorset(VkDescriptorPool pool, VkDescriptorSetLayout layout, VkDevice device) {
    VkDescriptorSetAllocateInfo allocInfo = { };
    
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = pool;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &layout;
    VkDescriptorSet newDescriptorset;
    vkAllocateDescriptorSets(vulkanDevice.getLogicalDevice(), &allocInfo, &newDescriptorset);
    
    return newDescriptorset;
}

VkDescriptorPool GDescriptorsetService::createDescriptorsetsPool(VkDevice device) {
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

VkDescriptorSetLayout GDescriptorsetService::createDescriptorsetsLayout(VkDevice device) {
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

}
