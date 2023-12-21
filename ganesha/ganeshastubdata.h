#ifndef SPCGANESHAENGINE_GANESHASTUBDATA_H
#define SPCGANESHAENGINE_GANESHASTUBDATA_H

#include <vulkan/vulkan.h>
#include <array>

#include "ganeshatypes.h"
#include "gcolor.h"
#include "gpoint.h"
#include "gpoint2d.h"

namespace spcGaneshaEngine {

struct Vertex {
public:
    GPoint pos;
    GPoint2D uv;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription {};
        
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        
        return bindingDescription;
    }
    
    static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions()  {
        std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};
        
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);
        
        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, uv);
        
        return attributeDescriptions;
    }
    
};

}   //  namespace spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GANESHASTUBDATA_H
