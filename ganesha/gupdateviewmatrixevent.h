#ifndef SPCGANESHAENGINE_GUPDATE_VIEW_MATRIX_EVENT_H
#define SPCGANESHAENGINE_GUPDATE_VIEW_MATRIX_EVENT_H

#include "gevent.h"
#include "gmatrix.h"

namespace spcGaneshaEngine {

class GUpdateViewMatrixEvent: public GEvent {
public:
    GUpdateViewMatrixEvent(const GMatrix& matrix);
    virtual ~GUpdateViewMatrixEvent();
    
    const GMatrix matrix;
    
    const std::string about() override;
};

};

#endif  //  SPCGANESHAENGINE_GUPDATE_VIEW_MATRIX_EVENT_H
