#ifndef SPCGANESHAENGINE_GQUATERNION_H
#define SPCGANESHAENGINE_GQUATERNION_H

#include "ganeshatypes.h"
#include "gvector.h"
#include "gmatrix.h"

namespace spcGaneshaEngine {

class GQuaternion {
public:
    GQuaternion();
    GQuaternion(const GVector& axis, const TFloat angle);
    ~GQuaternion();
    
    const GQuaternion& operator *=(const GQuaternion &q);
    
    GQuaternion euler(const TFloat pitch, const TFloat yaw, const TFloat roll) const;
    GMatrix matrix() const;
    TFloat euler_x() const;
    TFloat euler_y() const;
    TFloat euler_z() const;

    TFloat x;
    TFloat y;
    TFloat z;
    TFloat w;
};

}   //  namespace spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GQUATERNION_H
