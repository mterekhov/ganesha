#include "gsceneservice.h"

namespace spcGaneshaEngine {

GSceneService::GSceneService(std::shared_ptr<GDescriptorsetServiceProtocol> descriptorsetService,
                             std::shared_ptr<GImageServiceProtocol> imageService,
                             std::shared_ptr<GShadersServiceProtocol> shaderService,
                             std::shared_ptr<GCommandServiceProtocol> commandService,
                             GVULKANDevice& vulkanDevice) : descriptorsetService(descriptorsetService), imageService(imageService), shaderService(shaderService), commandService(commandService), vulkanDevice(vulkanDevice) {
    
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
    return std::make_shared<GMesh>(vertexesArray, indexesArray);
}

void GSceneService::addMeshInstance(GScene& scene, std::shared_ptr<GMesh> mesh, const GVector& translation, const GVector& rotation, const GVector& scale, std::shared_ptr<GVULKANImage> material) {
    scene.meshesArray.push_back(mesh);
    
    std::shared_ptr<GSceneNode> newNode = std::make_shared<GSceneNode>(mesh, translation, rotation, scale, material);
    scene.nodesArray.push_back(newNode);
}

void GSceneService::addFragmentShader(GScene& scene, const std::string& shaderFilePath) {
    scene.fragmentShadersArray.push_back(std::make_shared<GShader>(shaderFilePath));
}

void GSceneService::addVertexShader(GScene& scene, const std::string& shaderFilePath) {
    scene.vertexShadersArray.push_back(std::make_shared<GShader>(shaderFilePath));
}

void GSceneService::deployScene(const GScene& scene) {
    //  Deploy geometry
    for (std::shared_ptr<GMesh> mesh:scene.meshesArray) {
        mesh->deploy(commandService, vulkanDevice);
    }
    
    //  Deploy textures
    for (std::shared_ptr<GVULKANImage> material:scene.materialsArray) {
        GTGA tga(material->textureFileName);
        imageService->deployImage(material,
                                  tga,
                                  commandService,
                                  vulkanDevice);
//        material->deploy(VK_FORMAT_R8G8B8A8_SRGB,
//                         VK_IMAGE_ASPECT_COLOR_BIT,
//                         VK_IMAGE_TILING_OPTIMAL,
//                          VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
//                         commandService,
//                         vulkanDevice);
    }

    //  Deploy shaders
    for (std::shared_ptr<GShader> shader:scene.vertexShadersArray) {
        shaderService->deployShader(shader, vulkanDevice);
    }
    for (std::shared_ptr<GShader> shader:scene.fragmentShadersArray) {
        shaderService->deployShader(shader, vulkanDevice);
    }
}

void GSceneService::clearScene(const GScene& scene) {
    
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
//    GVULKANImage *material = imageService->findMaterial(materialFilePath);
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
