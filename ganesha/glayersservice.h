#ifndef SPCGANESHAENGINE_GLAYERSSERVICE_H
#define SPCGANESHAENGINE_GLAYERSSERVICE_H

#include <vector>

#include "glayer.h"

namespace spcGaneshaEngine {

typedef std::vector<std::shared_ptr<GLayer>> TLayersArray;

class GLayersServiceProtocol {
public:
    virtual void init() = 0;
    virtual void destroy() = 0;
    virtual void pushLayer(std::shared_ptr<GLayer> layer) = 0;
    virtual void pushOverlay(std::shared_ptr<GLayer> layer) = 0;
    virtual void popLayer(std::shared_ptr<GLayer> layer) = 0;
    virtual void popOverlay(std::shared_ptr<GLayer> layer) = 0;
    virtual TLayersArray::iterator begin() = 0;
    virtual TLayersArray::iterator end() = 0;
};

class GLayersService: public GLayersServiceProtocol {
public:
    GLayersService();
    virtual ~GLayersService();

    void init() override;
    void destroy() override;

    void pushLayer(std::shared_ptr<GLayer> layer) override;
    void pushOverlay(std::shared_ptr<GLayer> layer) override;
    void popLayer(std::shared_ptr<GLayer> layer) override;
    void popOverlay(std::shared_ptr<GLayer> layer) override;
    TLayersArray::iterator begin() override;
    TLayersArray::iterator end() override;

private:
    TLayersArray layersArray;
    TLayersArray::iterator layerInsert;
};

};  //  spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GLAYERSSERVICE_H
