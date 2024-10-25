#ifndef SPCGANESHAENGINE_GSCENE_NODE_H
#define SPCGANESHAENGINE_GSCENE_NODE_H

#include "gmesh.h"
#include "gvector.h"
#include "gcommandservice.h"
#include "gvulkandevice.h"
#include "gvulkanimage.h"
#include "gvulkanbuffer.h"

namespace spcGaneshaEngine {

class GSceneNode {
public:
    GSceneNode(std::shared_ptr<GMesh> mesh, 
               const GVector& newTranslation,
               const GVector& newRotation,
               const GVector& newScale,
               std::shared_ptr<GVULKANImage> newMaterial);
    ~GSceneNode();

    void deploy(std::shared_ptr<GCommandServiceProtocol> commandService, GVULKANDevice& vulkanDevice);

    std::shared_ptr<GMesh> mesh;
    GVector translation;
    GVector rotation;
    GVector scale;

    std::shared_ptr<GVULKANImage> material;

    //    void setupInstance(const GVector& newTranslation, const GVector& newRotation, const GVector& newScale, GVULKANImage *newMaterial);
    //    GVULKANImage *getMaterial();
    //    GVULKANBuffer& getInstanceBuffer();
    //    GMesh *getMesh();
    //
        GVULKANBuffer instanceBuffer;
    //    GCommandServiceProtocol *commandService;
    //    GDescriptorsetServiceProtocol *descriptorsetService;
    //    GVULKANDevice& vulkanDevice;
};

};  //  spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GSCENE_NODE_H
