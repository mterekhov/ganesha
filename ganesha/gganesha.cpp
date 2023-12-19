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
    
    const std::vector<Vertex> vertices = {
        {GPoint2D(-0.5f, -0.5f), GColor::blueColor()},
        {GPoint2D(0.5f, -0.5f), GColor::redColor()},
        {GPoint2D(0.5f, 0.5f), GColor::greenColor()},
        {GPoint2D(-0.5f, 0.5f), GColor::redColor()}
    };
    renderGraph.defineVertecesArray(vertices);

    const std::vector<uint16_t> indices = {
        0, 1, 2, 2, 3, 0
    };
    renderGraph.defineIndecesArray(indices);
    
    graphicsAPI->initAPI(metalLayer, width, height, renderGraph);
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
    graphicsAPI->drawFrame(renderGraph);
}

}
