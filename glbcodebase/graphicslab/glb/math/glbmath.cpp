//-----------------------------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 04 / 03
// Brief: Math tool for GraphicsLab
//-----------------------------------------------------------------------------------------
#include "glbmath.h"

namespace glb {

namespace math {

void CalcPickRay(float x, float y, Matrix& viewMat, Matrix& projMat, Vector& s, Vector& e) {
    // Transform into NDC space
    s = Vector((x - 0.5f) * 2.0f, (1.0f - y - 0.5f) * 2.0f, 0.0f, 1.0f);
    e = Vector((x - 0.5f) * 2.0f, (1.0f - y - 0.5f) * 2.0f, 1.0f, 1.0f);

    // Transform into view space
    Matrix invProjMat = Matrix::Inverse(projMat);
    s = invProjMat * s;
    e = invProjMat * e;

    // Transform into world space
    Matrix invViewMat = Matrix::Inverse(viewMat);
    s = invViewMat * s;
    e = invViewMat * e;

    s.x = s.x / s.w;
    s.y = s.y / s.w;
    s.z = s.z / s.w;
    s.w = 1.0f;

    e.x = e.x / e.w;
    e.y = e.y / e.w;
    e.z = e.z / e.w;
    e.w = 1.0f;
}

bool Intersection(Vector p, Vector n, Vector s, Vector e, Vector& r) {
    // Calculate ray direction
    Vector d = e - s;
    float l = d.Length();
    d.Normalize();

    float dn = Dot(d, n);
    if (abs(dn) < 0.00001f) dn = 1.0f;  // Avoid divide zero problem

    float t = (Dot(p, n) - Dot(s, n)) / dn;

    bool result = false;
    if (t <= l) {
        result = true;
        r = s + d * t;
    }

    return result;
}

};

};  // end namespace math