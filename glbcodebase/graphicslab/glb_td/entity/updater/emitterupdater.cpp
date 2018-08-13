//-------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2018. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2018/06/21
// Brief: Core's updater method
//-------------------------------------------------------------
#include "../entity.h"

#include <assert.h>

#include "../../gametimer.h"
#include "../../pyscript/pyscriptmgr.h"
#include "../../pyscript/topython_function.h"
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
    DataPack* pack = data->GetData("ShootDelta");
    float curDelta = pack->GetFloat();
    curDelta = curDelta + td::GameTimer::GetFrameSpeed();

    glb::math::Vector pos = transform->GetPosWorld();
    glb::math::Vector rot = transform->GetRotateWorld();

    if (glb::Input::IsMouseButtonPressed(glb::BM_LEFT) && curDelta > shootDelta) {
        // Reset shoot delta
        curDelta = 0.0f;

        // Create normal bullet
        int bullet = EntityCreate();
        EntityAddTransformCom(bullet, pos.x, pos.y, pos.z, rot.x, rot.y, rot.z, 1.0f, 1.0f, 1.0f);
        EntityAddRenderCom(bullet, "res/model/emitter/normal_bullet.obj", pos.x, pos.y, pos.z, rot.x, rot.y, rot.z, 1.0f, 1.0f, 1.0f, true, 512);
        EntityAddScriptCom(bullet, "EntityNormalBulletUpdater");
        EntityAddDataCom(bullet);
        EntityAddFloatData(bullet, "Life", 3.0f * 60.0f);
        EntityAddCollisionCom(bullet, pos.x, pos.y, pos.z, 1.0f, 1.0f, 1.0f);
        EntitySetCollisionHandle(bullet, "EntityNormalBulletCollisionHandle");
    }

    pack->SetFloat(curDelta);
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
    float life = pack->GetFloat();
    life = life - td::GameTimer::GetFrameSpeed();
    pack->SetFloat(life);

    float moveSpeed = pyscript::PyScriptMgr::GetValueF("ENTITY_NORMAL_BULLET_MOVE_SPEED");
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