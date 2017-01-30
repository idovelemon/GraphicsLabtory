//-----------------------------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016/06/10
// Brief: Define the matrix math tools
//-----------------------------------------------------------------------------------------
#ifndef GLB_GLBMATRIX_H_
#define GLB_GLBMATRIX_H_

#include <math.h>
#include <memory.h>
#include <stdint.h>

#include "glbmacro.h"
#include "glbvector.h"

namespace glb {

class Vector;

class Matrix {
 public:
    inline Matrix();
    inline ~Matrix();  // Do not need virtual

 public:
    //----------------------------------------------------------------------
    // @brief: These method will make the matrix become scale,
    // translate and rotation matrix
    //----------------------------------------------------------------------
    inline void MakeScaleMatrix(float sx, float sy, float sz);
    inline void MakeTranslateMatrix(float tx, float ty, float tz);
    inline void MakeRotateXMatrix(float rotate_degree);
    inline void MakeRotateYMatrix(float rotate_degree);
    inline void MakeRotateZMatrix(float rotate_degree);
    inline void MakeRotateZXYMatrix(float rx, float ry, float rz);
    inline void MakeIdentityMatrix();
    inline void MakeRotateAxisMatrix(Vector axix, float rotate_degree);

    //----------------------------------------------------------------------
    // @brief: Build projection and view matrix
    //----------------------------------------------------------------------
    inline void MakeProjectionMatrix(float aspect, float fov, float near, float far);
    inline void MakeProjectionMatrix(float left, float right, float bottom, float top, float near, float far);
    inline void MakeOrthogonalMatrix(float left, float right, float bottom, float top, float near, float far);
    inline void MakeViewMatrix(Vector pos, Vector target);

    //----------------------------------------------------------------------
    // @brief: These method will multiply a scale, translate and rotate matrix
    //----------------------------------------------------------------------
    inline void Scale(float sx, float sy, float sz);
    inline void Translate(float tx, float ty, float tz);
    inline void RotateX(float rotate_degree);
    inline void RotateY(float rotate_degree);
    inline void RotateZ(float rotate_degree);
    inline void RotateZXY(float rx, float ry, float rz);

    //----------------------------------------------------------------------
    // @brief: Multiply a matrix and save result
    //----------------------------------------------------------------------
    inline void Mul(const Matrix& m);

    //----------------------------------------------------------------------
    // @brief: Transpose the matrix and save result
    //----------------------------------------------------------------------
    inline void Transpose();

    //----------------------------------------------------------------------
    // @brief: Inverse the matrix and save result
    //----------------------------------------------------------------------
    inline void Inverse();

    //----------------------------------------------------------------------
    // @brief: Override operator
    //----------------------------------------------------------------------
    inline const Vector operator*(const Vector& v);

    //----------------------------------------------------------------------
    // @brief: Return the data of the matrix
    //----------------------------------------------------------------------
    inline const float* GetData();
    inline float GetElement(int32_t x, int32_t y);

 private:
    union {
        float m[4][4];
        float v[16];
    } m_Matrix;
};

};  // namespace glb

#include "glbmatrix.cpp.h"

#endif  // GLB_GLBMATRIX_H_
