#include "gsceneservice.h"

namespace spcGaneshaEngine {

GSceneService::GSceneService(GDescriptorsetServiceProtocol *descriptorsetService, GCommandServiceProtocol *commandService, GVULKANDevice& vulkanDevice) : descriptorsetService(descriptorsetService), commandService(commandService), vulkanDevice(vulkanDevice) {
    
}

GSceneService::~GSceneService() {
    
}


#pragma mark - GSceneServiceProtocol -

void GSceneService::init() {
    
}

void GSceneService::destroy() {
    
}

GScene GSceneService::createNewScene(const std::string& gltfFile) {
    return GScene();
}

std::shared_ptr<GMesh> GSceneService::createNewMesh(const std::vector<TFloat>& vertexesArray, const TIndexArray& indexesArray) {
    return std::make_shared<GMesh>(vertexesArray, indexesArray, commandService, vulkanDevice);
}

void GSceneService::addMeshInstance(GScene& scene, std::shared_ptr<GMesh> mesh, const GVector& translation, const GVector& rotation, const GVector& scale, std::shared_ptr<GVULKANImage> material) {
    scene.meshesArray.push_back(mesh);
    
    std::shared_ptr<GSceneNode> newNode = std::make_shared<GSceneNode>(mesh, translation, rotation, scale, material);
    scene.nodesArray.push_back(newNode);
}

void GSceneService::addFragmentShader(GScene& scene, const std::string& shaderFilePath) {
    scene.fragmentShadersArrray.push_back(shaderFilePath);
}

void GSceneService::addVertexShader(GScene& scene, const std::string& shaderFilePath) {
    scene.vertexShadersArray.push_back(shaderFilePath);
}

void GSceneService::loadScene(const GScene& scene) {
    
}

//GMesh* GScene::createNewMesh(const std::vector<TFloat>& vertexesArray, const TIndexArray& indexesArray) {
//    GMesh *newMesh = new GMesh(vertexesArray, indexesArray, commandService, vulkanDevice);
//    
//    meshesArray.push_back(newMesh);
//    
//    return newMesh;
//}
//
//void GScene::addMeshInstance(GMesh *mesh, const GVector& translation, const GVector& rotation, const GVector& scale, const std::string& materialFilePath) {
//    GSceneNode *newNode = new GSceneNode(mesh, descriptorsetService, commandService, vulkanDevice);
//    GVULKANImage *material = materialsService->findMaterial(materialFilePath);
//    newNode->setupInstance(translation, rotation, scale, material);
//    nodesArray.push_back(newNode);
//}
//
//void GScene::addFragmentShader(const std::string& shaderFilePath) {
//    fragmetShadersArrray.push_back(shaderFilePath);
//}
//
//void GScene::addVertexShader(const std::string& shaderFilePath) {
//    vertexShadersArray.push_back(shaderFilePath);
//}


}
