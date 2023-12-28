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
    
    /// Normalizes vector
    void normalize();
    
    /// Calculates length of vector
    TFloat magnitude() const;
    
    /// Cross product of current vector and vector in parameter. It will be new vector which is perpendicular to other two
    /// - Parameter vector: second vector
    GVector cross(const GVector& vector) const;
    
    /// Projection of one vector to another, result is always scalar
    /// - Parameter vector: vector for projection
    TFloat dot(const GVector& vector) const;
    
    TFloat angleBetween(GVector vector);
    void rotate(const TFloat angle, const GVector& aroundVector);

    TFloat x;
    TFloat y;
    TFloat z;
};

}   //  namespace spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GVECTOR_H
