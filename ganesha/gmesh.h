#ifndef SPCGANESHAENGINE_GMESH_H
#define SPCGANESHAENGINE_GMESH_H

#include <vulkan/vulkan.h>
#include <vector>

#include "ganeshatypes.h"
#include "gcommandservice.h"
#include "gvulkandevice.h"
#include "gvulkanbuffer.h"

namespace spcGaneshaEngine {

class GMesh {
public:
    GMesh(const std::vector<TFloat>& vertexesArray,
          const TIndexArray& indexesArray);
    virtual ~GMesh();
    
    virtual bool isDeployed();
    virtual void deploy(std::shared_ptr<GCommandServiceProtocol> commandService,
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
