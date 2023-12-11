#include "gganesha.h"
#include "gvulkanapi.h"
#include <math.h>

namespace spcGaneshaEngine {

GGanesha::GGanesha() {
    graphicsAPI = new GVULKANAPI();
}

GGanesha::~GGanesha() {
    delete graphicsAPI;
}

bool GGanesha::initEngine(void *metalLayer, const uint32_t width, const uint32_t height) {
    graphicsAPI->initAPI(metalLayer, width, height);
    graphicsAPI->installIsometricView(M_PI_4, 0.1f, 100.0f);
    graphicsAPI->installViewMatrix(camera.viewMatrix());

    return true;
}

void GGanesha::destroyVulkan() {
    graphicsAPI->destroyAPI();
}

void GGanesha::setViewSize(const uint32_t width, const uint32_t height) {
    
}

void GGanesha::processKeyboard(const uint32_t keyCode) {
    graphicsAPI->installViewMatrix(camera.viewMatrix());
}

void GGanesha::mainLoop() {
    graphicsAPI->drawFrame();
}

}
