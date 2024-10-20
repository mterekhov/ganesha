#include "glayersservice.h"

namespace spcGaneshaEngine {

GLayersService::GLayersService() {
}

GLayersService::~GLayersService() {
}

#pragma mark - GLayersServiceProtocol -

void GLayersService::init() {
    layerInsert = layersArray.begin();
}

void GLayersService::destroy() {
    for (std::shared_ptr<GLayer> layer:layersArray) {
        layer->onDetach();
    }
}

void GLayersService::pushLayer(std::shared_ptr<GLayer> layer) {
    layerInsert = layersArray.emplace(layerInsert, layer);
    layer->onAttach();
}

void GLayersService::pushOverlay(std::shared_ptr<GLayer> layer) {
    layersArray.emplace_back(layer);
    layer->onAttach();
}

void GLayersService::popLayer(std::shared_ptr<GLayer> layer) {
    auto it = std::find(layersArray.begin(), layersArray.end(), layer);
    if (it != layersArray.end()) {
        (*it)->onDetach();
        layersArray.erase(it);
        layerInsert--;
    }
}

void GLayersService::popOverlay(std::shared_ptr<GLayer> layer) {
    auto it = std::find(layersArray.begin(), layersArray.end(), layer);
    if (it != layersArray.end()) {
        (*it)->onDetach();
        layersArray.erase(it);
    }
}

TLayersArray::iterator GLayersService::begin() {
    return layersArray.begin();
}

TLayersArray::iterator GLayersService::end() {
    return layersArray.end();
}

#pragma mark - Routine -

};
