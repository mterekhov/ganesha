#include "gvulkanbuffer.h"

namespace spcGaneshaEngine {

GVULKANBuffer::GVULKANBuffer(GLog& log) : log(log) {
    
}

GVULKANBuffer::~GVULKANBuffer() {
    
}

VkBuffer& GVULKANBuffer::getBuffer() {
    return buffer;
}

void GVULKANBuffer::createBuffer(const void *data, const VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, bool protectAccess, GVULKANDevice& vulkanDevice, GVULKANCommands& vulkanCommands) {
    if (protectAccess) {
        VkBuffer stagingBuffer = createBuffer(vulkanDevice.getLogicalDevice(), size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
        VkDeviceMemory stagingBufferMemory = allocateBufferMemory(stagingBuffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, vulkanDevice);
        
        void *mappedData;
        vkMapMemory(vulkanDevice.getLogicalDevice(), stagingBufferMemory, 0, size, 0, &mappedData);
        memcpy(mappedData, data, static_cast<size_t>(size));
        vkUnmapMemory(vulkanDevice.getLogicalDevice(), stagingBufferMemory);
        
        buffer = createBuffer(vulkanDevice.getLogicalDevice(), size, usage);
        bufferMemory = allocateBufferMemory(buffer, properties, vulkanDevice);
        
        copyBuffer(stagingBuffer, buffer, size, vulkanDevice, vulkanCommands);
        
        vkDestroyBuffer(vulkanDevice.getLogicalDevice(), stagingBuffer, nullptr);
        vkFreeMemory(vulkanDevice.getLogicalDevice(), stagingBufferMemory, nullptr);
    }
    else {
        buffer = createBuffer(vulkanDevice.getLogicalDevice(), size, usage);
        bufferMemory = allocateBufferMemory(buffer, properties, vulkanDevice);

        void *mappedData;
        vkMapMemory(vulkanDevice.getLogicalDevice(), bufferMemory, 0, size, 0, &mappedData);
        memcpy(mappedData, data, static_cast<size_t>(size));
        vkUnmapMemory(vulkanDevice.getLogicalDevice(), bufferMemory);
    }
}

void GVULKANBuffer::destroyBuffer(GVULKANDevice& vulkanDevice) {
    vkDestroyBuffer(vulkanDevice.getLogicalDevice(), buffer, nullptr);
    vkFreeMemory(vulkanDevice.getLogicalDevice(), bufferMemory, nullptr);

}

void GVULKANBuffer::refreshBuffer(const void *data, const VkDeviceSize size, GVULKANDevice& vulkanDevice) {
    void *mappedData;
    vkMapMemory(vulkanDevice.getLogicalDevice(), bufferMemory, 0, size, 0, &mappedData);
    memcpy(mappedData, data, static_cast<size_t>(size));
    vkUnmapMemory(vulkanDevice.getLogicalDevice(), bufferMemory);
}

#pragma mark - Routine -

VkBuffer GVULKANBuffer::createBuffer(VkDevice& device, VkDeviceSize size, VkBufferUsageFlags usage) {
    VkBufferCreateInfo bufferInfo = { };
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    
    VkBuffer newBuffer;
    if (vkCreateBuffer(device, &bufferInfo, nullptr, &newBuffer) != VK_SUCCESS) {
        log.error("failed to create vertex buffer\n");
    }
    
    return newBuffer;
}

VkDeviceMemory GVULKANBuffer::allocateBufferMemory(VkBuffer& originalBuffer, VkMemoryPropertyFlags properties, GVULKANDevice& vulkanDevice) {
    VkMemoryRequirements memoryRequirements;
    vkGetBufferMemoryRequirements(vulkanDevice.getLogicalDevice(), originalBuffer, &memoryRequirements);
    
    VkMemoryAllocateInfo memoryAllocateInfo = { };
    memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memoryAllocateInfo.allocationSize = memoryRequirements.size;
    memoryAllocateInfo.memoryTypeIndex = findMemoryType(vulkanDevice.getPhysicalDevice(), memoryRequirements.memoryTypeBits, properties);
    
    VkDeviceMemory newBufferMemory;
    if (vkAllocateMemory(vulkanDevice.getLogicalDevice(), &memoryAllocateInfo, nullptr, &newBufferMemory) != VK_SUCCESS) {
        log.error("failed to allocate vertex buffer memory\n");
    }
    vkBindBufferMemory(vulkanDevice.getLogicalDevice(), originalBuffer, newBufferMemory, 0);
    
    return newBufferMemory;
}

void GVULKANBuffer::copyBuffer(VkBuffer& srcBuffer, VkBuffer& dstBuffer, const VkDeviceSize size, GVULKANDevice& vulkanDevice, GVULKANCommands& vulkanCommands) {
    VkCommandBuffer commandBuffer = vulkanCommands.copyBufferCommand(srcBuffer, dstBuffer, size, vulkanDevice.getLogicalDevice());
    
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;
    vkQueueSubmit(vulkanDevice.getGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(vulkanDevice.getGraphicsQueue());
    
    vulkanCommands.destroyCommandBuffer(commandBuffer, vulkanDevice.getLogicalDevice());
}

uint32_t GVULKANBuffer::findMemoryType(const VkPhysicalDevice& device, uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memoryProperties;
    vkGetPhysicalDeviceMemoryProperties(device, &memoryProperties);
    
    for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) &&
            (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }
    
    return 0;
}

}
