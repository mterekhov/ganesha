#ifndef SPCGANESHAENGINE_GVULKANLAYER_H
#define SPCGANESHAENGINE_GVULKANLAYER_H

#include <string>
#include <vector>

#include "glayer.h"
#include "gvulkanapi.h"
#include "gscene.h"

namespace spcGaneshaEngine {

/// This layer should process keyboard, mouse and window events
class GVULKANLayer : public GLayer {
public:
    GVULKANLayer(const std::string& applicationTitle, void *metalLayer, std::shared_ptr<GEventsServiceProtocol> eventsService);
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
    std::vector<GEventShell> processUpdateFrameSize(std::shared_ptr<GEvent> event);
};

};  //  spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GVULKANLAYER_H
