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
    for (auto &pair : materialsMap) {
        pair.second->destroyImage(device);
        delete pair.second;
    }
}

GVULKANImage *GMaterialsService::createMaterial(const std::string &imageFilePath) {
    GTGA tgaFile(imageFilePath);
    
    GVULKANImage *newImage = new GVULKANImage();
    newImage->createImage({ tgaFile.getWidth(), tgaFile.getHeight() },
                         VK_FORMAT_R8G8B8A8_SRGB,
                         VK_IMAGE_ASPECT_COLOR_BIT,
                         VK_IMAGE_TILING_OPTIMAL,
                         VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
                         vulkanDevice);
    newImage->deployData(tgaFile,
                        vulkanDevice,
                        commandService);
    
    materialsMap[imageFilePath] = newImage;
    
    return materialsMap[imageFilePath];
}

GVULKANImage *GMaterialsService::findMaterial(const std::string& imageFilePath) {
    return materialsMap[imageFilePath];
}

std::map<std::string, GVULKANImage *>& GMaterialsService::getMaterialsMap() {
    return materialsMap;
}

};
