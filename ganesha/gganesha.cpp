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
        { GPoint(-0.5f, 0.2, -0.5f), GPoint2D{2.0f, 0.0f} },
        { GPoint(0.5f, 0.2, -0.5f), GPoint2D{0.0f, 0.0f} },
        { GPoint(0.5f, 0.2, 0.5f), GPoint2D{0.0f, 2.0f} },
        { GPoint(-0.5f, 0.2, 0.5f), GPoint2D{2.0f, 2.0f} },

        { GPoint(-10.0f, 0.0, -10.0f), GPoint2D{10.0f, 0.0f} },
        { GPoint(10.0f, 0.0, -10.0f), GPoint2D{0.0f, 0.0f} },
        { GPoint(10.0f, 0.0, 10.0f), GPoint2D{0.0f, 10.0f} },
        { GPoint(-10.0f, 0.0, 10.0f), GPoint2D{10.0f, 10.0f} },
        
        { GPoint(-0.5f, 0.1, -0.5f), GPoint2D{2.0f, 0.0f} },
        { GPoint(0.5f, 0.1, -0.5f), GPoint2D{0.0f, 0.0f} },
        { GPoint(0.5f, 0.1, 0.5f), GPoint2D{0.0f, 2.0f} },
        { GPoint(-0.5f, 0.1, 0.5f), GPoint2D{2.0f, 2.0f} }
    };
    renderGraph.defineVertexesArray(vertexes);

    const TIndexArray indexes = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4,
        8, 9, 10, 10, 11, 8
    };
    renderGraph.defineIndexesArray(indexes);
    
    graphicsAPI->initAPI(metalLayer, width, height, renderGraph);
    graphicsAPI->installIsometricView(M_PI_4, 0.0001f, 100.0f);
    graphicsAPI->installViewMatrix(camera.viewMatrix());

    return true;
}

void GGanesha::destroyVulkan() {
    graphicsAPI->destroyAPI();
}

void GGanesha::setViewSize(const TUInt width, const TUInt height) {
    graphicsAPI->frameResized(width, height);
}

void GGanesha::processKeyboard(const TUInt keyCode) {
    printf("key %i\n", keyCode);
    TFloat speed = 0.5;
    switch (keyCode) {
        case 14:
            camera.updownCamera(-speed);
            //        print("e")
        case 12:
            camera.updownCamera(speed);
            //        print("q")
        case 13:
            camera.moveCamera(-speed);
            //        print("w")
        case 1:
            camera.moveCamera(speed);
            //        print("s")
        case 0:
            camera.strafeCamera(speed);
            //        print("a")
        case 2:
            camera.strafeCamera(-speed);
            //        print("d")
        default:
            break;
    }
    graphicsAPI->installViewMatrix(camera.viewMatrix());
}

void GGanesha::mainLoop() {
    graphicsAPI->drawFrame(renderGraph);
}

}
