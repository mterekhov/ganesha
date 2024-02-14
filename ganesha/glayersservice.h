#ifndef SPCGANESHAENGINE_GLAYERSSERVICE_H
#define SPCGANESHAENGINE_GLAYERSSERVICE_H

#include <vector>

#include "glayer.h"

namespace spcGaneshaEngine {

typedef std::vector<GLayer *> TLayersArray;

class GLayersServiceProtocol {
public:
    virtual void init() = 0;
    virtual void destroy() = 0;
    virtual void pushLayer(GLayer *layer) = 0;
    virtual void pushOverlay(GLayer *layer) = 0;
    virtual void popLayer(GLayer *layer) = 0;
    virtual void popOverlay(GLayer *layer) = 0;
    virtual TLayersArray::iterator begin() = 0;
    virtual TLayersArray::iterator end() = 0;
};

class GLayersService: public GLayersServiceProtocol {
public:
    GLayersService();
    virtual ~GLayersService();

    virtual void init();
    virtual void destroy();

    virtual void pushLayer(GLayer *layer);
    virtual void pushOverlay(GLayer *layer);
    virtual void popLayer(GLayer *layer);
    virtual void popOverlay(GLayer *layer);
    virtual TLayersArray::iterator begin();
    virtual TLayersArray::iterator end();

private:
    TLayersArray layersArray;
    TLayersArray::iterator layerInsert;
};

};  //  spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GLAYERSSERVICE_H
