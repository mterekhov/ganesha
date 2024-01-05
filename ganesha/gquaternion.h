#ifndef SPCGANESHAENGINE_GQUATERNION_H
#define SPCGANESHAENGINE_GQUATERNION_H

#include "ganeshatypes.h"
#include "gvector.h"
#include "gmatrix.h"

namespace spcGaneshaEngine {

class GQuaternion {
public:
    GQuaternion(const GVector& axis, const TFloat angle);
    ~GQuaternion();
    
    const GQuaternion& operator *=(const GQuaternion &q);
    GMatrix matrix() const;

    TFloat x;
    TFloat y;
    TFloat z;
    TFloat w;
};

}   //  namespace spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GQUATERNION_H
