//-----------------------------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 04 / 03
// Brief: Bounding Volume and intersection test
//-----------------------------------------------------------------------------------------
#include "glbbv.h"

#include <float.h>
#include <stdint.h>

namespace glb {

namespace math {

//-----------------------------------------------------------------------------------
// CONSTANT VALUE
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// TYPE DECLARATION
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// CLASS DECLARATION
//----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// CLASS DEFINITION
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// AABB DEFINITION
//-----------------------------------------------------------------------------------
AABB::AABB()
: m_Max(0.0f, 0.0f, 0.0f)
, m_Min(0.0f, 0.0f, 0.0f) {
}

AABB::AABB(Vector* points)
: m_Max(-FLT_MAX, -FLT_MAX, -FLT_MAX)
, m_Min(FLT_MAX, FLT_MAX, FLT_MAX) {
    for (int32_t j = 0; j < 8; j++) {
        if (points[j].x < m_Min.x) {
            m_Min.x = points[j].x;
        }
        if (points[j].x > m_Max.x) {
            m_Max.x = points[j].x;
        }

        if (points[j].y < m_Min.y) {
            m_Min.y = points[j].y;
        }
        if (points[j].y > m_Max.y) {
            m_Max.y = points[j].y;
        }

        if (points[j].z < m_Min.z) {
            m_Min.z = points[j].z;
        }
        if (points[j].z > m_Max.z) {
            m_Max.z = points[j].z;
        }
    }
}

AABB::~AABB() {
}

bool AABB::HasIntersect(const AABB& other) {
    if (m_Max.x < other.m_Min.x) return false;
    if (m_Max.y < other.m_Min.y) return false;
    if (m_Max.z < other.m_Min.z) return false;
    if (m_Min.x > other.m_Max.x) return false;
    if (m_Min.y > other.m_Max.y) return false;
    if (m_Min.z > other.m_Max.z) return false;
    return true;
}

};  // namespace math

};  // namespace glb