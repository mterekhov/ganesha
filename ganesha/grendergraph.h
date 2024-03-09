#ifndef SPCGANESHAENGINE_GRENDERGRAPH_H
#define SPCGANESHAENGINE_GRENDERGRAPH_H

#include <vector>
#include <array>

#include "ggraphnode.h"
#include "gmaterialsservice.h"
#include "gvulkanbuffer.h"
#include "gganeshacontent.h"
#include "gdescriptorsetservice.h"
#include "gcommandservice.h"
#include "gspritemesh.h"

namespace spcGaneshaEngine {

/// Manipulates with current position
class GRenderGraph {
public:
    GRenderGraph(GCommandServiceProtocol *commandService);
    ~GRenderGraph();
    
    void createGraph(GDescriptorsetServiceProtocol *descriptorsetService, GVULKANDevice& vulkanDevice);
    void destroyGraph(VkDevice device);
    
    void loadContent(const GGaneshaContent& contentLoader, GDescriptorsetServiceProtocol *descriptorsetService, GVULKANDevice& vulkanDevice);

    GSpriteMesh *createSpriteMesh(const std::string& materialFilePath, GDescriptorsetServiceProtocol *descriptorsetService, GVULKANDevice& vulkanDevice);
    void pushNode(GGraphNode *node);
    
    std::vector<GGraphNode *>& getNodeArray();

private:
    GCommandServiceProtocol *commandService;
    std::vector<GGraphNode *> graphNodeArray;
    GMaterialsServiceProtocol *materialsService;
};

};  //  spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GRENDERGRAPH_H
