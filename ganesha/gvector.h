#ifndef SPCGANESHAENGINE_GVECTOR_H
#define SPCGANESHAENGINE_GVECTOR_H

#include "ganeshatypes.h"

namespace spcGaneshaEngine {

class GVector {
public:
    GVector();
    GVector(const TFloat x, const TFloat y, const TFloat z);
    GVector(const GVector& vector);
    ~GVector();
    
    void normalize();
    GVector cross(const GVector& vector) const;
    TFloat dot(const GVector& vector) const;

    TFloat x;
    TFloat y;
    TFloat z;
};

}   //  namespace spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GVECTOR_H
