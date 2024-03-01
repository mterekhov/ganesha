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
    
    void onAttach() override;
    void onDetach() override;
    void onUpdate() override;
    std::vector<GEventShell> onEvent(GEventShell& shell) override;

private:
    GGraphicsAPIProtocol *vulkanAPI;
    void* metalLayer;
    GCamera camera;
    
    std::vector<GEventShell> processUpdateViewMatrix(GEvent *event);
    std::vector<GEventShell> processUpdateViewport(GEvent *event);
};

};  //  spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GVULKANLAYER_H
