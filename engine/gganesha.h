#ifndef SPCGANESHAENGINE_GGANESHA_H
#define SPCGANESHAENGINE_GGANESHA_H

#include <stdio.h>
#include "gvulkanapi.h"
#include "gcamera.h"

namespace spcGaneshaEngine {

class GGanesha {
public:
    bool initEngine(void *metalLayer, const uint32_t width, const uint32_t height);
    void destroyVulkan();

    void setViewSize(const uint32_t width, const uint32_t height);
    void setView(void *pView);
    void mainLoop();
    void processKeyboard(const uint32_t keyCode);
    
private:
    GVULKANAPI vulkanInstance;
    GCamera camera;

};

}   //  namespace spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GGANESHA_H
