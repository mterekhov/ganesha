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
    
    void defineIndecesArray(const std::vector<uint32_t>& newIndicesArray);
    const std::vector<uint32_t>& getIndecesArray() const;
    
private:
    std::vector<Vertex> verticesArray;
    std::vector<uint32_t> indicesArray;
};

};  //  spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GRENDERGRAPH_H
