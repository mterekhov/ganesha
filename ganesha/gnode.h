#ifndef SPCGANESHAENGINE_GNODE_H
#define SPCGANESHAENGINE_GNODE_H

#include <vulkan/vulkan.h>

namespace spcGaneshaEngine {

class GNode {
public:
    virtual void render(VkCommandBuffer renderCommand) = 0;
    virtual void destroyNode(VkDevice device) = 0;
};

};  //  spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GNODE_H
