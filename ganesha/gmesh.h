#ifndef SPCGANESHAENGINE_GMESH_H
#define SPCGANESHAENGINE_GMESH_H

#include <vulkan/vulkan.h>

#include "ganeshatypes.h"

namespace spcGaneshaEngine {

class GMesh {
public:
    virtual void render(TUInt instancesNumber, VkCommandBuffer renderCommand) = 0;
    virtual void destroyNode(VkDevice device) = 0;
};

};  //  spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GMESH_H
