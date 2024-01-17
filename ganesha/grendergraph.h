#ifndef SPCGANESHAENGINE_GRENDERGRAPH_H
#define SPCGANESHAENGINE_GRENDERGRAPH_H

#include <vector>

#include "ganeshastubdata.h"
#include "ggraphnode.h"
#include "gmaterialsservice.h"
#include "gvulkanbuffer.h"
#include "gganeshacontent.h"

namespace spcGaneshaEngine {

/// Manipulates with current position
class GRenderGraph {
public:
    GRenderGraph(GLog& log);
    ~GRenderGraph();
    void createGraph(GVULKANDevice& vulkanDevice, VkCommandPool commandPool);
    void destroyGraph(VkDevice device);
    
    void loadContent(GGaneshaContent& contentLoader, GVULKANDevice& vulkanDevice, VkCommandPool commandPool);

    GGraphNode *createSpriteNode(const std::string& materialFilePath, GVULKANDevice& vulkanDevice, VkCommandPool commandPool);
    void pushNode(GGraphNode *node);
    
    std::vector<GGraphNode *>& getNodeArray();
    std::vector<VkPipelineShaderStageCreateInfo>& getShadersArray();
    VkDescriptorSetLayout getDescriptoprsetsLayout();
    VkDescriptorSet getDescriptorset();
    
private:
    GLog& log;

    std::vector<GGraphNode *> graphNodeArray;
    std::vector<VkPipelineShaderStageCreateInfo> shadersArray;
    GMaterialsServiceProtocol *materialsService;
    VkDescriptorPool descriptorsetsPool;
    VkDescriptorSetLayout descriptorsetsLayout;
    VkDescriptorSet descriptorset;
    
    VkPipelineShaderStageCreateInfo createShader(const std::string& shaderFile,  const VkShaderStageFlagBits stage, VkDevice device);
    
    VkDescriptorSet createDescriptorset(VkDevice device);
    void attachImageToDescriptorset(GVULKANImage& image, VkDescriptorSet dstDescriptorset, TUInt bindingIndex, VkDevice device);
    void attachBufferToDescriptorset(GVULKANBuffer& buffer, VkDescriptorSet dstDescriptorset, TUInt bindingIndex, VkDevice device);
    VkDescriptorPool createDescriptorsetsPool(VkDevice device);
    VkDescriptorSetLayout createDescriptorsetsLayout(VkDevice device);
};

};  //  spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GRENDERGRAPH_H
