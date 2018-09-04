//-------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2018. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2018/06/21
// Brief: Core's updater method
//-------------------------------------------------------------
#include "../entity.h"

#include <assert.h>

#include "../../gametimer.h"
#include "../../log.h"
#include "../../pyscript/pyscriptmgr.h"
#include "../../pyscript/topython_function.h"
#include "../collisioncom.h"
#include "../datacom.h"
#include "../transformcom.h"
#include "../collision/collision.h"
#include "updater.h"

#include "glb.h"

namespace entity {

void EntityNormalEmitterUpdater(Entity* entity) {
    TransformCom* transform = reinterpret_cast<TransformCom*>(entity->GetComponent(CT_TRANSFORM));
    if (transform == NULL) {
        assert(false);
        return;
    }

    DataCom* data = reinterpret_cast<DataCom*>(entity->GetComponent(CT_DATA));
    if (data == NULL) {
        assert(false);
        return;
    }

    float shootDelta = pyscript::PyScriptMgr::GetValueF("NORMAL_EMITTER_SHOOT_DELTA");
    DataPack* shootAccelerateBulletPack = data->GetData("ShootAccelerateBulletDelta");
    DataPack* shootDeaccelerateBulletPack = data->GetData("ShootDeaccelerateBulletDelta");
    float curShootAccelerateBulletDelta = shootAccelerateBulletPack->GetFloat();
    float curShootDeaccelerateBulletDelta = shootDeaccelerateBulletPack->GetFloat();

    glb::math::Vector pos = transform->GetPosWorld();
    glb::math::Vector rot = transform->GetRotateWorld();

    bool canShootAccelerateBullet = false, canShootDeaccelerateBullet = false;

    if (glb::Input::IsMouseButtonPressed(glb::BM_LEFT)) {
        curShootAccelerateBulletDelta = curShootAccelerateBulletDelta + td::GameTimer::GetFrameSpeed();
    } else {
        if (curShootAccelerateBulletDelta > 0.0f) {
            canShootAccelerateBullet = true;
        }
    }

    if (glb::Input::IsMouseButtonPressed(glb::BM_RIGHT)) {
        curShootDeaccelerateBulletDelta = curShootDeaccelerateBulletDelta + td::GameTimer::GetFrameSpeed();
    } else {
        if (curShootDeaccelerateBulletDelta > 0.0f) {
            canShootDeaccelerateBullet = true;
        }
    }

    if (canShootAccelerateBullet) {
        // Create normal bullet
        float ratio = curShootAccelerateBulletDelta / shootDelta;
        ratio = max(0.0f, min(ratio, 1.0f));

        int bullet = EntityCreate();
        EntityAddTransformCom(bullet, pos.x, pos.y, pos.z, rot.x, rot.y, rot.z, 1.0f, 1.0f, 1.0f);
        EntityAddRenderCom(bullet, "res/model/emitter/normal_bullet.obj", pos.x, pos.y, pos.z, rot.x, rot.y, rot.z, 1.0f, 1.0f, 1.0f, true, 512);
        EntityAddScriptCom(bullet, "EntityNormalBulletUpdater");
        EntityAddDataCom(bullet);
        EntityAddFloatData(bullet, "Life", 3.0f * 60.0f);
        EntityAddFloatData(bullet, "Speed", pyscript::PyScriptMgr::GetValueF("ENTITY_NORMAL_BULLET_MOVE_SPEED"));
        EntityAddFloatData(bullet, "Damage", 1.0f);
        EntityAddStringData(bullet, "Tag", "PlayerAccelerateBullet");
        EntityAddFloatData(bullet, "ExtraSpeed", 1.0f + 4.0f * ratio);
        EntityAddFloatData(bullet, "ExtraSpeedTimer", 300.0f);
        EntityAddCollisionCom(bullet, pos.x, pos.y, pos.z, 0.8f, 0.8f, 0.8f);
        EntitySetCollisionHandle(bullet, "EntityNormalBulletCollisionHandle");
        EntitySetCollisionFilter(bullet, CollisionCom::PlayerBulletFilter, CollisionCom::EnemyFilter | CollisionCom::ObstacleFilter);

        curShootAccelerateBulletDelta = 0.0f;
    }

    if (canShootDeaccelerateBullet) {
        // Create normal bullet
        float ratio = curShootDeaccelerateBulletDelta / shootDelta;
        ratio = max(0.0f, min(ratio, 1.0f));

        int bullet = EntityCreate();
        EntityAddTransformCom(bullet, pos.x, pos.y, pos.z, rot.x, rot.y, rot.z, 1.0f, 1.0f, 1.0f);
        EntityAddRenderCom(bullet, "res/model/emitter/normal_bullet.obj", pos.x, pos.y, pos.z, rot.x, rot.y, rot.z, 1.0f, 1.0f, 1.0f, true, 512);
        EntityAddScriptCom(bullet, "EntityNormalBulletUpdater");
        EntityAddDataCom(bullet);
        EntityAddFloatData(bullet, "Life", 3.0f * 60.0f);
        EntityAddFloatData(bullet, "Speed", pyscript::PyScriptMgr::GetValueF("ENTITY_NORMAL_BULLET_MOVE_SPEED"));
        EntityAddFloatData(bullet, "Damage", 1.0f);
        EntityAddStringData(bullet, "Tag", "PlayerDeaccelerateBullet");
        EntityAddFloatData(bullet, "ExtraSpeed", 1.0f - 0.99f * ratio);
        EntityAddFloatData(bullet, "ExtraSpeedTimer", 300.0f);
        EntityAddCollisionCom(bullet, pos.x, pos.y, pos.z, 0.8f, 0.8f, 0.8f);
        EntitySetCollisionHandle(bullet, "EntityNormalBulletCollisionHandle");
        EntitySetCollisionFilter(bullet, CollisionCom::PlayerBulletFilter, CollisionCom::EnemyFilter | CollisionCom::ObstacleFilter);

        curShootDeaccelerateBulletDelta = 0.0f;
    }

    shootAccelerateBulletPack->SetFloat(curShootAccelerateBulletDelta);
    shootDeaccelerateBulletPack->SetFloat(curShootDeaccelerateBulletDelta);
}

void EntityNormalBulletUpdater(Entity* entity) {
    TransformCom* transform = reinterpret_cast<TransformCom*>(entity->GetComponent(CT_TRANSFORM));
    if (transform == NULL) {
        assert(false);
        return;
    }

    DataCom* data = reinterpret_cast<DataCom*>(entity->GetComponent(CT_DATA));
    if (data == NULL) {
        assert(false);
        return;
    }

    DataPack* pack = data->GetData("Life");
    DataPack* speedPack = data->GetData("Speed");
    float life = pack->GetFloat();
    life = life - td::GameTimer::GetFrameSpeed();
    pack->SetFloat(life);

    float moveSpeed = speedPack->GetFloat();
    glb::math::Vector pos = transform->GetPos();
    glb::math::Vector rot = transform->GetRotate();
    glb::math::Vector dir = glb::math::Vector(0.0f, 0.0f, 1.0f);
    dir = glb::math::Matrix::CreateRotateXYZMatrix(rot.x, rot.y, rot.z) * dir;
    pos = pos + dir * moveSpeed * td::GameTimer::GetFrameSpeed();
    transform->SetPos(pos);

    if (life <= 0.0f) {
        // Dead
        entity->SetDead(true);
    }
}

};  // namespace entity