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
        { GPoint(-0.5f, -0.3, -0.5f), GPoint2D{2.0f, 0.0f} },
        { GPoint(0.5f, -0.3, -0.5f), GPoint2D{0.0f, 0.0f} },
        { GPoint(0.5f, -0.3, 0.5f), GPoint2D{0.0f, 2.0f} },
        { GPoint(-0.5f, -0.3, 0.5f), GPoint2D{2.0f, 2.0f} },

        { GPoint(0.0f, -0.1, 0.0f), GPoint2D{2.0f, 0.0f} },
        { GPoint(1.0f, -0.1, 0.0f), GPoint2D{0.0f, 0.0f} },
        { GPoint(1.0f, -0.1, 1.0f), GPoint2D{0.0f, 2.0f} },
        { GPoint(0.0f, -0.1, 1.0f), GPoint2D{2.0f, 2.0f} },

        { GPoint(-2.5f, 0.0f, -2.5f), GPoint2D{2.0f, 0.0f} },
        { GPoint(2.5f, 0.0f, -2.5f), GPoint2D{0.0f, 0.0f} },
        { GPoint(2.5f, 0.0f, 2.5f), GPoint2D{0.0f, 2.0f} },
        { GPoint(-2.5f, 0.0f, 2.5f), GPoint2D{2.0f, 2.0f} }
    };
    renderGraph.defineVertexesArray(vertexes);

    const TIndexArray indexes = {
        2, 1, 0, 0, 3, 2,
        6, 5, 4, 4, 7, 6,
        10, 9, 8, 8, 11, 10
    };
    renderGraph.defineIndexesArray(indexes);
    
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
