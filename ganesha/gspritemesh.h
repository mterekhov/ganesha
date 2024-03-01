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

    void render(VkCommandBuffer renderCommand) override;
    void destroyNode(VkDevice device) override;

private:
    GVULKANBuffer vertexesBuffer;
    GVULKANBuffer indexesBuffer;
    GVULKANImage *material;
};

};  //  spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GSPRITEMESH_H
