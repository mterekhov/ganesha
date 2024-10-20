
#ifndef SPCGANESHAENGINE_GVULKANIMAGE_H
#define SPCGANESHAENGINE_GVULKANIMAGE_H

#include <string>
#include <vulkan/vulkan.h>

namespace spcGaneshaEngine {

class GVULKANImage {
public:
    GVULKANImage(const std::string& textureFileName);
    ~GVULKANImage();

    std::string textureFileName;
    VkExtent2D imageExtent;
    VkImage image;
    VkDeviceMemory imageMemory;
    VkImageView imageView;
    VkSampler sampler;
    
};

}   //  namespace spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GVULKANIMAGE_H
