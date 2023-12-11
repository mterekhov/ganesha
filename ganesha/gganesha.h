#ifndef SPCGANESHAENGINE_GGANESHA_H
#define SPCGANESHAENGINE_GGANESHA_H

#include <stdio.h>
#include "gvulkanapi.h"
#include "gcamera.h"

namespace spcGaneshaEngine {

/// This class is the main entry point for engine usage. You need to create this object and provide critical info(windows size, key touches, etc)
class GGanesha {
public:
    /// Init engine
    /// @param metalLayer  layer of MTKView
    /// @param width window width
    /// @param height window height
    bool initEngine(void *metalLayer, const uint32_t width, const uint32_t height);
    
    /// Destroys all engine objects
    void destroyVulkan();
    
    /// In case window size changes you can use this method to update rendering
    /// @param width new width
    /// @param height new height
    void setViewSize(const uint32_t width, const uint32_t height);
    
    /// Main engine loop which launches every time new frame should be generated
    void mainLoop();
    
    /// Any keyboard events should be proxied using this method
    /// @param keyCode keyboard event code
    void processKeyboard(const uint32_t keyCode);
    
private:
    GVULKANAPI vulkanInstance;
    GCamera camera;

};

}   //  namespace spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GGANESHA_H
