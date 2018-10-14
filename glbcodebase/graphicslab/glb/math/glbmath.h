//-----------------------------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 04 / 03
// Brief: Math tool for GraphicsLab
//-----------------------------------------------------------------------------------------
#ifndef GLB_GLBMATH_H_
#define GLB_GLBMATH_H_

#include "glbmatrix.h"
#include "glbvector.h"

namespace glb {

namespace math {

//--------------------------------------------------------------------------------
// @brief: Calculate pick ray from normalized screen position (range from 0.0 to 1.0)
// @param: x The normalized screen position x
// @param: y The normalized screen position y
// @param: viewMat View Matrix
// @param: projMat Projection Matrix
// @return: s The pick ray's start point
// @return: e The pick ray's end point
//--------------------------------------------------------------------------------
void CalcPickRay(float x, float y, Matrix& viewMat, Matrix& projMat, Vector& s, Vector& e);

//--------------------------------------------------------------------------------
// @brief: Caculate the intersection point between plane and segment
// @param: p The point on plane
// @param: n The normal of plane
// @param: s The start point of segment
// @param: e The end point of segment
// @return: r The intersection point
// @return: Return true if intersected, otherwise return false
//--------------------------------------------------------------------------------
bool Intersection(Vector p, Vector n, Vector s, Vector e, Vector& r);

};

};  // end namespace math

#endif  // GLB_GLBMATH_H_