#include "gganesha.h"
#include <math.h>

namespace spcGaneshaEngine {

bool GGanesha::initEngine(void *metalLayer, const uint32_t width, const uint32_t height) {
    vulkanInstance.createInstance(metalLayer, width, height);
    vulkanInstance.installIsometricView(M_PI_4, 0.1f, 100.0f);
    vulkanInstance.installViewMatrix(camera.viewMatrix());

    return true;
}

void GGanesha::destroyVulkan() {
    vulkanInstance.destroyInstance();
}

void GGanesha::setViewSize(const uint32_t width, const uint32_t height) {
    
}

void GGanesha::setView(void *pView) {
    
}

void GGanesha::processKeyboard(const uint32_t keyCode) {
    vulkanInstance.installViewMatrix(camera.viewMatrix());
}

void GGanesha::mainLoop() {
    vulkanInstance.drawFrame();
}

}
