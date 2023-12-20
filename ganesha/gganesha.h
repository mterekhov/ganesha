#ifndef SPCGANESHAENGINE_GGANESHA_H
#define SPCGANESHAENGINE_GGANESHA_H

#include <stdio.h>
#include "ggraphicsapiprotocol.h"
#include "gcamera.h"
#include "grendergraph.h"

namespace spcGaneshaEngine {

/// This class is the main entry point for engine usage. You need to create this object and provide critical info(windows size, key touches, etc)
class GGanesha {
public:
    GGanesha();
    ~GGanesha();
    
    /// Init engine
    /// @param metalLayer  layer of MTKView
    /// @param width window width
    /// @param height window height
    TBool initEngine(void *metalLayer, const TUInt width, const TUInt height);
    
    /// Destroys all engine objects
    void destroyVulkan();
    
    /// In case window size changes you can use this method to update rendering
    /// @param width new width
    /// @param height new height
    void setViewSize(const TUInt width, const TUInt height);
    
    /// Main engine loop which launches every time new frame should be generated
    void mainLoop();
    
    /// Any keyboard events should be proxied using this method
    /// @param keyCode keyboard event code
    void processKeyboard(const TUInt keyCode);
    
private:
    GGraphicsAPIProtocol *graphicsAPI;
    GCamera camera;
    GRenderGraph renderGraph;

};

}   //  namespace spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GGANESHA_H
