#include "grendergraph.h"
#include "gvulkantools.h"

namespace spcGaneshaEngine {

GRenderGraph::GRenderGraph(GCommandServiceProtocol *commandService) : commandService(commandService) {
}

GRenderGraph::~GRenderGraph() {
    
}

void GRenderGraph::createGraph(GDescriptorsetServiceProtocol *descriptorsetService, GVULKANDevice& vulkanDevice) {
    materialsService = new GMaterialsService(commandService, vulkanDevice);
    materialsService->init();
}

void GRenderGraph::destroyGraph(VkDevice device) {
    for (auto node:graphNodeArray) {
        node->mesh->destroyNode(device);
        delete node->mesh;
        delete node;
    }
    
    materialsService->destroy();
    delete materialsService;
}

void GRenderGraph::loadContent(const GGaneshaContent& contentLoader, GDescriptorsetServiceProtocol *descriptorsetService, GVULKANDevice& vulkanDevice) {
    //  creating material
    const TStringsArray& materialsArray = contentLoader.getSpritesMaterialsArray();
    
    //  creating meshj
    GSpriteMesh *spriteMesh = createSpriteMesh(*materialsArray.begin(), descriptorsetService, vulkanDevice);
    
    //  creating mesh instances
    GGraphNode *meshInstance = new GGraphNode(spriteMesh, descriptorsetService, commandService, vulkanDevice);
    meshInstance->addInstance(GVector(0.0, 0.0, 0.0), GVector(0.0, 0.0, 0.0), GVector(1.0, 1.0, 1.0));
    meshInstance->addInstance(GVector(0.0, 0.0, 0.5), GVector(0.0, 3.14f/180.0f*45.0f, 0.0), GVector(1.0, 1.0, 1.0));
    meshInstance->addInstance(GVector(-2, 0.0, 0.0), GVector(0.0, 0.0, 0.0), GVector(1.0, 1.0, 1.0));
    pushNode(meshInstance);
}

GSpriteMesh *GRenderGraph::createSpriteMesh(const std::string& materialFilePath, GDescriptorsetServiceProtocol *descriptorsetService, GVULKANDevice& vulkanDevice) {
    //  creating mesh
    GVULKANImage *material = materialsService->findMaterial(materialFilePath);
    if (material == 0) {
        material = materialsService->createMaterial(materialFilePath);
        descriptorsetService->updateDescriptorset(*material, 2);
    }
    
    return new GSpriteMesh(material, vulkanDevice, commandService);
}

void GRenderGraph::pushNode(GGraphNode *node) {
    graphNodeArray.push_back(node);
}

std::vector<GGraphNode *>& GRenderGraph::getNodeArray() {
    return graphNodeArray;
}

};  //  spcGaneshaEngine
