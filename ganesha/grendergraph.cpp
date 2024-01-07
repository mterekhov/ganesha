//
//  grendergraph.cpp
//  ganesha
//
//  Created by cipher on 17.12.2023.
//

#include "grendergraph.h"

namespace spcGaneshaEngine {

GRenderGraph::GRenderGraph() {
    
}

GRenderGraph::~GRenderGraph() {
    
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
