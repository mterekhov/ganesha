#ifndef SPCGANESHAENGINE_GUPDATEVIEWMATRIXEVENT_H
#define SPCGANESHAENGINE_GUPDATEVIEWMATRIXEVENT_H

#include "gevent.h"
#include "gmatrix.h"

namespace spcGaneshaEngine {

class GUpdateViewMatrixEvent: public GEvent {
public:
    GUpdateViewMatrixEvent(const GMatrix& matrix);
    virtual ~GUpdateViewMatrixEvent();
    
    const GMatrix matrix;
};

};

#endif  //  SPCGANESHAENGINE_GUPDATEVIEWMATRIXEVENT_H
