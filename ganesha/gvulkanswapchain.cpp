#include "gvulkanswapchain.h"

namespace spcGaneshaEngine {

GVULKANSwapChain::GVULKANSwapChain(GLog& log) : log(log) {
}

GVULKANSwapChain::~GVULKANSwapChain() {
}

void GVULKANSwapChain::createSwapChain(const TUInt screenWidth, const TUInt screenHeight, GVULKANDevice& vulkanDevice, VkSurfaceKHR& surface) {
    createSwapChain(screenWidth, screenHeight, vulkanDevice, surface, false);
}

void GVULKANSwapChain::destroySwapChain(GVULKANDevice& device) {
    destroyExtentDependency(device.getLogicalDevice());
    destroyRenderPass(device.getLogicalDevice());
}

void GVULKANSwapChain::updateScreenSize(const TUInt screenWidth, const TUInt screenHeight, GVULKANDevice& vulkanDevice, VkSurfaceKHR& surface) {
    destroyExtentDependency(vulkanDevice.getLogicalDevice());
    createSwapChain(screenWidth, screenHeight, vulkanDevice, surface, true);
}

std::vector<VkImageView>& GVULKANSwapChain::getImageViewsArray() {
    return imageViewsArray;
}

VkExtent2D GVULKANSwapChain::getExtent() {
    return extent;
}

VkSwapchainKHR GVULKANSwapChain::getVulkanSwapChain() {
    return swapChain;
}

size_t GVULKANSwapChain::framebuffersNumber() {
    return framebuffersArray.size();
}

VkFormat GVULKANSwapChain::getImagesFormat() {
    return imageFormat;
}

VkRenderPass GVULKANSwapChain::getRenderPass() {
    return renderPass;
}

std::vector<VkFramebuffer>& GVULKANSwapChain::getFramebuffers() {
    return framebuffersArray;
}

#pragma mark - Routine -

void GVULKANSwapChain::createSwapChain(const TUInt screenWidth, const TUInt screenHeight, GVULKANDevice& vulkanDevice, VkSurfaceKHR& surface, const TBool recreateSwapChain) {
    SwapChainSupportDetails supportDetails = vulkanDevice.querySwapChainSupport(surface);
    
    swapChain = createNewSwapChain(screenWidth, screenHeight, supportDetails, vulkanDevice, surface);
    imageFormat = selectSwapSurfaceFormat(supportDetails.formats).format;
    extent = selectSwapExtent(supportDetails.surfaceCapabilities, screenWidth, screenHeight);

    if (!recreateSwapChain) {
        renderPass = createRenderPass(vulkanDevice.getLogicalDevice(), imageFormat);
    }
    imagesArray = ejectImagesArray(vulkanDevice.getLogicalDevice(), swapChain);
    imageViewsArray = createImageViews(vulkanDevice.getLogicalDevice(), imagesArray);
    framebuffersArray = createFramebuffers(vulkanDevice.getLogicalDevice(), imageViewsArray, renderPass, extent);
}

VkSwapchainKHR GVULKANSwapChain::createNewSwapChain(const TUInt screenWidth, const TUInt screenHeight, const SwapChainSupportDetails& supportDetails, GVULKANDevice& vulkanDevice, VkSurfaceKHR& surface) {
    VkSurfaceFormatKHR surfaceFormat = selectSwapSurfaceFormat(supportDetails.formats);
    VkExtent2D newExtent = selectSwapExtent(supportDetails.surfaceCapabilities, screenWidth, screenHeight);
    
    TUInt count = supportDetails.surfaceCapabilities.minImageCount + 1;
    if (supportDetails.surfaceCapabilities.maxImageCount > 0 && count > supportDetails.surfaceCapabilities.maxImageCount) {
        count = supportDetails.surfaceCapabilities.maxImageCount;
    }
    
    VkSwapchainCreateInfoKHR swapChainInfo = { };
    swapChainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapChainInfo.surface = surface;
    swapChainInfo.minImageCount = count;
    swapChainInfo.imageFormat = surfaceFormat.format;
    swapChainInfo.imageColorSpace = surfaceFormat.colorSpace;
    swapChainInfo.imageExtent = newExtent;
    swapChainInfo.imageArrayLayers = 1;
    swapChainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    
    std::vector<TUInt> queueFamilyIndexArray;
    if (vulkanDevice.presentationIsEqualToGraphics()) {
        swapChainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        swapChainInfo.queueFamilyIndexCount = 0;
        swapChainInfo.pQueueFamilyIndices = nullptr;
    }
    else {
        queueFamilyIndexArray = vulkanDevice.getQueuesIndexesArray();
        swapChainInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swapChainInfo.queueFamilyIndexCount = static_cast<TUInt>(queueFamilyIndexArray.size());
        swapChainInfo.pQueueFamilyIndices = queueFamilyIndexArray.data();
    }
    swapChainInfo.preTransform = supportDetails.surfaceCapabilities.currentTransform;
    swapChainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapChainInfo.presentMode = selectSwapPresentMode(supportDetails.presentModes);
    swapChainInfo.clipped = VK_TRUE;
    
    VkSwapchainKHR newSwapChain;
    if (vkCreateSwapchainKHR(vulkanDevice.getLogicalDevice(), &swapChainInfo, nullptr, &newSwapChain) != VK_SUCCESS) {
        log.error("failed to create swap chain\n");
    }
    
    return newSwapChain;
}

std::vector<VkImage> GVULKANSwapChain::ejectImagesArray(VkDevice device, const VkSwapchainKHR& swapChainSource) {
    std::vector<VkImage> newImagesArray;
    
    TUInt count;
    vkGetSwapchainImagesKHR(device, swapChainSource, &count, nullptr);
    newImagesArray.resize(count);
    vkGetSwapchainImagesKHR(device, swapChainSource, &count, newImagesArray.data());

    return newImagesArray;
}

void GVULKANSwapChain::destroyExtentDependency(VkDevice device) {
    vkDeviceWaitIdle(device);
    for (auto framebuffer : framebuffersArray) {
        vkDestroyFramebuffer(device, framebuffer, nullptr);
    }

    for (auto imageView : imageViewsArray) {
        vkDestroyImageView(device, imageView, nullptr);
    }
    
    vkDestroySwapchainKHR(device, swapChain, nullptr);
}

void GVULKANSwapChain::destroyRenderPass(VkDevice device) {
    vkDestroyRenderPass(device, renderPass, nullptr);
}

VkRenderPass GVULKANSwapChain::createRenderPass(VkDevice device, VkFormat format) {
    VkAttachmentDescription colorAttachment = { };
    colorAttachment.format = format;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    
    VkAttachmentReference colorAttachmentRef = { };
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    VkSubpassDescription subpass = { };
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;
    
    VkSubpassDependency dependency = { };
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    
    VkRenderPassCreateInfo renderPassInfo = { };
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount =  1;
    renderPassInfo.pDependencies = &dependency;
    
    VkRenderPass newRenderPass;
    if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &newRenderPass) != VK_SUCCESS) {
        log.error("failed to create render pass\n");
    }
    
    return newRenderPass;
}

