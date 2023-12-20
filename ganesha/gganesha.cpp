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

TBool GGanesha::initEngine(void *metalLayer, const TUInt width, const TUInt height) {
    
    const std::vector<Vertex> vertexes = {
        {GPoint2D(-0.5f, -0.5f), GColor::blueColor()},
        {GPoint2D(0.5f, -0.5f), GColor::redColor()},
        {GPoint2D(0.5f, 0.5f), GColor::greenColor()},
        {GPoint2D(-0.5f, 0.5f), GColor::redColor()}
    };
    renderGraph.defineVertexesArray(vertexes);

    const TIndexArray indexes = {
        0, 1, 2, 2, 3, 0
    };
    renderGraph.defineIndexesArray(indexes);
    
    graphicsAPI->initAPI(metalLayer, width, height, renderGraph);
    graphicsAPI->installIsometricView(M_PI_4, 0.1f, 100.0f);
    graphicsAPI->installViewMatrix(camera.viewMatrix());

    return true;
}

void GGanesha::destroyVulkan() {
    graphicsAPI->destroyAPI();
}

void GGanesha::setViewSize(const TUInt width, const TUInt height) {
    
}

void GGanesha::processKeyboard(const TUInt keyCode) {
    graphicsAPI->installViewMatrix(camera.viewMatrix());
}

void GGanesha::mainLoop() {
    graphicsAPI->drawFrame(renderGraph);
}

}
