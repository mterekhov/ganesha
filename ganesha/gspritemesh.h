#ifndef SPCGANESHAENGINE_GSPRITEMESH_H
#define SPCGANESHAENGINE_GSPRITEMESH_H

#include <string>

#include "gmesh.h"
#include "gvulkanbuffer.h"
#include "gvulkanimage.h"
#include "gcommandservice.h"

namespace spcGaneshaEngine {

class GSpriteMesh: public GMesh {
public:
    GSpriteMesh(GVULKANImage *material, GVULKANDevice& vulkanDevice, GCommandServiceProtocol *commandService);
    virtual ~GSpriteMesh();

    virtual void render(VkCommandBuffer renderCommand);
    virtual void destroyNode(VkDevice device);

private:
    GVULKANBuffer vertexesBuffer;
    GVULKANBuffer indexesBuffer;
    GVULKANImage *material;
};

};  //  spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GSPRITEMESH_H
