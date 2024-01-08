//
//  grendergraph.cpp
//  ganesha
//
//  Created by cipher on 17.12.2023.
//

#include "grendergraph.h"
#include "gspritenode.h"

namespace spcGaneshaEngine {

GRenderGraph::GRenderGraph() {
    
}

GRenderGraph::~GRenderGraph() {
    
}

GGraphNode *GRenderGraph::createSpriteNode(const std::string& textureFilePath) {
    //  creating mesh
    GSpriteNode *spriteMesh = new GSpriteNode(textureFilePath);
    
    //  creating mesh instance
    GGraphNode *meshInstance = new GGraphNode(spriteMesh);
    meshInstance->rtsMatrix = GMatrix::identityMatrix();
    
    return meshInstance;
}

void GRenderGraph::pushNode(GGraphNode *node) {
    graphNodeArray.push_back(node);
}

std::vector<GGraphNode *>& GRenderGraph::getNodeArray() {
    return graphNodeArray;
}

void GRenderGraph::pushVertexShader(const std::string& shaderFullFilePath) {
    vertexShadersArray.push_back(shaderFullFilePath);
}

TStringsArray& GRenderGraph::getVertexesShadersArray() {
    return vertexShadersArray;
}

void GRenderGraph::pushFragmentShader(const std::string& shaderFullFilePath) {
    fragmentsShadersArray.push_back(shaderFullFilePath);
}

TStringsArray& GRenderGraph::getFragmentShadersArray() {
    return fragmentsShadersArray;
}

void GRenderGraph::pushTextureFilePath(const std::string& shaderFullFilePath) {
    texturesFilePathArray.push_back(shaderFullFilePath);
}

TStringsArray& GRenderGraph::getTextureFilePathArray() {
    return texturesFilePathArray;
}

void GRenderGraph::defineVertexesArray(const std::vector<Vertex>& newVertexesArray) {
    vertexesArray = newVertexesArray;
}

const std::vector<Vertex>& GRenderGraph::getVertexesArray() const {
    return vertexesArray;
}

void GRenderGraph::defineIndexesArray(const TIndexArray& newIndexesArray) {
    indexesArray = newIndexesArray;
}

const TIndexArray& GRenderGraph::getIndexesArray() const {
    return indexesArray;
}

};  //  spcGaneshaEngine
