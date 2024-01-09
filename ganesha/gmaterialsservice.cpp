#include "gmaterialsservice.h"
#include "gtga.h"

namespace spcGaneshaEngine {

GMaterialsService::GMaterialsService(GLog& log, VkCommandPool commandPool) : log(log), commandPool(commandPool) {
    
}

GMaterialsService::~GMaterialsService() {
}

GVULKANImage& GMaterialsService::createMaterial(const std::string &imageFilePath, GVULKANDevice& vulkanDevice) {
    GTGA tgaFile(imageFilePath);
    
    GVULKANImage *newImage = new GVULKANImage(log);
    newImage->createImage({ tgaFile.getWidth(), tgaFile.getHeight() },
                         VK_FORMAT_R8G8B8A8_SRGB,
                         VK_IMAGE_ASPECT_COLOR_BIT,
                         VK_IMAGE_TILING_OPTIMAL,
                         VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
                         vulkanDevice);
    newImage->deployData(tgaFile,
                        vulkanDevice,
                        commandPool);
    
    materialsMap[imageFilePath] = newImage;
    
    return *materialsMap[imageFilePath];
}

GVULKANImage& GMaterialsService::findMaterial(const std::string& imageFilePath) {
    return *materialsMap[imageFilePath];
}

std::map<std::string, GVULKANImage *>& GMaterialsService::getMaterialsMap() {
    return materialsMap;
}

void GMaterialsService::destroyMaterials(VkDevice device) {
    for (auto &pair : materialsMap) {
        pair.second->destroyImage(device);
        delete pair.second;
    }
}

};
