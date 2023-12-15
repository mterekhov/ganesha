//
//  gvulkaninstance.hpp
//  ganesha
//
//  Created by Mihail Terekhov on 23.07.2021.
//

#ifndef SPCGANESHAENGINE_GVULKANIMAGEVIEW_H
#define SPCGANESHAENGINE_GVULKANIMAGEVIEW_H

#include <vulkan/vulkan.h>

namespace spcGaneshaEngine {

class GVULKANImageView {
public:
    GVULKANImageView(const VkImage& image, const VkFormat& imageFormat, const VkDevice& logicalDevice);
    ~GVULKANImageView();
    
    VkImageView& getImageView();

private:
    const VkDevice& logicalDevice;
    VkImageView imageView;
    
    VkImageView createImageView(const VkImage& image, const VkFormat& imageFormat, const VkDevice& device);
};

}   //  namespace spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GVULKANIMAGEVIEW_H
