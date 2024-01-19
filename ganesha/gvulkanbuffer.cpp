#include "gvulkanbuffer.h"
#include "gvulkantools.h"

namespace spcGaneshaEngine {

GVULKANBuffer::GVULKANBuffer() {
    
}

GVULKANBuffer::~GVULKANBuffer() {
    
}

void GVULKANBuffer::createBuffer(const void *data, const VkDeviceSize size, const VkBufferUsageFlags usage, const VkMemoryPropertyFlags properties, const TBool protectAccess, GVULKANDevice& vulkanDevice, VkCommandPool commandPool) {
    if (protectAccess) {
        VkBuffer stagingBuffer = createBuffer(vulkanDevice.getLogicalDevice(), size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
        VkDeviceMemory stagingBufferMemory = allocateBufferMemory(stagingBuffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, vulkanDevice);
        
        void *mappedData;
        vkMapMemory(vulkanDevice.getLogicalDevice(), stagingBufferMemory, 0, size, 0, &mappedData);
        memcpy(mappedData, data, static_cast<size_t>(size));
        vkUnmapMemory(vulkanDevice.getLogicalDevice(), stagingBufferMemory);
        
        buffer = createBuffer(vulkanDevice.getLogicalDevice(), size, usage);
        bufferMemory = allocateBufferMemory(buffer, properties, vulkanDevice);
        
        copyBuffer(stagingBuffer, buffer, size, vulkanDevice, commandPool);
        
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
    
    bufferSize = static_cast<TUInt>(size);
}

void GVULKANBuffer::destroyBuffer(VkDevice device) {
    vkDestroyBuffer(device, buffer, nullptr);
    vkFreeMemory(device, bufferMemory, nullptr);
}

void GVULKANBuffer::refreshBuffer(const void *data, GVULKANDevice& vulkanDevice) {
    void *mappedData;
    vkMapMemory(vulkanDevice.getLogicalDevice(), bufferMemory, 0, bufferSize, 0, &mappedData);
    memcpy(mappedData, data, static_cast<size_t>(bufferSize));
    vkUnmapMemory(vulkanDevice.getLogicalDevice(), bufferMemory);
}

TUInt GVULKANBuffer::getBufferSize() {
    return bufferSize;
}

VkBuffer GVULKANBuffer::getBuffer() {
    return buffer;
}

#pragma mark - Routine -

VkBuffer GVULKANBuffer::createBuffer(VkDevice device, const VkDeviceSize size, const VkBufferUsageFlags usage) {
    VkBufferCreateInfo bufferInfo = { };
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    
    VkBuffer newBuffer;
    if (vkCreateBuffer(device, &bufferInfo, nullptr, &newBuffer) != VK_SUCCESS) {
        GLOG_ERROR("failed to create vertex buffer\n");
    }
    
    return newBuffer;
}

VkDeviceMemory GVULKANBuffer::allocateBufferMemory(VkBuffer originalBuffer, const VkMemoryPropertyFlags properties, GVULKANDevice& vulkanDevice) {
    VkMemoryRequirements memoryRequirements;
    vkGetBufferMemoryRequirements(vulkanDevice.getLogicalDevice(), originalBuffer, &memoryRequirements);
    
    VkMemoryAllocateInfo memoryAllocateInfo = { };
    memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memoryAllocateInfo.allocationSize = memoryRequirements.size;
    memoryAllocateInfo.memoryTypeIndex = GVULKANTools().findMemoryType(vulkanDevice.getPhysicalDevice(), memoryRequirements.memoryTypeBits, properties);
    
    VkDeviceMemory newBufferMemory;
    if (vkAllocateMemory(vulkanDevice.getLogicalDevice(), &memoryAllocateInfo, nullptr, &newBufferMemory) != VK_SUCCESS) {
        GLOG_ERROR("failed to allocate vertex buffer memory\n");
    }
    vkBindBufferMemory(vulkanDevice.getLogicalDevice(), originalBuffer, newBufferMemory, 0);
    
    return newBufferMemory;
}

void GVULKANBuffer::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, const VkDeviceSize size, GVULKANDevice& vulkanDevice, VkCommandPool commandPool) {
    VkCommandBuffer commandBuffer = allocateBufferCommand(srcBuffer, dstBuffer, size, commandPool, vulkanDevice.getLogicalDevice());
    
    VkCommandBufferBeginInfo beginInfo = { };
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    vkBeginCommandBuffer(commandBuffer, &beginInfo);
        VkBufferCopy copyRegion{};
        copyRegion.srcOffset = 0; // Optional
        copyRegion.dstOffset = 0; // Optional
        copyRegion.size = size;
        vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;
    vkQueueSubmit(vulkanDevice.getGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(vulkanDevice.getGraphicsQueue());
    
    vkFreeCommandBuffers(vulkanDevice.getLogicalDevice(), commandPool, 1, &commandBuffer);
}

VkCommandBuffer GVULKANBuffer::allocateBufferCommand(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkCommandPool commandPool, VkDevice device) {
    VkCommandBufferAllocateInfo allocInfo = { };
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;
    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

    return commandBuffer;
}

}
