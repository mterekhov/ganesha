#include "gvulkanimage.h"

namespace spcGaneshaEngine {

GVULKANImage::GVULKANImage(const std::string& textureFileName) : textureFileName(textureFileName),
                                                                imageExtent{0, 0},
                                                                image(VK_NULL_HANDLE),
                                                                imageMemory(VK_NULL_HANDLE),
                                                                imageView(VK_NULL_HANDLE),
                                                                sampler(VK_NULL_HANDLE) {
    
}

GVULKANImage::~GVULKANImage() {
    
}

}
