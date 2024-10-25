#include "gimageservice.h"
#include "gvulkantools.h"
#include "gvulkanbuffer.h"
#include "glog.h"

namespace spcGaneshaEngine {

GImageService::GImageService() {
    
}

GImageService::~GImageService() {
}

#pragma mark - GImageServiceProtocol -

void GImageService::init() {
}

void GImageService::destroy() {
}

std::shared_ptr<GVULKANImage> GImageService::createImage(const std::string &imageName,
                                                         VkExtent2D extent,
                                                         VkFormat format,
                                                         VkImageAspectFlags aspectFlags,
                                                         VkImageTiling tiling,
                                                         VkImageUsageFlags usage,
                                                         std::shared_ptr<GCommandServiceProtocol> commandService,
                                                         GVULKANDevice& vulkanDevice) {
    std::shared_ptr<GVULKANImage> newVulkanImage =  std::make_shared<GVULKANImage>(imageName);
    
    VkImageCreateInfo imageInfo = {};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = extent.width;
    imageInfo.extent.height = extent.height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = format;
    imageInfo.tiling = tiling;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = usage;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    VkImage newImage;
    if (vkCreateImage(vulkanDevice.getLogicalDevice(), &imageInfo, nullptr, &newImage) != VK_SUCCESS) {
        GLOG_ERROR("can not create image\n");
    }
    newVulkanImage->image = newImage;
    
    VkMemoryRequirements memoryRequirements;
    vkGetImageMemoryRequirements(vulkanDevice.getLogicalDevice(), newVulkanImage->image, &memoryRequirements);
    VkMemoryAllocateInfo allocInfo = { };
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memoryRequirements.size;
    allocInfo.memoryTypeIndex = vulkanDevice.findMemoryType(memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    if (vkAllocateMemory(vulkanDevice.getLogicalDevice(), &allocInfo, nullptr, &newVulkanImage->imageMemory) != VK_SUCCESS) {
        GLOG_ERROR("failed to allocate image memory!");
    }
    vkBindImageMemory(vulkanDevice.getLogicalDevice(), newVulkanImage->image, newVulkanImage->imageMemory, 0);
    
    GVULKANTools tools;
    newVulkanImage->imageView = tools.createImageView(newVulkanImage->image, format, aspectFlags, vulkanDevice.getLogicalDevice());
    newVulkanImage->sampler = createTextureSampler(vulkanDevice);

    return newVulkanImage;
}

void GImageService::destroyImage(std::shared_ptr<GVULKANImage> vulkanImage, GVULKANDevice& vulkanDevice) {
    VkDevice device = vulkanDevice.getLogicalDevice();
    
    vkDestroySampler(device, vulkanImage->sampler, nullptr);
    vkDestroyImageView(device, vulkanImage->imageView, nullptr);
    vkDestroyImage(device, vulkanImage->image, nullptr);
    vkFreeMemory(device, vulkanImage->imageMemory, nullptr);
}

//void GImageService::deployImage(std::shared_ptr<GVULKANImage> vulkanImage, GCommandServiceProtocol *commandService, GVULKANDevice& vulkanDevice) {
//    deployImage(vulkanImage,
//                   VK_FORMAT_R8G8B8A8_SRGB,
//                   VK_IMAGE_ASPECT_COLOR_BIT,
//                   VK_IMAGE_TILING_OPTIMAL,
//                   VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
//                   commandService,
//                   vulkanDevice);
//}

void GImageService::deployImage(std::shared_ptr<GVULKANImage> vulkanImage,
                                GTGA& tga,
                                std::shared_ptr<GCommandServiceProtocol> commandService,
                                GVULKANDevice& vulkanDevice) {
    deployImageData(vulkanImage, tga, commandService, vulkanDevice);
}

bool GImageService::isDeployed(std::shared_ptr<GVULKANImage> vulkanImage) {
    if (vulkanImage->image == VK_NULL_HANDLE) {
        return false;
    }
    
    return true;
}

void GImageService::deployImageData(std::shared_ptr<GVULKANImage> vulkanImage,
                                    GTGA& tga,
                                    std::shared_ptr<GCommandServiceProtocol> commandService,
                                    GVULKANDevice& vulkanDevice) {
    GVULKANBuffer stagingBuffer;
    stagingBuffer.createBuffer(tga.getImageData(),
                               tga.getWidth() * tga.getHeight() * tga.getBytepp(),
                               VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                               VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                               false,
                               commandService,
                               vulkanDevice);
    
    transitionImageLayout(vulkanImage->image,
                          VK_FORMAT_R8G8B8A8_SRGB,
                          VK_IMAGE_LAYOUT_UNDEFINED,
                          VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                          commandService);
    
    copyBufferToImage(stagingBuffer.getBuffer(), vulkanImage->image, vulkanImage->imageExtent, commandService);
    
    transitionImageLayout(vulkanImage->image,
                          VK_FORMAT_R8G8B8A8_SRGB,
                          VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                          VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                          commandService);
    
    stagingBuffer.destroyBuffer(vulkanDevice.getLogicalDevice());
}

VkSampler GImageService::createTextureSampler(GVULKANDevice& device) {
    VkPhysicalDeviceProperties properties = device.getPhysicalDeviceProperties();
    
    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.anisotropyEnable = VK_TRUE;
    samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    
    VkSampler newSampler;
    if (vkCreateSampler(device.getLogicalDevice(), &samplerInfo, nullptr, &newSampler) != VK_SUCCESS) {
        GLOG_ERROR("can not create sampler for image\n");
    }
    
    return newSampler;
}

void GImageService::copyBufferToImage(VkBuffer buffer, VkImage image, VkExtent2D extent, std::shared_ptr<GCommandServiceProtocol> commandService) {
    VkCommandBuffer commandBuffer = commandService->allocateCommandBuffer();
    
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    vkBeginCommandBuffer(commandBuffer, &beginInfo);
    VkBufferImageCopy region = {};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;
    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;
    region.imageOffset = { 0, 0, 0 };
    region.imageExtent = { extent.width, extent.height, 1 };
    vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
    vkEndCommandBuffer(commandBuffer);
    
    commandService->submitCommandBuffer({ commandBuffer });
}

void GImageService::transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, std::shared_ptr<GCommandServiceProtocol> commandService) {
    VkCommandBuffer commandBuffer = commandService->allocateCommandBuffer();
    
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    vkBeginCommandBuffer(commandBuffer, &beginInfo);
    VkImageMemoryBarrier barrier = { };
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;
    VkPipelineStageFlags sourceStage = 0;
    VkPipelineStageFlags destinationStage = 0;
    if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        
        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        
        sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);
    vkEndCommandBuffer(commandBuffer);
    
    commandService->submitCommandBuffer({ commandBuffer });
}

};
