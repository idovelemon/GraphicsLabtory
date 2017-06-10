//---------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017/06/01
// Brief: Dynamic object implement basic primitive test
//---------------------------------------------------------
#include "dynamicobject.h"

#include <assert.h>

namespace dynamic {

//-----------------------------------------------------------------
DTAabb::DTAabb(glb::math::Vector max, glb::math::Vector min, glb::math::Vector pos)
: DynamicObject(DOT_AABB)
, m_Max(max)
, m_Min(min)
, m_Pos(pos) {
}

DTAabb::~DTAabb() {
}

bool DTAabb::IsIntersection(DynamicObject* object) {
    bool result = false;

    if (object == nullptr) return result;

    switch (object->GetType()) {
    case DOT_AABB:
        result = IsIntersectionWithAABB(reinterpret_cast<DTAabb*>(object));
        break;

    default:
        assert(false && "Unsupport dynamic object type");
    }

    return result;
}

void DTAabb::Update(glb::math::Vector max, glb::math::Vector min, glb::math::Vector center) {
    m_Max = max;
    m_Min = min;
    m_Pos = center;
}

bool DTAabb::IsIntersectionWithAABB(DTAabb* object) {
    if (object == nullptr) return false;

    glb::math::Vector center0 = (m_Max + m_Min) * 0.5f;
    glb::math::Vector diff0 = m_Pos - center0;
    glb::math::Vector max0 = m_Max + diff0;
    glb::math::Vector min0 = m_Min + diff0;
    glb::math::Vector center1 = (object->m_Max + object->m_Min) * 0.5f;
    glb::math::Vector diff1 = object->m_Pos - center1;
    glb::math::Vector max1 = object->m_Max + diff1;
    glb::math::Vector min1 = object->m_Min + diff1;

    if (max0.x < min1.x) return false;
    if (max0.y < min1.y) return false;
    if (max0.z < min1.z) return false;
    if (min0.x > max1.x) return false;
    if (min0.y > max1.y) return false;
    if (min0.z > max1.z) return false;

    return true;
}

};  // namespace dynamic