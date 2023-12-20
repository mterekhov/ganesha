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

void GRenderGraph::defineVertexesArray(const std::vector<Vertex>& newVertexesArray) {
    verticesArray = newVertexesArray;
}

const std::vector<Vertex>& GRenderGraph::getVertexesArray() const {
    return verticesArray;
}

void GRenderGraph::defineIndexesArray(const TIndexArray& newIndicesArray) {
    indicesArray = newIndicesArray;
}

const TIndexArray& GRenderGraph::getIndexesArray() const {
    return indicesArray;
}

};  //  spcGaneshaEngine
