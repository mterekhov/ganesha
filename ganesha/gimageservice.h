#ifndef SPCGANESHAENGINE_GIMAGE_SERVICE_H
#define SPCGANESHAENGINE_GIMAGE_SERVICE_H

#include <string>
#include <vulkan/vulkan.h>

#include "gvulkanimage.h"
#include "gvulkandevice.h"
#include "gcommandservice.h"
#include "gtga.h"

namespace spcGaneshaEngine {

class GImageServiceProtocol {
public:
    virtual void init() = 0;
    virtual void destroy() = 0;
    virtual std::shared_ptr<GVULKANImage> createImage(const std::string &imageName,
                                                      VkExtent2D extent,
                                                      VkFormat format,
                                                      VkImageAspectFlags aspectFlags,
                                                      VkImageTiling tiling,
                                                      VkImageUsageFlags usage,
                                                      std::shared_ptr<GCommandServiceProtocol> commandService,
                                                      GVULKANDevice& vulkanDevice) = 0;
    virtual void destroyImage(std::shared_ptr<GVULKANImage> vulkanImage, GVULKANDevice& vulkanDevice) = 0;
    virtual void deployImage(std::shared_ptr<GVULKANImage> vulkanImage,
                             GTGA& tga,
                             std::shared_ptr<GCommandServiceProtocol> commandService,
                             GVULKANDevice& vulkanDevice) = 0;
    virtual bool isDeployed(std::shared_ptr<GVULKANImage> image) = 0;

};

class GImageService: public GImageServiceProtocol {
public:
    GImageService();
    virtual ~GImageService();
    
    void init() override;
    void destroy() override;
    std::shared_ptr<GVULKANImage> createImage(const std::string &imageName,
                                              VkExtent2D extent,
                                              VkFormat format,
                                              VkImageAspectFlags aspectFlags,
                                              VkImageTiling tiling,
                                              VkImageUsageFlags usage,
                                              std::shared_ptr<GCommandServiceProtocol> commandService,
                                              GVULKANDevice& vulkanDevice) override;
    void destroyImage(std::shared_ptr<GVULKANImage> vulkanImage, GVULKANDevice& vulkanDevice) override;
    void deployImage(std::shared_ptr<GVULKANImage> vulkanImage,
                     GTGA& tga,
                     std::shared_ptr<GCommandServiceProtocol> commandService,
                     GVULKANDevice& vulkanDevice) override;
    bool isDeployed(std::shared_ptr<GVULKANImage> vulkanImage) override;

private:
    void deployImageData(std::shared_ptr<GVULKANImage> vulkanImage,
                         GTGA& tgaFile,
                         std::shared_ptr<GCommandServiceProtocol> commandService,
                         GVULKANDevice& vulkanDevice);
    VkSampler createTextureSampler(GVULKANDevice& device);
    void copyBufferToImage(VkBuffer buffer, VkImage image, VkExtent2D extent, std::shared_ptr<GCommandServiceProtocol> commandService);
    void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, std::shared_ptr<GCommandServiceProtocol> commandService);


};

};  //  spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GIMAGE_SERVICE_H
