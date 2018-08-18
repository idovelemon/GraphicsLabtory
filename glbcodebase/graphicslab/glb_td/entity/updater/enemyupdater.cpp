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

void EntityEnemy02Updater(Entity* entity) {
    TransformCom* transform = reinterpret_cast<TransformCom*>(entity->GetComponent(CT_TRANSFORM));
    if (transform == NULL) {
        assert(false);
        return;
    }

    DataCom* data = reinterpret_cast<DataCom*>(entity->GetComponent(CT_DATA));
    if (data == nullptr) {
        assert(false);
        return;
    }

    DataPack* statePack = data->GetData("State");
    DataPack* timerPack = data->GetData("StateTimer");
    DataPack* rushDirXPack = data->GetData("RushDirX");
    DataPack* rushDirYPack = data->GetData("RushDirY");
    DataPack* rushDirZPack = data->GetData("RushDirZ");
    if (statePack == nullptr || timerPack == nullptr
        || rushDirXPack == nullptr || rushDirYPack == nullptr || rushDirZPack == nullptr) {
        assert(false);
        return;
    }

    if (!statePack->GetString().compare("Idle")) {
        float timer = timerPack->GetFloat();
        timer = timer - td::GameTimer::GetFrameSpeed();

        if (timer <= 0.0f) {
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

            // Calculate rush direction
            glb::math::Vector dir = playerPos - transform->GetPosWorld();
            dir.Normalize();

            statePack->SetString("Rush");
            timerPack->SetFloat(pyscript::PyScriptMgr::GetValueF("ENTITY_ENEMY_02_RUSH_TIME"));
            rushDirXPack->SetFloat(dir.x);
            rushDirYPack->SetFloat(dir.y);
            rushDirZPack->SetFloat(dir.z);
        } else {
            timerPack->SetFloat(timer);
        }
    } else if (!statePack->GetString().compare("Rush")) {
        float timer = timerPack->GetFloat();
        timer = timer - td::GameTimer::GetFrameSpeed();

        if (timer <= 0.0f) {
            statePack->SetString("Idle");
            timerPack->SetFloat(pyscript::PyScriptMgr::GetValueF("ENTITY_ENEMY_02_IDLE_TIME"));
        } else {
            timerPack->SetFloat(timer);

            glb::math::Vector pos = transform->GetPos();
            glb::math::Vector dir = glb::math::Vector(rushDirXPack->GetFloat(), rushDirYPack->GetFloat(), rushDirZPack->GetFloat());
            pos = pos + dir * pyscript::PyScriptMgr::GetValueF("ENTITY_ENEMY_02_RUSH_SPEED") * td::GameTimer::GetFrameSpeed();
            transform->SetPos(pos);

            glb::math::Vector rot = transform->GetRotateWorld();
            rot.y = (dir.x > 0.0f ? -1.0f : 1.0f) * acos(glb::math::Dot(dir, glb::math::Vector(0.0f, 0.0f, -1.0f))) * 180.0f / 3.14159;
            transform->SetRotate(rot);
        }
    }
}

};  // namespace entity