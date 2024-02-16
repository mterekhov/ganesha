#ifndef SPCGANESHAENGINE_GVULKANLAYER_H
#define SPCGANESHAENGINE_GVULKANLAYER_H

#include "glayer.h"
#include "gvulkanapi.h"
#include "gcamera.h"

namespace spcGaneshaEngine {

/// This layer should process keyboard, mouse and window events
class GVULKANLayer : public GLayer {
public:
    GVULKANLayer(void *metalLayer, GGaneshaContent& content, GEventsServiceProtocol *eventsService);
    virtual ~GVULKANLayer();
    
    virtual std::vector<GEventShell> onEvent(GEventShell& shell);
    virtual void onAttach();
    virtual void onDetach();
    virtual void onUpdate();
    
private:
    GGraphicsAPIProtocol *vulkanAPI;
    void* metalLayer;
    GCamera camera;
    
    std::vector<GEventShell> processUpdateViewMatrix(GEvent *event);
    std::vector<GEventShell> processUpdateFrameSize(GEvent *event);
};

};  //  spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GVULKANLAYER_H
