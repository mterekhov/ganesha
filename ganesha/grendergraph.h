#ifndef SPCGANESHAENGINE_GRENDERGRAPH_H
#define SPCGANESHAENGINE_GRENDERGRAPH_H

#include <vector>

#include "ganeshastubdata.h"
#include "ggraphnode.h"
#include "gmaterialsservice.h"
#include "gvulkanbuffer.h"
#include "gganeshacontent.h"
#include "gdescriptorsetservice.h"

namespace spcGaneshaEngine {

/// Manipulates with current position
class GRenderGraph {
public:
    GRenderGraph();
    ~GRenderGraph();
    void createGraph(GVULKANDevice& vulkanDevice, VkCommandPool commandPool);
    void destroyGraph(VkDevice device);
    
    void loadContent(GGaneshaContent& contentLoader, GVULKANDevice& vulkanDevice, VkCommandPool commandPool, GDescriptorsetServiceProtocol *descriptorsetService);

    GGraphNode *createSpriteNode(const std::string& materialFilePath, GVULKANDevice& vulkanDevice, VkCommandPool commandPool, GDescriptorsetServiceProtocol *descriptorsetService);
    void pushNode(GGraphNode *node);
    
    std::vector<GGraphNode *>& getNodeArray();
    std::vector<VkPipelineShaderStageCreateInfo>& getShadersArray();
    
private:
    std::vector<GGraphNode *> graphNodeArray;
    std::vector<VkPipelineShaderStageCreateInfo> shadersArray;
    GMaterialsServiceProtocol *materialsService;

    VkPipelineShaderStageCreateInfo createShader(const std::string& shaderFile,  const VkShaderStageFlagBits stage, VkDevice device);
};

};  //  spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GRENDERGRAPH_H
