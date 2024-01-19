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

namespace spcGaneshaEngine {

/// Manipulates with current position
class GRenderGraph {
public:
    static VkVertexInputBindingDescription getBindingDescription();
    static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions();

    GRenderGraph(GCommandServiceProtocol *commandService);
    ~GRenderGraph();
    void createGraph(GDescriptorsetServiceProtocol *descriptorsetService, GVULKANDevice& vulkanDevice);
    void destroyGraph(VkDevice device);
    
    void loadContent(GGaneshaContent& contentLoader, GDescriptorsetServiceProtocol *descriptorsetService, GVULKANDevice& vulkanDevice);

    GGraphNode *createSpriteNode(const std::string& materialFilePath, GDescriptorsetServiceProtocol *descriptorsetService, GVULKANDevice& vulkanDevice);
    void pushNode(GGraphNode *node);
    
    std::vector<GGraphNode *>& getNodeArray();
    std::vector<VkPipelineShaderStageCreateInfo>& getShadersArray();
    GVULKANBuffer& getModelBuffer();

private:
    GVULKANBuffer modelBuffer;
    GCommandServiceProtocol *commandService;
    std::vector<GGraphNode *> graphNodeArray;
    std::vector<VkPipelineShaderStageCreateInfo> shadersArray;
    GMaterialsServiceProtocol *materialsService;

    VkPipelineShaderStageCreateInfo createShader(const std::string& shaderFile,  const VkShaderStageFlagBits stage, VkDevice device);
};

};  //  spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GRENDERGRAPH_H
