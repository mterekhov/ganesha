#include "grendergraph.h"
#include "gspritenode.h"
#include "gvulkantools.h"

namespace spcGaneshaEngine {

GRenderGraph::GRenderGraph(GLog& log) : log(log) {
}

GRenderGraph::~GRenderGraph() {
    
}

void GRenderGraph::createGraph(GVULKANDevice& vulkanDevice, VkCommandPool commandPool) {
    materialsService = new GMaterialsService(log, commandPool);
}

void GRenderGraph::destroyGraph(VkDevice device) {
    for (auto node:graphNodeArray) {
        node->node->destroyNode(device);
        delete node->node;
        delete node;
    }
    
    for (auto shader:shadersArray) {
        vkDestroyShaderModule(device, shader.module, nullptr);
    }
    
    materialsService->destroyMaterials(device);
    delete materialsService;
}

void GRenderGraph::loadContent(GGaneshaContent& contentLoader, GVULKANDevice& vulkanDevice, VkCommandPool commandPool, GDescriptorsetServiceProtocol *descriptorsetService) {
    //  loading shaders
    for (auto shader:contentLoader.getVertexShadersArray()) {
        shadersArray.push_back(createShader(shader, VK_SHADER_STAGE_VERTEX_BIT, vulkanDevice.getLogicalDevice()));
    }
    for (auto shader:contentLoader.getFragmetShadersArrray()) {
        shadersArray.push_back(createShader(shader, VK_SHADER_STAGE_FRAGMENT_BIT, vulkanDevice.getLogicalDevice()));
    }
    
    //  creating sprites
    for (auto spriteMaterial:contentLoader.getSpritesMaterialsArray()) {
        GGraphNode *newSprite = createSpriteNode(spriteMaterial, vulkanDevice, commandPool, descriptorsetService);
        pushNode(newSprite);
    }
}

GGraphNode *GRenderGraph::createSpriteNode(const std::string& materialFilePath, GVULKANDevice& vulkanDevice, VkCommandPool commandPool, GDescriptorsetServiceProtocol *descriptorsetService) {
    //  creating mesh
    GVULKANImage *material = materialsService->findMaterial(materialFilePath);
    if (material == 0) {
        material = materialsService->createMaterial(materialFilePath, vulkanDevice);
        descriptorsetService->attachImageToDescriptorset(*material, 2, vulkanDevice.getLogicalDevice());
    }
    GSpriteNode *spriteMesh = new GSpriteNode(material, vulkanDevice, commandPool, log);
    
    //  creating mesh instance
    GGraphNode *meshInstance = new GGraphNode(spriteMesh);
    return meshInstance;
}

void GRenderGraph::pushNode(GGraphNode *node) {
    graphNodeArray.push_back(node);
}

std::vector<GGraphNode *>& GRenderGraph::getNodeArray() {
    return graphNodeArray;
}

VkPipelineShaderStageCreateInfo GRenderGraph::createShader(const std::string& shaderFile, const VkShaderStageFlagBits stage, VkDevice device) {
    GVULKANTools tools;
    const std::vector<uint8_t> code = tools.readFile(shaderFile);
    VkShaderModuleCreateInfo createInfo = { };
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const TUInt*>(code.data());
    
    VkShaderModule shaderModule;
    if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        log.error("No chance to create shader module\n");
    }
    
    VkPipelineShaderStageCreateInfo shaderStageInfo = { };
    shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStageInfo.stage = stage;
    shaderStageInfo.module = shaderModule;
    shaderStageInfo.pName = "main";
    
    return shaderStageInfo;
}

std::vector<VkPipelineShaderStageCreateInfo>& GRenderGraph::getShadersArray() {
    return shadersArray;
}

};  //  spcGaneshaEngine
