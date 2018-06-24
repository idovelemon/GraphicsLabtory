//-------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2018. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2018/06/21
// Brief: Core's updater method
//-------------------------------------------------------------
#include "updater.h"

#include <assert.h>

#include "glb.h"

#include "../entity.h"
#include "../entityfilter.h"
#include "../entitymgr.h"
#include "../../gametimer.h"
#include "../../pyscript/pyscriptmgr.h"
#include "../../pyscript/topython_function.h"
#include "../datacom.h"
#include "../transformcom.h"

namespace entity {

void EntityEnemy01Updater(Entity* entity) {
    TransformCom* transform = reinterpret_cast<TransformCom*>(entity->GetComponent(CT_TRANSFORM));
    if (transform == NULL) {
        assert(false);
        return;
    }

    // Find player's position
    glb::math::Vector playerPos(0.0f, 0.0f, 0.0f);
    std::vector<void*> args;
    char buffer[] = "Player";
    args.push_back(buffer);
    std::vector<Entity*> entities = EntityMgr::FindEntities(TagNameEntityFilter, args);
    if (!entities.empty() && entities[0]) {
        TransformCom* playerTransform = reinterpret_cast<TransformCom*>(entities[0]->GetComponent(CT_TRANSFORM));
        if (playerTransform != NULL) {
            playerPos = playerTransform->GetPosWorld();
        } else {
            assert(false);
        }
    } else {
        assert(false);
    }

    // Calculate enemy's transform
    float moveSpeed = pyscript::PyScriptMgr::GetValueF("ENTITY_ENEMY_01_MOVE_SPEED");
    glb::math::Vector enemyPos = transform->GetPosWorld();
    glb::math::Vector toPlayer = playerPos - enemyPos;
    toPlayer.Normalize();
    enemyPos = enemyPos + toPlayer * moveSpeed * td::GameTimer::GetFrameSpeed();
    transform->SetPos(enemyPos);

    glb::math::Vector rot = transform->GetRotateWorld();
    rot.y = (toPlayer.x > 0.0f ? -1.0f : 1.0f) * acos(glb::math::Dot(toPlayer, glb::math::Vector(0.0f, 0.0f, -1.0f))) * 180.0f / 3.14159;
    transform->SetRotate(rot);
}

void EntityEnemy01RotateUpdater(Entity* entity) {
    TransformCom* transform = reinterpret_cast<TransformCom*>(entity->GetComponent(CT_TRANSFORM));
    if (transform == NULL) {
        assert(false);
        return;
    }

    float rotateSpeed = pyscript::PyScriptMgr::GetValueF("ENTITY_ENEMY_01_ROTATE_SPEED");

    glb::math::Vector rot = transform->GetRotate();
    rot.x = rot.x - rotateSpeed * td::GameTimer::GetFrameSpeed();
    transform->SetRotate(rot);
}

};  // namespace entity