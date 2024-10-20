#include "gmaterialsservice.h"
#include "gtga.h"
#include "gvulkantools.h"
#include "gvulkanbuffer.h"

namespace spcGaneshaEngine {

GMaterialsService::GMaterialsService() {
    
}

GMaterialsService::~GMaterialsService() {
}

#pragma mark - GMaterialsServiceProtocol -

void GMaterialsService::init() {
}

void GMaterialsService::destroy() {
}

std::shared_ptr<GVULKANImage> GMaterialsService::createMaterial(const std::string &imageFilePath) {
    std::shared_ptr<GVULKANImage> newMaterial =  std::make_shared<GVULKANImage>(imageFilePath);
    return newMaterial;
}

void GMaterialsService::destroyMaterial(std::shared_ptr<GVULKANImage> material, GVULKANDevice& vulkanDevice) {
    VkDevice device = vulkanDevice.getLogicalDevice();
    
    vkDestroySampler(device, material->sampler, nullptr);
    vkDestroyImageView(device, material->imageView, nullptr);
    vkDestroyImage(device, material->image, nullptr);
    vkFreeMemory(device, material->imageMemory, nullptr);
}

void GMaterialsService::deployMaterial(std::shared_ptr<GVULKANImage> material, GCommandServiceProtocol *commandService, GVULKANDevice& vulkanDevice) {
    deployMaterial(material,
                   VK_FORMAT_R8G8B8A8_SRGB,
                   VK_IMAGE_ASPECT_COLOR_BIT,
                   VK_IMAGE_TILING_OPTIMAL,
                   VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
                   commandService,
                   vulkanDevice);
}

bool GMaterialsService::isDeployed(std::shared_ptr<GVULKANImage> image) {
    if (image->image == VK_NULL_HANDLE) {
        return false;
    }
    
    return true;
}

void GMaterialsService::deployMaterial(std::shared_ptr<GVULKANImage> material,
                                    VkFormat format,
                                    VkImageAspectFlags aspectFlags,
                                    VkImageTiling tiling,
                                    VkImageUsageFlags usage,
                                    GCommandServiceProtocol *commandService,
                                    GVULKANDevice& vulkanDevice) {
    GTGA tgaImage(material->textureFileName);
    material->imageExtent = {tgaImage.getWidth(), tgaImage.getHeight()};
    
    VkImageCreateInfo imageInfo = {};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = material->imageExtent.width;
    imageInfo.extent.height = material->imageExtent.height;
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
    material->image = newImage;
    
    VkMemoryRequirements memoryRequirements;
    vkGetImageMemoryRequirements(vulkanDevice.getLogicalDevice(), material->image, &memoryRequirements);
    VkMemoryAllocateInfo allocInfo = { };
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memoryRequirements.size;
    allocInfo.memoryTypeIndex = vulkanDevice.findMemoryType(memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    if (vkAllocateMemory(vulkanDevice.getLogicalDevice(), &allocInfo, nullptr, &material->imageMemory) != VK_SUCCESS) {
        GLOG_ERROR("failed to allocate image memory!");
    }
    vkBindImageMemory(vulkanDevice.getLogicalDevice(), material->image, material->imageMemory, 0);
    
    GVULKANTools tools;
    material->imageView = tools.createImageView(material->image, format, aspectFlags, vulkanDevice.getLogicalDevice());
    material->sampler = createTextureSampler(vulkanDevice);
    
    deployMaterialData(material, tgaImage, commandService, vulkanDevice);
}

void GMaterialsService::deployMaterialData(std::shared_ptr<GVULKANImage> material,
                                           GTGA& tgaFile,
                                           GCommandServiceProtocol *commandService,
                                           GVULKANDevice& vulkanDevice) {
    GVULKANBuffer stagingBuffer;
    stagingBuffer.createBuffer(tgaFile.getImageData(),
                               tgaFile.getWidth() * tgaFile.getHeight() * tgaFile.getBytepp(),
                               VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                               VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                               false,
                               commandService,
                               vulkanDevice);
    
    transitionImageLayout(material->image,
                          VK_FORMAT_R8G8B8A8_SRGB,
                          VK_IMAGE_LAYOUT_UNDEFINED,
                          VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                          commandService);
    
    copyBufferToImage(stagingBuffer.getBuffer(), material->image, material->imageExtent, commandService);
    
    transitionImageLayout(material->image,
                          VK_FORMAT_R8G8B8A8_SRGB,
                          VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                          VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                          commandService);
    
    stagingBuffer.destroyBuffer(vulkanDevice.getLogicalDevice());
}

VkSampler GMaterialsService::createTextureSampler(GVULKANDevice& device) {
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

void GMaterialsService::copyBufferToImage(VkBuffer buffer, VkImage image, VkExtent2D extent, GCommandServiceProtocol *commandService) {
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

void GMaterialsService::transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, GCommandServiceProtocol *commandService) {
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
