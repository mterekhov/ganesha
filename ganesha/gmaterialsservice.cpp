#include "gmaterialsservice.h"
#include "gtga.h"

namespace spcGaneshaEngine {

GMaterialsService::GMaterialsService(GCommandServiceProtocol *commandService, GVULKANDevice& vulkanDevice) : commandService(commandService), vulkanDevice(vulkanDevice) {
    
}

GMaterialsService::~GMaterialsService() {
}

#pragma mark - GMaterialsServiceProtocol -

void GMaterialsService::init() {
}

void GMaterialsService::destroy() {
    VkDevice device = vulkanDevice.getLogicalDevice();
    materialsMap.clear();
}

std::shared_ptr<GVULKANImage> GMaterialsService::createMaterial(const std::string &imageFilePath) {
    GTGA tgaFile(imageFilePath);
    
    std::shared_ptr<GVULKANImage> newImage =  std::make_shared<GVULKANImage>();
    newImage->createImage({ tgaFile.getWidth(), tgaFile.getHeight() },
                         VK_FORMAT_R8G8B8A8_SRGB,
                         VK_IMAGE_ASPECT_COLOR_BIT,
                         VK_IMAGE_TILING_OPTIMAL,
                          VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
                         vulkanDevice);
    newImage->deployData(tgaFile,
                        vulkanDevice,
                        commandService);
    
    materialsMap[imageFilePath] = newImage;
    
    return materialsMap[imageFilePath];
}

std::shared_ptr<GVULKANImage> GMaterialsService::findMaterial(const std::string& imageFilePath) {
    return materialsMap[imageFilePath];
}

std::map<std::string, std::shared_ptr<GVULKANImage>>& GMaterialsService::getMaterialsMap() {
    return materialsMap;
}

};
