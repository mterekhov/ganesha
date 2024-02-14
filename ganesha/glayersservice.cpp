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
    for (GLayer * layer:layersArray) {
        layer->onDetach();
        delete layer;
    }
}

void GLayersService::pushLayer(GLayer *layer) {
    layerInsert = layersArray.emplace(layerInsert, layer);
    layer->onAttach();
}

void GLayersService::pushOverlay(GLayer *layer) {
    layersArray.emplace_back(layer);
    layer->onAttach();
}

void GLayersService::popLayer(GLayer *layer) {
    auto it = std::find(layersArray.begin(), layersArray.end(), layer);
    if (it != layersArray.end()) {
        (*it)->onDetach();
        layersArray.erase(it);
        layerInsert--;
    }
}

void GLayersService::popOverlay(GLayer *layer) {
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
