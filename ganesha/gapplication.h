#ifndef SPCGANESHAENGINE_GAPPLICATION_H
#define SPCGANESHAENGINE_GAPPLICATION_H

#include "glayersservice.h"
#include "geventsservice.h"
#include "ganeshatypes.h"
#include "gevent.h"
#include "geventshell.h"
#include "gvulkanapi.h"

namespace spcGaneshaEngine {

class GApplication {
public:
    GApplication();
    virtual ~GApplication();

    void handleEvent(GEventShell& shell);
    void processRunLoop();
    void pushLayer(std::shared_ptr<GLayer> layer);
    void pushOverlay(std::shared_ptr<GLayer> layer);

    std::shared_ptr<GEventsServiceProtocol> eventsService;
    
private:
    std::shared_ptr<GLayersServiceProtocol> layerService;
};

};  //  spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GAPPLICATION_H
