#ifndef SPCGANESHAENGINE_GGRAPHNODE_H
#define SPCGANESHAENGINE_GGRAPHNODE_H

#include <vector>

#include "gmesh.h"
#include "gvector.h"
#include "gmatrix.h"
#include "gvulkanbuffer.h"
#include "gvulkandevice.h"
#include "gcommandservice.h"
#include "gdescriptorsetservice.h"

namespace spcGaneshaEngine {

class GGraphNode {
public:
    GGraphNode(GMesh *mesh, GDescriptorsetServiceProtocol *descriptorsetService, GCommandServiceProtocol *commandService, GVULKANDevice& vulkanDevice);
    ~GGraphNode();
    
    void addInstance(const GVector& translation, const GVector& rotation, const GVector& scale);
    TUInt getInstancesCount();
    
    GMesh *mesh;
    GVULKANBuffer instanceBuffer;
private:
    std::vector<GVector> instancesArray;
    GCommandServiceProtocol *commandService;
    GDescriptorsetServiceProtocol *descriptorsetService;
    GVULKANDevice& vulkanDevice;
};

};  //  spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GGRAPHNODE_H
