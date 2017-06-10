//---------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017/06/01
// Brief: Dynamic object implement basic primitive test
//---------------------------------------------------------
#ifndef DYNAMIC_DYNAMICOBJECT_H_
#define DYNAMIC_DYNAMICOBJECT_H_

#include "math/glbvector.h"

namespace dynamic {

//---------------------------------------------------------
// Type Declaration

//---------------------------------------------------------
enum DynamicObjectType {
    DOT_UNKOWN = -1,
    DOT_AABB,
};

//---------------------------------------------------------
class DynamicObject {
public:
    DynamicObject(DynamicObjectType type)
    : m_Type(type)
    , m_UserData(nullptr) {
    }
    virtual ~DynamicObject() {
    }

public:
    virtual DynamicObjectType GetType() const { return m_Type; }
    virtual bool IsIntersection(DynamicObject* object) = 0;
    virtual void SetUserData(void* data) { m_UserData = data; }
    virtual void* GetUserData() { return m_UserData; }

protected:
    DynamicObjectType   m_Type;
    void*               m_UserData;
};

//---------------------------------------------------------
class DTAabb : public DynamicObject {
public:
    DTAabb(glb::math::Vector max, glb::math::Vector min, glb::math::Vector pos);
    virtual ~DTAabb();

public:
    virtual bool IsIntersection(DynamicObject* object);
    virtual void Update(glb::math::Vector max, glb::math::Vector min, glb::math::Vector center);

protected:
    bool IsIntersectionWithAABB(DTAabb* aabb);

protected:
    glb::math::Vector       m_Max;
    glb::math::Vector       m_Min;
    glb::math::Vector       m_Pos;
};

};  // namespace dynamic

#endif  // DYNAMIC_DYNAMICOBJECT_H_