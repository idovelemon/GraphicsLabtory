//---------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017/06/08
// Brief: Collision deal with collision detection with dynamic world
//---------------------------------------------------------------
#ifndef ENTITY_COLLISIONCOM_H_
#define ENTITY_COLLISIONCOM_H_

#include <stdint.h>
#include <vector>

#include "component.h"
#include "math/glbvector.h"

#include "../dynamic/dynamicobject.h"

namespace entity {

//-----------------------------------------------------------
// Pre Declaration

class Entity;

//-----------------------------------------------------------
// Type Declaration

class CollisionCom : public Component {
public:
    enum {
        PlayerFilter = 1 << 7,
        EnemyFilter = 1 << 8,
        PlayerBulletFilter = 1 << 9,
        EnemyBulletFilter = 1 << 10,
        ObstacleFilter = 1 << 11,
    };

public:
    CollisionCom(Entity* owner, float width, float height, float depth, glb::math::Vector pos);
    virtual ~CollisionCom();

public:
    virtual void Update();

    void SetCollisionHandle(dynamic::DynamicObject::CollisionEventHandle handle);
    void SetCollisionFilter(int32_t groupFilter, int32_t maskFilter);

    void GetBoundBox(float& outWidth, float& outHeight, float& outDepth);

protected:
    int32_t                     m_DynamicObjectID;
};

};  // namespace entity

#endif  // ENTITY_COLLISIONCOM_H_