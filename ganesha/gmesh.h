#ifndef SPCGANESHAENGINE_GMESH_H
#define SPCGANESHAENGINE_GMESH_H

#include <vulkan/vulkan.h>

#include "ganeshatypes.h"
#include "gvulkanbuffer.h"
#include "gvulkanimage.h"
#include "gcommandservice.h"

namespace spcGaneshaEngine {

class GMesh {
public:
    GMesh(const std::vector<TFloat>& vertexesArray,
          const TIndexArray& indexesArray);
    virtual ~GMesh();
    
    virtual bool isDeployed();
    virtual void deploy(GCommandServiceProtocol *commandService,
                        GVULKANDevice& vulkanDevice);
    virtual void render(TUInt instancesNumber, VkCommandBuffer renderCommand);
    virtual void destroyMesh(VkDevice device);
    
private:
    std::vector<TFloat> vertexesArray;
    TIndexArray indexesArray;
    GVULKANBuffer vertexesBuffer;
    GVULKANBuffer indexesBuffer;
};

};  //  spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GMESH_H
