#ifndef SPCGANESHAENGINE_GRENDERGRAPH_H
#define SPCGANESHAENGINE_GRENDERGRAPH_H

#include <vector>

#include "ganeshastubdata.h"

namespace spcGaneshaEngine {

/// Manipulates with current position
class GRenderGraph {
public:
    GRenderGraph();
    ~GRenderGraph();

    void defineVertecesArray(const std::vector<Vertex>& newVertecesArray);
    const std::vector<Vertex>& getVertecesArray() const;
    
    void defineIndecesArray(const TIndexArray& newIndicesArray);
    const TIndexArray& getIndecesArray() const;
    
private:
    std::vector<Vertex> verticesArray;
    TIndexArray indicesArray;
};

};  //  spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GRENDERGRAPH_H
