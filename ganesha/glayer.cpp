#include "glayer.h"

namespace spcGaneshaEngine {

GLayer::GLayer(const std::string& name, GEventsServiceProtocol *eventsService) : name(name), eventsService(eventsService) {
}

GLayer::~GLayer() {
}

void GLayer::onAttach() {
}

void GLayer::onDetach() {
}

void GLayer::onUpdate() {
}

std::vector<GEventShell> GLayer::onEvent(GEventShell& shell) {
    return std::vector<GEventShell>();
}

std::string GLayer::getName() {
    return name;
}

};
