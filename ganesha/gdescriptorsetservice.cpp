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

void GDescriptorsetService::updateDescriptorset(GVULKANImage& image, TUInt bindingIndex) {
    VkDescriptorImageInfo imageInfo = { };
    imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    imageInfo.imageView = image.imageView;
    imageInfo.sampler = image.sampler;
    
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

void GDescriptorsetService::updateDescriptorset(GVULKANBuffer& buffer, TUInt bindingIndex) {
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
    allocInfo.pSetLayouts = &layout;
    allocInfo.descriptorSetCount = 1;
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
    
    VkDescriptorSetLayoutBinding instanceBinding = { };
    instanceBinding.binding = 1;
    instanceBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    instanceBinding.descriptorCount = 1;
    instanceBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    
    VkDescriptorSetLayoutBinding fragmentBinding = { };
    fragmentBinding.binding = 2;
    fragmentBinding.descriptorCount = 1;
    fragmentBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    fragmentBinding.pImmutableSamplers = nullptr;
    fragmentBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    
    std::array<VkDescriptorSetLayoutBinding, 3> bindingsArray = { projectionBinding, instanceBinding, fragmentBinding };
    
    VkDescriptorSetLayoutCreateInfo layoutInfo = { };
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = bindingsArray.size();
    layoutInfo.pBindings = bindingsArray.data();
    
    VkDescriptorSetLayout newLayout;
    vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &newLayout);
    
    return newLayout;
}

std::vector<VkVertexInputBindingDescription> GDescriptorsetService::getBindingDescription() {
    std::vector<VkVertexInputBindingDescription> bindingDescriptions;
    
    VkVertexInputBindingDescription newBindingDescr {};
    newBindingDescr.binding = 0;
    newBindingDescr.stride = sizeof(TFloat) * 5;
    newBindingDescr.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    bindingDescriptions.push_back(newBindingDescr);

    newBindingDescr.binding = 1;
    newBindingDescr.stride = sizeof(TFloat) * 9;
    newBindingDescr.inputRate = VK_VERTEX_INPUT_RATE_INSTANCE;
    bindingDescriptions.push_back(newBindingDescr);

    return bindingDescriptions;
}

std::vector<VkVertexInputAttributeDescription> GDescriptorsetService::getAttributeDescriptions()  {
    std::vector<VkVertexInputAttributeDescription> attributeDescriptions;

    //  vertex coords
    VkVertexInputAttributeDescription newAttributeDescr {};
    newAttributeDescr.binding = 0;
    newAttributeDescr.location = 0;
    newAttributeDescr.format = VK_FORMAT_R32G32B32_SFLOAT;
    newAttributeDescr.offset = 0;
    attributeDescriptions.push_back(newAttributeDescr);

    //  vertex UV coords
    newAttributeDescr.binding = 0;
    newAttributeDescr.location = 1;
    newAttributeDescr.format = VK_FORMAT_R32G32_SFLOAT;
    newAttributeDescr.offset = sizeof(TFloat) * 3;
    attributeDescriptions.push_back(newAttributeDescr);

    //  translation
    newAttributeDescr.binding = 1;
    newAttributeDescr.location = 2;
    newAttributeDescr.format = VK_FORMAT_R32G32B32_SFLOAT;
    newAttributeDescr.offset = 0;
    attributeDescriptions.push_back(newAttributeDescr);

    //  rotation
    newAttributeDescr.binding = 1;
    newAttributeDescr.location = 3;
    newAttributeDescr.format = VK_FORMAT_R32G32B32_SFLOAT;
    newAttributeDescr.offset = sizeof(TFloat) * 3;
    attributeDescriptions.push_back(newAttributeDescr);

    //  scale
    newAttributeDescr.binding = 1;
    newAttributeDescr.location = 4;
    newAttributeDescr.format = VK_FORMAT_R32G32B32_SFLOAT;
    newAttributeDescr.offset = sizeof(TFloat) * 6;
    attributeDescriptions.push_back(newAttributeDescr);

    return attributeDescriptions;
}

}
