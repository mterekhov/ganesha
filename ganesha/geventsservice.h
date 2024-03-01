#ifndef SPCGANESHAENGINE_GEVENTSSERVICE_H
#define SPCGANESHAENGINE_GEVENTSSERVICE_H

#include "geventshell.h"
#include "gmatrix.h"
#include "gviewport.h"

namespace spcGaneshaEngine {

class GEventsServiceProtocol {
public:
    virtual void init() = 0;
    virtual void destroy() = 0;
  
    virtual void markAsHandled(GEventShell& shell) = 0;
    virtual TBool doesHandled(GEventShell& shell) = 0;
    virtual GEventShell mouseEvent(const TFloat position_x, const TFloat position_y) = 0;
    virtual GEventShell keyboardEvent(const TUInt keyCode) = 0;
    virtual GEventShell windowEvent(const TFloat width, const TFloat height) = 0;
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
    GEventShell windowEvent(const TFloat width, const TFloat height) override;
    GEventShell updateViewMatrixEvent(const GMatrix& matrix) override;
    GEventShell updateViewportEvent(const GViewport& viewport) override;
};

};  //  spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GEVENTSSERVICE_H
