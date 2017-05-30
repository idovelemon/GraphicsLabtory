//-------------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 06 / 10
// Brief: Define the vector tools
//-------------------------------------------------------------------------
#ifndef GLB_GLBVECTOR_H_
#define GLB_GLBVECTOR_H_

#include <math.h>

namespace glb {

namespace math {

class Vector {
 public:
    inline Vector();
    inline Vector(float x, float y, float z, float w = 1.0f);
    inline ~Vector();  // Ignore virtual

    inline void Set(float x, float y, float z, float w = 1.0f);

    inline Vector operator-();
    inline Vector operator+(const Vector& v);
    inline Vector operator-(const Vector& v);
    inline Vector operator*(float v);
    inline const Vector& operator+=(const Vector& v);
    inline const Vector& operator/=(float v);
    inline const Vector& operator*=(float v);

    inline float Length();
    inline float SqureLength();
    inline void Normalize();

 public:
    float x;
    float y;
    float z;
    float w;
};

static const Vector Cross(const Vector& v1, const Vector& v2);
static float Dot(const Vector& v1, const Vector& v2);
static Vector CalcRotateByLookVecZXY(const Vector& v);

#include "glbvector.inl"

};  // namespace math

};  // namespace glb

#endif  //  GLB_GLBVECTOR_H_
