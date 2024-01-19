#ifndef SPCGANESHAENGINE_GSPRITENODE_H
#define SPCGANESHAENGINE_GSPRITENODE_H

#include <string>

#include "gnode.h"
#include "gvulkanbuffer.h"
#include "gvulkanimage.h"

namespace spcGaneshaEngine {

class GSpriteNode: public GNode {
public:
    GSpriteNode(GVULKANImage *material, GVULKANDevice& vulkanDevice, VkCommandPool commandPool);
    virtual ~GSpriteNode();

    virtual void render(VkCommandBuffer renderCommand);
    virtual void destroyNode(VkDevice device);

private:
    GVULKANBuffer vertexesBuffer;
    GVULKANBuffer indexesBuffer;
    GVULKANImage *material;
};

};  //  spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GSPRITENODE_H
