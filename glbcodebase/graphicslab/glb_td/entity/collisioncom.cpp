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

CollisionCom::CollisionCom(Entity* owner)
: Component(CT_COLLISION, owner)
, m_DynamicObjectID(-1)
, m_Iterate(-1)
, m_OriMax(0.0f, 0.0f, 0.0f)
, m_OriMin(0.0f, 0.0f, 0.0f) {
    dynamic::DTAabb* object = new dynamic::DTAabb(glb::math::Vector(0.0f, 0.0f, 0.0f), glb::math::Vector(0.0f, 0.0f, 0.0f), glb::math::Vector(0.0f, 0.0f, 0.0f));
    m_DynamicObjectID = dynamic::DynamicWorld::AddDynamicObject(object);
    object->SetUserData(reinterpret_cast<void*>(m_Entity->GetID()));

    RenderCom* com = reinterpret_cast<RenderCom*>(m_Entity->GetComponent(CT_RENDER));
    if (com != NULL) {
        m_OriMax = com->GetBoundBoxMax();
        m_OriMin = com->GetBoundBoxMin();
    } else {
        assert(false && "Must have render component");
    }
}

CollisionCom::~CollisionCom() {
    dynamic::DynamicWorld::RemoveDynamicObject(m_DynamicObjectID);
    m_DynamicObjectID = -1;
}

void CollisionCom::Update() {
    TransformCom* com = reinterpret_cast<TransformCom*>(m_Entity->GetComponent(CT_TRANSFORM));
    if (com != NULL) {
        glb::math::Vector cur_pos = com->GetPos();
        glb::math::Vector cur_scale = com->GetScale();
        glb::math::Vector center = (m_OriMax + m_OriMin) * 0.5f;
        center = center + cur_pos;
        glb::math::Vector max = m_OriMax * cur_scale + cur_pos;
        glb::math::Vector min = m_OriMin * cur_scale + cur_pos;
        dynamic::DTAabb* aabb = reinterpret_cast<dynamic::DTAabb*>(dynamic::DynamicWorld::GetDynamicObject(m_DynamicObjectID));
        if (aabb != NULL) {
            aabb->Update(max - center, min - center, center);
        }
    } else {
        assert(false && "Must have render component");
    }
}

void CollisionCom::CheckCollision() {
    dynamic::DynamicWorld::CheckCollision(m_DynamicObjectID, m_CollIDs);
}

void CollisionCom::BeginIterate() {
    m_Iterate = 0;
}

int CollisionCom::Iterate() {
    int result = -1;
    int size = m_CollIDs.size();
    if (m_Iterate < size) {
        result = m_CollIDs[m_Iterate];
        m_Iterate++;
    }
    return result;
}

void CollisionCom::EndIterate() {
    m_Iterate = 0;
}

float CollisionCom::GetWidth() {
    float result = 0.0f;

    dynamic::DynamicObject* object = dynamic::DynamicWorld::GetDynamicObject(m_DynamicObjectID);
    if (object != NULL) {
        result = object->GetWidth();
    }

    return result;
}

float CollisionCom::GetLength() {
    float result = 0.0f;

    dynamic::DynamicObject* object = dynamic::DynamicWorld::GetDynamicObject(m_DynamicObjectID);
    if (object != NULL) {
        result = object->GetLength();
    }

    return result;
}

};  // namespace entity