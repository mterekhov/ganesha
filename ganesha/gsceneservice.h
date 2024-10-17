#ifndef SPCGANESHAENGINE_GSCENE_SERVICE_H
#define SPCGANESHAENGINE_GSCENE_SERVICE_H

#include <vector>

#include "gscene.h"

namespace spcGaneshaEngine {

class GSceneServiceProtocol {
public:
    virtual void init() = 0;
    virtual void destroy() = 0;
    
    /// Creates new GScene object and parses gltf content so it can be uploaded to hardware for render
    /// - Parameter gltfFile: gltf file which describes scene content
    virtual GScene createNewScene(const std::string& gltfFile) = 0;

    /// Creates new mesh object
    /// - Parameter vertexesArray: vertexes array
    /// - Parameter indexesArray: indexes array
    virtual std::shared_ptr<GMesh> createNewMesh(const std::vector<TFloat>& vertexesArray, const TIndexArray& indexesArray) = 0;
    
    /// Creates new mesh object
    /// - Parameter vertexesArray: gltf file which describes scene content
    virtual void addMeshInstance(GScene& scene, std::shared_ptr<GMesh>, const GVector& translation, const GVector& rotation, const GVector& scale, std::shared_ptr<GVULKANImage> material) = 0;

    /// Adds fragment shader to scene
    /// - Parameter scene: scene to which this shader should be added
    /// - Parameter shaderFile: full filepath to binary shader
    virtual void addFragmentShader(GScene& scene, const std::string& shaderFilePath) = 0;

    /// Adds vertex shader to scene
    /// - Parameter scene: scene to which this shader should be added
    /// - Parameter shaderFile: full filepath to binary shader
    virtual void addVertexShader(GScene& scene, const std::string& shaderFilePath) = 0;

    /// Takes all the content described in scene object and loads it to video memory. GMesh and GVULKANImage already loaded by that moment
    virtual void loadScene(const GScene& scene) = 0;
    
};

class GSceneService: public GSceneServiceProtocol {
public:
    GSceneService(GDescriptorsetServiceProtocol *descriptorsetService, GCommandServiceProtocol *commandService, GVULKANDevice& vulkanDevice);
    virtual ~GSceneService();
    
    void init() override;
    void destroy() override;
    GScene createNewScene(const std::string& gltfFile) override;
    std::shared_ptr<GMesh> createNewMesh(const std::vector<TFloat>& vertexesArray, const TIndexArray& indexesArray) override;
    void addMeshInstance(GScene& scene, std::shared_ptr<GMesh>, const GVector& translation, const GVector& rotation, const GVector& scale, std::shared_ptr<GVULKANImage> material) override;
    void addFragmentShader(GScene& scene, const std::string& shaderFilePath) override;
    void addVertexShader(GScene& scene, const std::string& shaderFilePath) override;
    void loadScene(const GScene& scene) override;

private:
    GDescriptorsetServiceProtocol *descriptorsetService;
    GCommandServiceProtocol *commandService;
    GVULKANDevice& vulkanDevice;
};

};  //  spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GSCENE_SERVICE_H
