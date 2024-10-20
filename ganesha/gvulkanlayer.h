#ifndef SPCGANESHAENGINE_GVULKANLAYER_H
#define SPCGANESHAENGINE_GVULKANLAYER_H

#include "glayer.h"
#include "gvulkanapi.h"
#include "gcamera.h"

namespace spcGaneshaEngine {

/// This layer should process keyboard, mouse and window events
class GVULKANLayer : public GLayer {
public:
    GVULKANLayer(const std::string& applicationTitle, void *metalLayer, GEventsServiceProtocol *eventsService);
    virtual ~GVULKANLayer();
    
    void onAttach() override;
    void onDetach() override;
    void onUpdate() override;
    std::vector<GEventShell> onEvent(GEventShell& shell) override;

private:
    std::shared_ptr<GVULKANAPI> vulkanAPI;
    GScene scene;
    
    std::vector<GEventShell> processCameraPositionUpdate(std::shared_ptr<GEvent> event);
    std::vector<GEventShell> processCameraLookUpdate(std::shared_ptr<GEvent> event);
    std::vector<GEventShell> processLoadGundle(std::shared_ptr<GEvent> event);
    std::vector<GEventShell> processUpdateViewMatrix(std::shared_ptr<GEvent> event);
    std::vector<GEventShell> processUpdateViewport(std::shared_ptr<GEvent> event);
};

};  //  spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GVULKANLAYER_H
