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

void GRenderGraph::defineVertecesArray(const std::vector<Vertex>& newVertecesArray) {
    verticesArray = newVertecesArray;
}

const std::vector<Vertex>& GRenderGraph::getVertecesArray() const {
    return verticesArray;
}

void GRenderGraph::defineIndecesArray(const TIndexArray& newIndicesArray) {
    indicesArray = newIndicesArray;
}

const TIndexArray& GRenderGraph::getIndecesArray() const {
    return indicesArray;
}

};  //  spcGaneshaEngine
