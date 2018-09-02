//-------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2018. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2018/08/18
// Brief: Player's collision handle
//-------------------------------------------------------------
#include "../entity.h"
#include "../entitymgr.h"

#include <assert.h>

#include "../../gametimer.h"
#include "../../log.h"
#include "../../pyscript/pyscriptmgr.h"
#include "../../pyscript/topython_function.h"
#include "../collisioncom.h"
#include "../datacom.h"
#include "../transformcom.h"
#include "collision.h"

#include "glb.h"

namespace entity {

void EntityPlayerOnCollisionWithWall(Entity* player, Entity* wall) {
    TransformCom* playerTransform = reinterpret_cast<TransformCom*>(player->GetComponent(CT_TRANSFORM));
    TransformCom* wallTransform = reinterpret_cast<TransformCom*>(wall->GetComponent(CT_TRANSFORM));
    CollisionCom* playerCollision = reinterpret_cast<CollisionCom*>(player->GetComponent(CT_COLLISION));
    CollisionCom* wallCollision = reinterpret_cast<CollisionCom*>(wall->GetComponent(CT_COLLISION));
    if (!playerTransform || !wallTransform || !playerCollision || !wallCollision) return;

    glb::math::Vector playerCollisionSize, wallCollisionSize;
    playerCollision->GetBoundBox(playerCollisionSize.x, playerCollisionSize.y, playerCollisionSize.z);
    wallCollision->GetBoundBox(wallCollisionSize.x, wallCollisionSize.y, wallCollisionSize.z);

    glb::math::Vector playerPos = playerTransform->GetPos();
    glb::math::Vector wallPos = wallTransform->GetPos();
    glb::math::Vector collisionVec = playerPos - wallPos;
    collisionVec.y = 0.0f;  // Only consider xz-plane
    collisionVec.x = collisionVec.x / wallCollisionSize.x;
    collisionVec.z = collisionVec.z / wallCollisionSize.z;
    if (abs(collisionVec.x) > abs(collisionVec.z)) {
        collisionVec.z = 0.0f;
        collisionVec.Normalize();
        playerPos.x = wallPos.x + collisionVec.x * (wallCollisionSize.x / 2.0f + playerCollisionSize.x / 2.0f + 0.001f);
    } else {
        collisionVec.x = 0.0f;
        collisionVec.Normalize();
        playerPos.z = wallPos.z + collisionVec.z * (wallCollisionSize.z / 2.0f + playerCollisionSize.z / 2.0f + 0.001f);
    }

    playerTransform->SetPos(playerPos);
}

void EntityPlayerCollisionHandle(dynamic::DynamicObject* self, dynamic::DynamicObject* other) {
    int32_t selfEntityID = reinterpret_cast<int32_t>(self->GetUserData());
    int32_t otherEntityID = reinterpret_cast<int32_t>(other->GetUserData());

    Entity* selfEntity = EntityMgr::GetEntity(selfEntityID);
    Entity* otherEntity = EntityMgr::GetEntity(otherEntityID);
    if (!selfEntity || !otherEntity) {
        assert(false);
        return;
    }

    DataCom* data = reinterpret_cast<DataCom*>(otherEntity->GetComponent(entity::CT_DATA));
    if (data && !data->GetDataString("Tag").compare("Wall")) {
        EntityPlayerOnCollisionWithWall(selfEntity, otherEntity);
        return;
    }

    // TEST
    MessageBoxA(NULL, "GameOver", "Try Again", MB_OK);
    exit(0);
}

};  // namespace entity