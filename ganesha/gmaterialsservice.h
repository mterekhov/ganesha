#ifndef SPCGANESHAENGINE_GMATERIALSSERVICE_H
#define SPCGANESHAENGINE_GMATERIALSSERVICE_H

#include <string>
#include <map>
#include <vulkan/vulkan.h>

#include "gvulkanimage.h"
#include "gvulkandevice.h"
#include "gcommandservice.h"
#include "gtga.h"

namespace spcGaneshaEngine {

typedef std::map<std::string, std::shared_ptr<GVULKANImage>> TMaterialsMap;

class GMaterialsServiceProtocol {
public:
    virtual void init() = 0;
    virtual void destroy() = 0;
    virtual std::shared_ptr<GVULKANImage> createMaterial(const std::string& imageFilePath) = 0;
    virtual void destroyMaterial(std::shared_ptr<GVULKANImage> material, GVULKANDevice& vulkanDevice) = 0;
    virtual void deployMaterial(std::shared_ptr<GVULKANImage> material, GCommandServiceProtocol *commandService, GVULKANDevice& vulkanDevice) = 0;
    virtual bool isDeployed(std::shared_ptr<GVULKANImage> image) = 0;

};

class GMaterialsService: public GMaterialsServiceProtocol {
public:
    GMaterialsService();
    virtual ~GMaterialsService();
    
    void init() override;
    void destroy() override;
    std::shared_ptr<GVULKANImage> createMaterial(const std::string& imageFilePath) override;
    void destroyMaterial(std::shared_ptr<GVULKANImage> material, GVULKANDevice& vulkanDevice) override;
    void deployMaterial(std::shared_ptr<GVULKANImage> material, GCommandServiceProtocol *commandService, GVULKANDevice& vulkanDevice) override;
    bool isDeployed(std::shared_ptr<GVULKANImage> image) override;

private:
    void deployMaterial(std::shared_ptr<GVULKANImage> material,
                        VkFormat format,
                        VkImageAspectFlags aspectFlags,
                        VkImageTiling tiling,
                        VkImageUsageFlags usage,
                        GCommandServiceProtocol *commandService,
                        GVULKANDevice& vulkanDevice);
    void deployMaterialData(std::shared_ptr<GVULKANImage> material,
                            GTGA& tgaFile,
                            GCommandServiceProtocol *commandService,
                            GVULKANDevice& vulkanDevice);
    VkSampler createTextureSampler(GVULKANDevice& device);
    void copyBufferToImage(VkBuffer buffer, VkImage image, VkExtent2D extent, GCommandServiceProtocol *commandService);
    void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, GCommandServiceProtocol *commandService);


};

};  //  spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GMATERIALSSERVICE_H
