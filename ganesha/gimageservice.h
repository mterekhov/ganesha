#ifndef SPCGANESHAENGINE_GIMAGE_SERVICE_H
#define SPCGANESHAENGINE_GIMAGE_SERVICE_H

#include <string>
#include <map>
#include <vulkan/vulkan.h>

#include "gvulkanimage.h"
#include "gvulkandevice.h"
#include "gcommandservice.h"
#include "gtga.h"

namespace spcGaneshaEngine {

typedef std::map<std::string, std::shared_ptr<GVULKANImage>> TMaterialsMap;

class GImageServiceProtocol {
public:
    virtual void init() = 0;
    virtual void destroy() = 0;
    virtual std::shared_ptr<GVULKANImage> createImage(const std::string& imageFilePath) = 0;
    virtual void destroyImage(std::shared_ptr<GVULKANImage> vulkanImage, GVULKANDevice& vulkanDevice) = 0;
    virtual void deployImage(std::shared_ptr<GVULKANImage> vulkanImage, GCommandServiceProtocol *commandService, GVULKANDevice& vulkanDevice) = 0;
    virtual bool isDeployed(std::shared_ptr<GVULKANImage> image) = 0;

};

class GImageService: public GImageServiceProtocol {
public:
    GImageService();
    virtual ~GImageService();
    
    void init() override;
    void destroy() override;
    std::shared_ptr<GVULKANImage> createImage(const std::string& imageFilePath) override;
    void destroyImage(std::shared_ptr<GVULKANImage> vulkanImage, GVULKANDevice& vulkanDevice) override;
    void deployImage(std::shared_ptr<GVULKANImage> vulkanImage, GCommandServiceProtocol *commandService, GVULKANDevice& vulkanDevice) override;
    bool isDeployed(std::shared_ptr<GVULKANImage> vulkanImage) override;

private:
    void deployImage(std::shared_ptr<GVULKANImage> vulkanImage,
                        VkFormat format,
                        VkImageAspectFlags aspectFlags,
                        VkImageTiling tiling,
                        VkImageUsageFlags usage,
                        GCommandServiceProtocol *commandService,
                        GVULKANDevice& vulkanDevice);
    void deployImageData(std::shared_ptr<GVULKANImage> vulkanImage,
                            GTGA& tgaFile,
                            GCommandServiceProtocol *commandService,
                            GVULKANDevice& vulkanDevice);
    VkSampler createTextureSampler(GVULKANDevice& device);
    void copyBufferToImage(VkBuffer buffer, VkImage image, VkExtent2D extent, GCommandServiceProtocol *commandService);
    void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, GCommandServiceProtocol *commandService);


};

};  //  spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GIMAGE_SERVICE_H
