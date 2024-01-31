#include "grendergraph.h"
#include "gspritemesh.h"
#include "gvulkantools.h"

namespace spcGaneshaEngine {

VkVertexInputBindingDescription GRenderGraph::getBindingDescription() {
    VkVertexInputBindingDescription bindingDescription {};
    
    bindingDescription.binding = 0;
    bindingDescription.stride = sizeof(TFloat) * 5;
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    
    return bindingDescription;
}

std::array<VkVertexInputAttributeDescription, 2> GRenderGraph::getAttributeDescriptions()  {
    std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};
    
    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[0].offset = 0;
    
    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[1].offset = sizeof(TFloat) * 3;
    
    return attributeDescriptions;
}

GRenderGraph::GRenderGraph(GCommandServiceProtocol *commandService) : commandService(commandService) {
}

GRenderGraph::~GRenderGraph() {
    
}

void GRenderGraph::createGraph(GDescriptorsetServiceProtocol *descriptorsetService, GVULKANDevice& vulkanDevice) {
    materialsService = new GMaterialsService(commandService);
    
    GMatrix identityMatrix = GMatrix::identityMatrix();
    modelBuffer.createBuffer(&identityMatrix,
                             sizeof(GMatrix),
                             VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                             VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                             false,
                             vulkanDevice,
                             commandService);
    descriptorsetService->attachBufferToDescriptorset(modelBuffer, 1, vulkanDevice.getLogicalDevice());
}

void GRenderGraph::destroyGraph(VkDevice device) {
    for (auto node:graphNodeArray) {
        node->mesh->destroyNode(device);
        delete node->mesh;
        delete node;
    }
    
    for (auto shader:shadersArray) {
        vkDestroyShaderModule(device, shader.module, nullptr);
    }
    
    materialsService->destroyMaterials(device);
    delete materialsService;
    
    modelBuffer.destroyBuffer(device);
}

void GRenderGraph::loadContent(GGaneshaContent& contentLoader, GDescriptorsetServiceProtocol *descriptorsetService, GVULKANDevice& vulkanDevice) {
    //  loading shaders
    for (auto shader:contentLoader.getVertexShadersArray()) {
        shadersArray.push_back(createShader(shader, VK_SHADER_STAGE_VERTEX_BIT, vulkanDevice.getLogicalDevice()));
    }
    for (auto shader:contentLoader.getFragmetShadersArrray()) {
        shadersArray.push_back(createShader(shader, VK_SHADER_STAGE_FRAGMENT_BIT, vulkanDevice.getLogicalDevice()));
    }
    
    //  creating sprites
    for (auto spriteMaterial:contentLoader.getSpritesMaterialsArray()) {
        GGraphNode *newSprite = createSpriteNode(spriteMaterial, descriptorsetService, vulkanDevice);
        pushNode(newSprite);
        
        newSprite = createSpriteNode(spriteMaterial, descriptorsetService, vulkanDevice);
//        newSprite->rts.m[3][1] = 5;
        pushNode(newSprite);
    }
}

GGraphNode *GRenderGraph::createSpriteNode(const std::string& materialFilePath, GDescriptorsetServiceProtocol *descriptorsetService, GVULKANDevice& vulkanDevice) {
    //  creating mesh
    GVULKANImage *material = materialsService->findMaterial(materialFilePath);
    if (material == 0) {
        material = materialsService->createMaterial(materialFilePath, vulkanDevice);
        descriptorsetService->attachImageToDescriptorset(*material, 2, vulkanDevice.getLogicalDevice());
    }
    GSpriteMesh *spriteMesh = new GSpriteMesh(material, vulkanDevice, commandService);
    
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

GVULKANBuffer& GRenderGraph::getModelBuffer() {
    return modelBuffer;
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
        GLOG_ERROR("No chance to create shader module\n");
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
