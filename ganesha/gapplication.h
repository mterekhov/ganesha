#ifndef SPCGANESHAENGINE_GAPPLICATION_H
#define SPCGANESHAENGINE_GAPPLICATION_H

#include "glayersservice.h"
#include "ganeshatypes.h"
#include "gevent.h"
#include "geventshell.h"
#include "gganesha.h"
#include "gvulkanapi.h"
#include "geventsservice.h"

namespace spcGaneshaEngine {

class GApplication {
public:
    GApplication(void *metalLayer, GGaneshaContent& content);
    virtual ~GApplication();

    void handleEvent(GEventShell shell);
    void processRunLoop();
    void pushLayer(GLayer *layer);
    void pushOverlay(GLayer *layer);
    
    GEventsServiceProtocol *eventsService;

private:
    GLayersServiceProtocol *layerService;
    GGraphicsAPIProtocol *vulkanAPI;
    GGaneshaContent content;
};

};  //  spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GAPPLICATION_H