std::vector<VkFramebuffer> GVULKANSwapChain::createFramebuffers(VkDevice device, const std::vector<VkImageView>& useImagesViewArray, VkRenderPass useRenderPass, const VkExtent2D& useExtent) {
    std::vector<VkFramebuffer> newFramebuffersArray;
    
    newFramebuffersArray.resize(useImagesViewArray.size());
    for (size_t i = 0; i < useImagesViewArray.size(); i++) {
        VkImageView attachments[] = {
            useImagesViewArray[i]
        };
        
        VkFramebufferCreateInfo framebufferInfo = { };
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = useRenderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = useExtent.width;
        framebufferInfo.height = useExtent.height;
        framebufferInfo.layers = 1;
        
        if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &newFramebuffersArray[i]) != VK_SUCCESS) {
            log.error("failed to create framebuffer with index %zu\n", i);
        }
    }
    
    return newFramebuffersArray;
}

std::vector<VkImageView> GVULKANSwapChain::createImageViews(VkDevice logicalDevice, std::vector<VkImage>& swapChainImagesArray) {
    std::vector<VkImageView> newImageViewsArray;
    
    newImageViewsArray.resize(swapChainImagesArray.size());
    for (size_t i = 0; i < swapChainImagesArray.size(); i++) {
        VkImageViewCreateInfo imageViewInfo = { };
        
        imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imageViewInfo.image = swapChainImagesArray[i];
        imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        imageViewInfo.format = imageFormat;
        imageViewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        imageViewInfo.subresourceRange.baseMipLevel = 0;
        imageViewInfo.subresourceRange.levelCount = 1;
        imageViewInfo.subresourceRange.baseArrayLayer = 0;
        imageViewInfo.subresourceRange.layerCount = 1;
        
        if (vkCreateImageView(logicalDevice, &imageViewInfo, nullptr, &newImageViewsArray[i]) != VK_SUCCESS) {
            log.error("failed to create image view\n");
        }
    }
    
    return newImageViewsArray;
}

VkExtent2D GVULKANSwapChain::selectSwapExtent(const VkSurfaceCapabilitiesKHR& surfaceCapabilities, const TUInt screenWidth, const TUInt screenHeight) {
    VkExtent2D actualExtent = {screenWidth, screenHeight};
    
    if (surfaceCapabilities.currentExtent.width != std::numeric_limits<TUInt>::max()) {
        return surfaceCapabilities.currentExtent;
    } else {
        actualExtent.width = std::max(surfaceCapabilities.minImageExtent.width, std::min(surfaceCapabilities.maxImageExtent.width, actualExtent.width));
        actualExtent.height = std::max(surfaceCapabilities.minImageExtent.height, std::min(surfaceCapabilities.maxImageExtent.height, actualExtent.height));
    }
    
    return actualExtent;
}

VkSurfaceFormatKHR GVULKANSwapChain::selectSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
    for (const auto& availableFormat : availableFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
            availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }
    
    return availableFormats[0];
}

VkPresentModeKHR GVULKANSwapChain::selectSwapPresentMode(const std::vector<VkPresentModeKHR>& presentModesArray) {
    for (const auto& mode : presentModesArray) {
        if (mode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return mode;
        }
    }
    
    return VK_PRESENT_MODE_FIFO_KHR;
}

}
