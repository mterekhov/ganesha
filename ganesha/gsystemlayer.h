#ifndef SPCGANESHAENGINE_GSYSTEMLAYER_H
#define SPCGANESHAENGINE_GSYSTEMLAYER_H

#include <vector>

#include "glayer.h"

namespace spcGaneshaEngine {

/// This layer should process keyboard, mouse and window events
class GSystemLayer : public GLayer {
public:
    GSystemLayer(std::shared_ptr<GEventsServiceProtocol> eventsService);
    virtual ~GSystemLayer();

    std::vector<GEventShell> onEvent(GEventShell& shell) override;
    
private:
    std::vector<GEventShell> processKeyboard(std::shared_ptr<GEvent> event);
    std::vector<GEventShell> processMouseMove(std::shared_ptr<GEvent> event);
    std::vector<GEventShell> processWindowResize(std::shared_ptr<GEvent> event);
};

};  //  spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GSYSTEMLAYER_H
