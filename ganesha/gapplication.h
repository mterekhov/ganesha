#ifndef SPCGANESHAENGINE_GAPPLICATION_H
#define SPCGANESHAENGINE_GAPPLICATION_H

#include "glayersservice.h"
#include "ganeshatypes.h"
#include "gevent.h"
#include "geventshell.h"
#include "gvulkanapi.h"
#include "geventsservice.h"

namespace spcGaneshaEngine {

class GApplication {
public:
    GApplication(GEventsServiceProtocol *eventsService, GLayersServiceProtocol *layerService, void *metalLayer, const GGaneshaContent& loadContent);
    virtual ~GApplication();

    void handleEvent(GEventShell& shell);
    void processRunLoop();
    void pushLayer(GLayer *layer);
    void pushOverlay(GLayer *layer);

    //  dependency injections
    GEventsServiceProtocol *eventsService;
    GLayersServiceProtocol *layerService;
    GGaneshaContent content;
};

};  //  spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GAPPLICATION_H
