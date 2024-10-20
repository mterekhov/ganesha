#ifndef SPCGANESHAENGINE_GEVENTSSERVICE_H
#define SPCGANESHAENGINE_GEVENTSSERVICE_H

#include "geventshell.h"
#include "gmatrix.h"
#include "gviewport.h"
#include "gpoint2d.h"
#include "gpoint.h"
#include "gvector.h"
#include "gcameradata.h"

namespace spcGaneshaEngine {

class GEventsServiceProtocol {
public:
    virtual void init() = 0;
    virtual void destroy() = 0;
  
    virtual void markAsHandled(GEventShell& shell) = 0;
    virtual TBool doesHandled(GEventShell& shell) = 0;

    //  System layer events
    virtual GEventShell mouseEvent(const TFloat position_x, const TFloat position_y) = 0;
    virtual GEventShell keyboardEvent(const TUInt keyCode) = 0;
    
    //  VULKAN layer events
    virtual GEventShell loadGundle(const std::string& gundleFilePath) = 0;
    virtual GEventShell cameraLookUpdate(const GPoint2D& lookUpdate) = 0;
    virtual GEventShell cameraPositionUpdate(const EMovementDirection moveDirection) = 0;
    virtual GEventShell updateViewMatrixEvent(const GMatrix& matrix) = 0;
    virtual GEventShell updateViewportEvent(const GViewport& viewport) = 0;
};

class GEventsService: public GEventsServiceProtocol {
public:
    GEventsService();
    virtual ~GEventsService();
    
    void init() override;
    void destroy() override;
    
    void markAsHandled(GEventShell& shell) override;
    TBool doesHandled(GEventShell& shell) override;
    GEventShell mouseEvent(const TFloat position_x, const TFloat position_y) override;
    GEventShell keyboardEvent(const TUInt keyCode) override;

    GEventShell loadGundle(const std::string& gundleFilePath) override;
    GEventShell cameraLookUpdate(const GPoint2D& lookUpdate) override;
    GEventShell cameraPositionUpdate(const EMovementDirection moveDirection) override;
    GEventShell updateViewMatrixEvent(const GMatrix& matrix) override;
    GEventShell updateViewportEvent(const GViewport& viewport) override;
};

};  //  spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GEVENTSSERVICE_H
