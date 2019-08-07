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

//-----------------------------------------------------------------------------------
// Sphere DEFINITION
//-----------------------------------------------------------------------------------
Sphere::Sphere()
: m_Center(0.0f, 0.0f, 0.0f)
, m_Radius(0.0f) {
}

Sphere::Sphere(Vector* points, int32_t num)
: m_Center(-FLT_MAX, -FLT_MAX, -FLT_MAX)
, m_Radius(FLT_MAX) {
    // Using Ritter's method to compute bound sphere
    unsigned int maxXIndex = 0, maxYIndex = 0, maxZIndex = 0, minXIndex = -1, minYIndex = -1, minZIndex = -1;
    float maxX = -FLT_MAX, minX = FLT_MAX, maxY = -FLT_MAX, minY = FLT_MAX, maxZ = -FLT_MAX, minZ = FLT_MAX;

    // Find the max and min along the x-axis, y-axis, z-axis
    for(int i = 0 ; i < num ; i ++) {
        if(points[i].x > maxX) {
            maxXIndex = i;
            maxX = points[i].x;
        }

        if(points[i].x < minX) {
            minXIndex = i;
            minX = points[i].x;
        }

        if(points[i].y > maxY) {
            maxYIndex = i;
            maxY = points[i].y;
        }

        if(points[i].y < minY) {
            minYIndex = i;
            minY = points[i].y;
        }

        if(points[i].z > maxZ) {
            maxZIndex = i;
            maxZ = points[i].z;
        }

        if(points[i].z < minZ) {
            minZIndex = i;
            minZ = points[i].z;
        }
    }

    float x = 0;
    Vector sub1, sub2;
    sub1.x = points[maxXIndex].x; sub1.y = points[maxXIndex].y; sub1.z = points[maxXIndex].z;
    sub2.x = points[minXIndex].x; sub2.y = points[minXIndex].y; sub2.z = points[minXIndex].z;
    sub1 = sub1 - sub2;
    x = Dot(sub1, sub1);

    float y = 0;
    sub1.x = points[maxYIndex].x; sub1.y = points[maxYIndex].y; sub1.z = points[maxYIndex].z;
    sub2.x = points[minYIndex].x; sub2.y = points[minYIndex].y; sub2.z = points[minYIndex].z;
    sub1 = sub1 - sub2;
    y = Dot(sub1, sub1);

    float z = 0;
    sub1.x = points[maxZIndex].x; sub1.y = points[maxZIndex].y; sub1.z = points[maxZIndex].z;
    sub2.x = points[minZIndex].x; sub2.y = points[minZIndex].y; sub2.z = points[minZIndex].z;
    sub1 = sub1 - sub2;
    z = Dot(sub1, sub1);

    float dia = x;
    int max = maxXIndex, min = minXIndex;
    if( z > x && z > y) {
        max = maxZIndex;
        min = minZIndex;
        dia = z ;
    } else if(y > x && y > z) {
        max = maxYIndex;
        min = minYIndex;
        dia = y;
    }

    // Compute the center point
    m_Center.x = 0.5 * (points[max].x + points[min].x) ;
    m_Center.y = 0.5 * (points[max].y + points[min].y) ;
    m_Center.z = 0.5 * (points[max].z + points[min].z) ;

    // Compute the radious
    m_Radius = 0.5 * sqrt(dia);

    // Fix it
    for(int i = 0 ; i < num ; i ++) {
        Vector d ;
        d = points[i] - m_Center;
        float dist2 = 0 ;
        dist2 = Dot(d, d);

        if(dist2 > m_Radius * m_Radius)
        {
            float dist = sqrt(dist2);
            float newRadious = (dist + m_Radius) * 0.5 ;
            float k = (newRadious - m_Radius) / dist ;
            m_Radius = newRadious ;
            Vector temp;
            temp = d * k;
            m_Center = m_Center + temp;
        }
    }
}

Sphere::~Sphere() {
}

bool Sphere::HasIntersect(const Sphere& other) {
    return true;
}

};  // namespace math

};  // namespace glb