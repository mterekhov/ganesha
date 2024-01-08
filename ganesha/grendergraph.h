#ifndef SPCGANESHAENGINE_GRENDERGRAPH_H
#define SPCGANESHAENGINE_GRENDERGRAPH_H

#include <vector>

#include "ganeshastubdata.h"
#include "ggraphnode.h"

namespace spcGaneshaEngine {

/// Manipulates with current position
class GRenderGraph {
public:
    GRenderGraph();
    ~GRenderGraph();

    void defineVertexesArray(const std::vector<Vertex>& newVertexesArray);
    const std::vector<Vertex>& getVertexesArray() const;
    
    void defineIndexesArray(const TIndexArray& newIndexesArray);
    const TIndexArray& getIndexesArray() const;
    
    void pushVertexShader(const std::string& shaderFullFilePath);
    TStringsArray& getVertexesShadersArray();
    
    void pushFragmentShader(const std::string& shaderFullFilePath);
    TStringsArray& getFragmentShadersArray();

    void pushTextureFilePath(const std::string& textureFullFilePath);
    TStringsArray& getTextureFilePathArray();

private:
    std::vector<GGraphNode *> graph;
    
    TStringsArray vertexShadersArray;
    TStringsArray fragmentsShadersArray;
    TStringsArray texturesFilePathArray;

    std::vector<Vertex> vertexesArray;
    TIndexArray indexesArray;
};

};  //  spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GRENDERGRAPH_H
