#include "grendergraph.h"
#include "gspritemesh.h"
#include "gvulkantools.h"

namespace spcGaneshaEngine {

GRenderGraph::GRenderGraph(GCommandServiceProtocol *commandService) : commandService(commandService) {
}

GRenderGraph::~GRenderGraph() {
    
}

void GRenderGraph::createGraph(GDescriptorsetServiceProtocol *descriptorsetService, GVULKANDevice& vulkanDevice) {
    materialsService = new GMaterialsService(commandService, vulkanDevice);
    materialsService->init();
    
    GMatrix identityMatrix = GMatrix::identityMatrix();
    modelBuffer.createBuffer(&identityMatrix,
                             sizeof(GMatrix),
                             VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                             VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                             false,
                             vulkanDevice,
                             commandService);
    descriptorsetService->attachBufferToDescriptorset(modelBuffer, 1);
}

void GRenderGraph::destroyGraph(VkDevice device) {
    for (auto node:graphNodeArray) {
        node->mesh->destroyNode(device);
        delete node->mesh;
        delete node;
    }
    
    materialsService->destroy();
    delete materialsService;
    
    modelBuffer.destroyBuffer(device);
}

void GRenderGraph::loadContent(const GGaneshaContent& contentLoader, GDescriptorsetServiceProtocol *descriptorsetService, GVULKANDevice& vulkanDevice) {
    //  creating sprites
    const TStringsArray& materialsArray = contentLoader.getSpritesMaterialsArray();

    GGraphNode *newSprite = createSpriteNode(*materialsArray.begin(), descriptorsetService, vulkanDevice);
    pushNode(newSprite);
    
    newSprite = createSpriteNode(*materialsArray.begin(), descriptorsetService, vulkanDevice);
    newSprite->rts.m[3][1] = 5;
    pushNode(newSprite);
    GLOG_INFO("sprite added\n");
}

GGraphNode *GRenderGraph::createSpriteNode(const std::string& materialFilePath, GDescriptorsetServiceProtocol *descriptorsetService, GVULKANDevice& vulkanDevice) {
    //  creating mesh
    GVULKANImage *material = materialsService->findMaterial(materialFilePath);
    if (material == 0) {
        material = materialsService->createMaterial(materialFilePath);
        descriptorsetService->attachImageToDescriptorset(*material, 2);
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


};  //  spcGaneshaEngine
