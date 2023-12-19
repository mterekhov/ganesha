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

void GRenderGraph::defineIndecesArray(const std::vector<uint16_t>& newIndicesArray) {
    indicesArray = newIndicesArray;
}

const std::vector<uint16_t>& GRenderGraph::getIndecesArray() const {
    return indicesArray;
}

};  //  spcGaneshaEngine
