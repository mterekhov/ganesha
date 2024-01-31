#ifndef SPCGANESHAENGINE_GMESH_H
#define SPCGANESHAENGINE_GMESH_H

#include <vulkan/vulkan.h>

namespace spcGaneshaEngine {

class GMesh {
public:
    virtual void render(VkCommandBuffer renderCommand) = 0;
    virtual void destroyNode(VkDevice device) = 0;
};

};  //  spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GMESH_H
