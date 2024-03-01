#include "gupdateviewportevent.h"

namespace spcGaneshaEngine {

GUpdateViewportEvent::GUpdateViewportEvent(const GViewport& viewport) : GEvent(EVENT_TYPE_VULKAN_UPDATE_VIEWPORT), viewport(viewport) {
}

GUpdateViewportEvent::~GUpdateViewportEvent() {
    
}

const std::string GUpdateViewportEvent::about() {
    std::string aboutString = "GUpdateViewportEvent new viewport:\n";

    aboutString += "width = ";
    aboutString += std::to_string(viewport.width);
    aboutString += "\nheight = ";
    aboutString += std::to_string(viewport.height);
    aboutString += "\nfov = ";
    aboutString += std::to_string(viewport.fov);
    aboutString += "\nfar = ";
    aboutString += std::to_string(viewport.far);
    aboutString += "\nnear = ";
    aboutString += std::to_string(viewport.near);

    return aboutString;
}

}
