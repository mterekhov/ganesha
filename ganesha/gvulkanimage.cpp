#include "gvulkanimage.h"
#include "gtga.h"
#include "gvulkantools.h"
#include "gvulkanbuffer.h"

namespace spcGaneshaEngine {

GVULKANImage::GVULKANImage(GLog& log) : log(log) {
    
}

GVULKANImage::~GVULKANImage() {
    
}

void GVULKANImage::createImage(const std::string filePath,
                               VkFormat format, 
                               VkImageTiling tiling,
                               VkImageUsageFlags usage,
                               GVULKANDevice& vulkanDevice,
                               GVULKANCommands& vulkanCommands) {
    GTGA tgaFile(filePath);
    GVULKANTools tools;
    
    VkImageCreateInfo imageInfo = {};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = tgaFile.getWidth();
    imageInfo.extent.height = tgaFile.getHeight();
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = format;
    imageInfo.tiling = tiling;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = usage;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    if (vkCreateImage(vulkanDevice.getLogicalDevice(), &imageInfo, nullptr, &image) != VK_SUCCESS) {
        log.error("can not create image %s\n", filePath.c_str());
    }
    
    VkMemoryRequirements memoryRequirements;
    vkGetImageMemoryRequirements(vulkanDevice.getLogicalDevice(), image, &memoryRequirements);
    VkMemoryAllocateInfo allocInfo = { };
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memoryRequirements.size;
    allocInfo.memoryTypeIndex = tools.findMemoryType(vulkanDevice.getPhysicalDevice(), memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    if (vkAllocateMemory(vulkanDevice.getLogicalDevice(), &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
        log.error("failed to allocate image memory!");
    }
    vkBindImageMemory(vulkanDevice.getLogicalDevice(), image, imageMemory, 0);
    
    GVULKANBuffer stagingBuffer(log);
    stagingBuffer.createBuffer(tgaFile.getImageData(),
                               tgaFile.getWidth() * tgaFile.getHeight() * tgaFile.getBytepp(),
                               VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                               VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                               false,
                               vulkanDevice,
                               vulkanCommands);

    VkCommandBuffer tmpCommand = vulkanCommands.transitionImageLayout(image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, vulkanDevice.getLogicalDevice());
    vulkanCommands.submitCommand(tmpCommand, vulkanDevice);

    tmpCommand = vulkanCommands.copyBufferToImage(stagingBuffer.getBuffer(), image, tgaFile.getWidth(), tgaFile.getHeight(), vulkanDevice.getLogicalDevice());
    vulkanCommands.submitCommand(tmpCommand, vulkanDevice);
    
    tmpCommand = vulkanCommands.transitionImageLayout(image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, vulkanDevice.getLogicalDevice());
    vulkanCommands.submitCommand(tmpCommand, vulkanDevice);
    
    stagingBuffer.destroyBuffer(vulkanDevice);
    
    imageView = tools.createImageView(image, VK_FORMAT_R8G8B8A8_SRGB, vulkanDevice.getLogicalDevice());
    sampler = createTextureSampler(vulkanDevice);
}

VkSampler GVULKANImage::createTextureSampler(GVULKANDevice& device) {
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
        log.error("can not create sampler for image\n");
    }
    
    return newSampler;
}

void GVULKANImage::destroyImage(GVULKANDevice& vulkanDevice) {
    vkDestroySampler(vulkanDevice.getLogicalDevice(), sampler, nullptr);
    vkDestroyImageView(vulkanDevice.getLogicalDevice(), imageView, nullptr);
    vkDestroyImage(vulkanDevice.getLogicalDevice(), image, nullptr);
    vkFreeMemory(vulkanDevice.getLogicalDevice(), imageMemory, nullptr);
}

}
