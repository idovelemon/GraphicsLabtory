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
#include "../dynamic/dynamicobject.h"
#include "../dynamic/dynamicworld.h"

namespace entity {

CollisionCom::CollisionCom(Entity* owner)
: Component(CT_COLLISION, owner)
, m_DynamicObjectID(-1)
, m_Iterate(-1) {
    dynamic::DTAabb* object = new dynamic::DTAabb(glb::math::Vector(0.0f, 0.0f, 0.0f), glb::math::Vector(0.0f, 0.0f, 0.0f), glb::math::Vector(0.0f, 0.0f, 0.0f));
    m_DynamicObjectID = dynamic::DynamicWorld::AddDynamicObject(object);
    object->SetUserData(reinterpret_cast<void*>(m_Entity->GetID()));
}

CollisionCom::~CollisionCom() {
    dynamic::DynamicWorld::RemoveDynamicObject(m_DynamicObjectID);
    m_DynamicObjectID = -1;
}

void CollisionCom::Update() {
    RenderCom* com = reinterpret_cast<RenderCom*>(m_Entity->GetComponent(CT_RENDER));
    if (com != NULL) {
        glb::math::Vector max = com->GetBoundBoxMax();
        glb::math::Vector min = com->GetBoundBoxMin();
        glb::math::Vector center = (max + min) * 0.5f;
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

};  // namespace entity