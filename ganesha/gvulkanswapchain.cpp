#include "gvulkanswapchain.h"
#include "gvulkantools.h"

namespace spcGaneshaEngine {

GVULKANSwapChain::GVULKANSwapChain() : swapChain(VK_NULL_HANDLE), imageFormat(VK_FORMAT_UNDEFINED), extent({800, 600}), renderPass(VK_NULL_HANDLE), depthImage("") {
}

GVULKANSwapChain::~GVULKANSwapChain() {
}

void GVULKANSwapChain::createSwapChain(VkSurfaceKHR& surface, GVULKANDevice& vulkanDevice) {
    createSwapChain(extent, true, surface, vulkanDevice);
}

void GVULKANSwapChain::createSwapChain(const TUInt screenWidth, const TUInt screenHeight, GVULKANDevice& vulkanDevice, VkSurfaceKHR& surface) {
    createSwapChain({screenWidth, screenHeight}, true, surface, vulkanDevice);
}

void GVULKANSwapChain::destroySwapChain(GVULKANDevice& vulkanDevice) {
    if (swapChain == VK_NULL_HANDLE) {
        return;
    }
    
    destroySwapChainAndDependency(vulkanDevice);
    destroyRenderPass(vulkanDevice.getLogicalDevice());
}

void GVULKANSwapChain::updateScreenSize(const TUInt screenWidth, const TUInt screenHeight, GVULKANDevice& vulkanDevice, VkSurfaceKHR& surface) {
    vkDeviceWaitIdle(vulkanDevice.getLogicalDevice());
    
    destroySwapChainAndDependency(vulkanDevice);
    createSwapChain({screenWidth, screenHeight}, false, surface, vulkanDevice);
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

void GVULKANSwapChain::createSwapChain(const VkExtent2D& initialExtent, const TBool shouldCreateRenderPass, VkSurfaceKHR& surface, GVULKANDevice& vulkanDevice) {
    SwapChainSupportDetails supportDetails = vulkanDevice.querySwapChainSupport(surface);
    
    swapChain = createNewSwapChain(initialExtent, supportDetails, vulkanDevice, surface);
    imageFormat = selectSwapSurfaceFormat(supportDetails.formats).format;
    extent = selectSwapExtent(supportDetails.surfaceCapabilities, screenWidth, screenHeight);

    if (shouldCreateRenderPass) {
        renderPass = createRenderPass(imageFormat, vulkanDevice);
    }

    depthImage.createImage(extent,
                           vulkanDevice.findDepthImageFormat(),
                           VK_IMAGE_ASPECT_DEPTH_BIT,
                           VK_IMAGE_TILING_OPTIMAL,
                           VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                           vulkanDevice);
    imagesArray = ejectImagesArray(vulkanDevice.getLogicalDevice(), swapChain);
    imageViewsArray = createImageViews(vulkanDevice.getLogicalDevice(), imagesArray);
    framebuffersArray = createFramebuffers(vulkanDevice.getLogicalDevice(), imageViewsArray, depthImage.getImageView(), renderPass, extent);
}

VkSwapchainKHR GVULKANSwapChain::createNewSwapChain(const VkExtent2D& initialExtent, const SwapChainSupportDetails& supportDetails, GVULKANDevice& vulkanDevice, VkSurfaceKHR& surface) {
    VkSurfaceFormatKHR surfaceFormat = selectSwapSurfaceFormat(supportDetails.formats);
    VkExtent2D newExtent = selectSwapExtent(supportDetails.surfaceCapabilities, initialExtent);
    
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
        GLOG_ERROR("failed to create swap chain\n");
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

void GVULKANSwapChain::destroySwapChainAndDependency(GVULKANDevice& vulkanDevice) {
    for (auto framebuffer : framebuffersArray) {
        vkDestroyFramebuffer(vulkanDevice.getLogicalDevice(), framebuffer, nullptr);
    }
    framebuffersArray.clear();

    for (auto imageView : imageViewsArray) {
        vkDestroyImageView(vulkanDevice.getLogicalDevice(), imageView, nullptr);
    }
    imageViewsArray.clear();
    imagesArray.clear();
    
    depthImage.destroyImage(vulkanDevice.getLogicalDevice());
    depthImage = GVULKANImage("");
    
    vkDestroySwapchainKHR(vulkanDevice.getLogicalDevice(), swapChain, nullptr);
    swapChain = VK_NULL_HANDLE;
    extent = {0, 0};
    imageFormat = VK_FORMAT_UNDEFINED;
}

void GVULKANSwapChain::destroyRenderPass(VkDevice device) {
    vkDestroyRenderPass(device, renderPass, nullptr);
    renderPass = VK_NULL_HANDLE;
}

VkRenderPass GVULKANSwapChain::createRenderPass(VkFormat format, GVULKANDevice& vulkanDevice) {
    VkAttachmentDescription colorAttachment = { };
    colorAttachment.format = format;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    
    VkAttachmentDescription depthAttachment = { };
    depthAttachment.format = vulkanDevice.findDepthImageFormat();
    depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    
    VkAttachmentReference colorAttachmentRef = { };
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthAttachmentRef{};
    depthAttachmentRef.attachment = 1;
    depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass = { };
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;
    subpass.pDepthStencilAttachment = &depthAttachmentRef;
    
    VkSubpassDependency dependency = { };
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

    std::array<VkAttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };
    VkRenderPassCreateInfo renderPassInfo = { };
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = attachments.size();
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount =  1;
    renderPassInfo.pDependencies = &dependency;
    
    VkRenderPass newRenderPass;
    if (vkCreateRenderPass(vulkanDevice.getLogicalDevice(), &renderPassInfo, nullptr, &newRenderPass) != VK_SUCCESS) {
        GLOG_ERROR("failed to create render pass\n");
    }
    
    return newRenderPass;
}

std::vector<VkFramebuffer> GVULKANSwapChain::createFramebuffers(VkDevice device, const std::vector<VkImageView>& useImagesViewArray, VkImageView depthImageView, VkRenderPass useRenderPass, const VkExtent2D& useExtent) {
    std::vector<VkFramebuffer> newFramebuffersArray;
    
    newFramebuffersArray.resize(useImagesViewArray.size());
    for (size_t i = 0; i < useImagesViewArray.size(); i++) {
        std::array<VkImageView, 2> attachments = {
            useImagesViewArray[i],
            depthImageView
        };
        
        VkFramebufferCreateInfo framebufferInfo = { };
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = useRenderPass;
        framebufferInfo.attachmentCount = attachments.size();
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = useExtent.width;
        framebufferInfo.height = useExtent.height;
        framebufferInfo.layers = 1;
        
        if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &newFramebuffersArray[i]) != VK_SUCCESS) {
            GLOG_ERROR("failed to create framebuffer with index %zu\n", i);
        }
    }
    
    return newFramebuffersArray;
}

std::vector<VkImageView> GVULKANSwapChain::createImageViews(VkDevice logicalDevice, std::vector<VkImage>& swapChainImagesArray) {
    std::vector<VkImageView> newImageViewsArray;

    GVULKANTools tools;
    newImageViewsArray.resize(swapChainImagesArray.size());
    for (size_t i = 0; i < swapChainImagesArray.size(); i++) {
        newImageViewsArray[i] = tools.createImageView(swapChainImagesArray[i], imageFormat, VK_IMAGE_ASPECT_COLOR_BIT, logicalDevice);
    }
    
    return newImageViewsArray;
}

VkExtent2D GVULKANSwapChain::selectSwapExtent(const VkSurfaceCapabilitiesKHR& surfaceCapabilities, const VkExtent2D& initialExtent) {
    VkExtent2D actualExtent = initialExtent;
    
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
