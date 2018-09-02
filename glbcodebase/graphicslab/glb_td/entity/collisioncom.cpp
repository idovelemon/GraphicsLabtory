//---------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017/06/08
// Brief: Collision deal with collision detection with dynamic world
//---------------------------------------------------------------
#include "collisioncom.h"

#include <assert.h>

#include "entity.h"
#include "rendercom.h"
#include "transformcom.h"
#include "../dynamic/dynamicobject.h"
#include "../dynamic/dynamicworld.h"

namespace entity {

CollisionCom::CollisionCom(Entity* owner, float width, float height, float depth, glb::math::Vector pos)
: Component(CT_COLLISION, owner)
, m_DynamicObjectID(-1) {
    dynamic::DTAabb* aabb = new dynamic::DTAabb(glb::math::Vector(width / 2.0f, height / 2.0f, depth / 2.0f)
        , glb::math::Vector(-width / 2.0f, -height / 2.0f, -depth / 2.0f)
        , pos);
    aabb->SetUserData(reinterpret_cast<void*>(owner->GetID()));
    m_DynamicObjectID = dynamic::DynamicWorld::AddDynamicObject(aabb);
}

CollisionCom::~CollisionCom() {
    dynamic::DynamicWorld::RemoveDynamicObject(m_DynamicObjectID);
    m_DynamicObjectID = -1;
}

void CollisionCom::Update() {
    entity::TransformCom* com = reinterpret_cast<entity::TransformCom*>(m_Entity->GetComponent(entity::CT_TRANSFORM));
    if (com) {
        reinterpret_cast<dynamic::DTAabb*>(dynamic::DynamicWorld::GetDynamicObject(m_DynamicObjectID))->Update(com->GetPos(), com->GetRotate(), com->GetScale());
    }
}

void CollisionCom::SetCollisionHandle(dynamic::DynamicObject::CollisionEventHandle handle) {
    dynamic::DynamicWorld::GetDynamicObject(m_DynamicObjectID)->SetCollisionHandle(handle);
}

void CollisionCom::SetCollisionFilter(int32_t groupFilter, int32_t maskFilter) {
    dynamic::DynamicWorld::GetDynamicObject(m_DynamicObjectID)->SetCollisionFilter(groupFilter, maskFilter);
}

void CollisionCom::GetBoundBox(float& outWidth, float& outHeight, float& outDepth) {
    glb::math::Vector maxPos, minPos;
    reinterpret_cast<dynamic::DTAabb*>(dynamic::DynamicWorld::GetDynamicObject(m_DynamicObjectID))->GetBoundBox(maxPos, minPos);
    outWidth = maxPos.x - minPos.x;
    outHeight = maxPos.y - minPos.y;
    outDepth = maxPos.z - minPos.z;
}

};  // namespace entity