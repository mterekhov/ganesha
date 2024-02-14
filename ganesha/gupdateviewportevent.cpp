#include "gupdateviewportevent.h"

namespace spcGaneshaEngine {

GUpdateViewportEvent::GUpdateViewportEvent(const GViewport& viewport) : GEvent(EVENT_TYPE_VULKAN_UPDATE_VIEWPORT), viewport(viewport) {
    
}

GUpdateViewportEvent::~GUpdateViewportEvent() {
    
}

}
