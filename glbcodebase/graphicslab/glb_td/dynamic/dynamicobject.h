//---------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017/06/01
// Brief: Dynamic object implement basic primitive test
//---------------------------------------------------------
#ifndef DYNAMIC_DYNAMICOBJECT_H_
#define DYNAMIC_DYNAMICOBJECT_H_

#include <stdint.h>

#include "math/glbvector.h"

#include "BulletCollision/CollisionDispatch/btCollisionObject.h"
#include "BulletCollision/CollisionShapes/btBoxShape.h"

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
    typedef void (*CollisionEventHandle)(DynamicObject* self, DynamicObject* other);

public:
    DynamicObject(DynamicObjectType type)
    : m_Type(type)
    , m_btCollisionShape(nullptr)
    , m_btCollision(nullptr)
    , m_UserData(nullptr)
    , m_CollisionHandle(nullptr)
    , m_CollisionFilterGroup(-1)
    , m_CollisionFilterMask(-1) {
    }
    virtual ~DynamicObject() {
    }

public:
    virtual DynamicObjectType GetType() const { return m_Type; }
    virtual void Update(glb::math::Vector pos, glb::math::Vector rot, glb::math::Vector scale) = 0;
    virtual btCollisionObject* GetBtCollision() { return m_btCollision; }
    virtual void SetUserData(void* data) { m_UserData = data; }
    virtual void* GetUserData() const { return m_UserData; }
    virtual void SetCollisionHandle(CollisionEventHandle handle) { m_CollisionHandle = handle; }
    virtual CollisionEventHandle GetCollisionHandle() const { return m_CollisionHandle; }
    virtual void SetCollisionFilter(int32_t group, int32_t mask) {
        m_CollisionFilterGroup = group;
        m_CollisionFilterMask = mask;
        if (m_btCollision) {
            m_btCollision->getBroadphaseHandle()->m_collisionFilterGroup = group;
            m_btCollision->getBroadphaseHandle()->m_collisionFilterMask = mask;
        }
    }
    virtual int32_t GetCollisionFilterGroup() const { return m_CollisionFilterGroup; }
    virtual int32_t GetCollisionFilterMask() const { return m_CollisionFilterMask; }

protected:
    DynamicObjectType       m_Type;
    btCollisionShape*       m_btCollisionShape;
    btCollisionObject*      m_btCollision;
    void*                   m_UserData;
    CollisionEventHandle    m_CollisionHandle;
    int32_t                 m_CollisionFilterGroup;
    int32_t                 m_CollisionFilterMask;
};

//---------------------------------------------------------
class DTAabb : public DynamicObject {
public:
    DTAabb(glb::math::Vector max, glb::math::Vector min, glb::math::Vector pos);
    virtual ~DTAabb();

public:
    virtual void Update(glb::math::Vector pos, glb::math::Vector rot = glb::math::Vector(0.0f, 0.0f, 0.0f), glb::math::Vector scale = glb::math::Vector(1.0f, 1.0f, 1.0f)) override;
    void GetBoundBox(glb::math::Vector& outMax, glb::math::Vector& outMin);

protected:
    glb::math::Vector       m_OriMax;
    glb::math::Vector       m_OriMin;
};

};  // namespace dynamic

#endif  // DYNAMIC_DYNAMICOBJECT_H_