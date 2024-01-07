#include "gganesha.h"
#include "gvulkanapi.h"
#include <math.h>

namespace spcGaneshaEngine {

GGanesha::GGanesha() : log("Ganesha"), camera(log) {
    graphicsAPI = new GVULKANAPI(log);
}

GGanesha::~GGanesha() {
    delete graphicsAPI;
}

TBool GGanesha::initEngine(void *metalLayer, const TUInt width, const TUInt height) {    
    graphicsAPI->initAPI(metalLayer, width, height, renderGraph);
    graphicsAPI->installIsometricView(M_PI_4, 0.000001, 100.0f);
    graphicsAPI->installViewMatrix(camera.viewMatrix());

    return true;
}

void GGanesha::destroyVulkan() {
    graphicsAPI->destroyAPI();
}

void GGanesha::setViewSize(const TUInt width, const TUInt height) {
    graphicsAPI->frameResized(width, height);
}

void GGanesha::processMouseMove(const TFloat diff_x, const TFloat diff_y) {
    camera.mouseCamera(diff_x, diff_y);
    graphicsAPI->installViewMatrix(camera.viewMatrix());
}

void GGanesha::processKeyboard(const TUInt keyCode) {
    switch (keyCode) {
        case 14:
            camera.downCamera();
            break;
        case 12:
            camera.upCamera();
            break;
        case 13:
            camera.forwardCamera();
            break;
        case 1:
            camera.backwardCamera();
            break;
        case 0:
            camera.strafeLeftCamera();
            break;
        case 2:
            camera.strafeRightCamera();
            break;
        default:
            break;
    }
    graphicsAPI->installViewMatrix(camera.viewMatrix());
}

void GGanesha::mainLoop() {
    graphicsAPI->drawFrame(renderGraph);
}

}
