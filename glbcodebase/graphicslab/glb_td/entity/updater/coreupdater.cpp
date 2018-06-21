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
#include "../datacom.h"
#include "../transformcom.h"
#include "updater.h"

#include "glb.h"

namespace entity {

void EntityCoreUpdater(Entity* entity) {
    TransformCom* transform = reinterpret_cast<TransformCom*>(entity->GetComponent(CT_TRANSFORM));
    if (transform == NULL) {
        assert(false);
        return;
    }

    float rotationSpeed = pyscript::PyScriptMgr::GetValueF("ENTITY_CORE_ROTATE_SPEED");

    glb::math::Vector rot = transform->GetRotate();
    rot = rot + glb::math::Vector(0.0f, rotationSpeed, 0.0f) * td::GameTimer::GetFrameSpeed();
    transform->SetRotate(rot);
}

void EntityCoreRotUpdater(Entity* entity) {
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

    float rotationAccel = pyscript::PyScriptMgr::GetValueF("ENTITY_CORE_ROT_ROTATE_ACCEL");
    float rotationDecel = pyscript::PyScriptMgr::GetValueF("ENTITY_CORE_ROT_ROTATE_DECEL");
    float minRotationSpeed = pyscript::PyScriptMgr::GetValueF("ENTITY_CORE_ROT_ROTATE_MIN_SPEED");
    float maxRotationSpeed = pyscript::PyScriptMgr::GetValueF("ENTITY_CORE_ROT_ROTATE_MAX_SPEED");

    DataPack* pack = data->GetData("CurRotateSpeed");
    float curRotateSpeed = 0.0f;
    if (glb::Input::IsMouseButtonPressed(glb::BM_LEFT)) {
        // Press left mouse button to accelerate rotation of core rot
        curRotateSpeed = pack->GetFloat() + rotationAccel * td::GameTimer::GetFrameSpeed();
    } else {
        // Slow down rotation speed of core rot when release left mouse button
        curRotateSpeed = pack->GetFloat() - rotationDecel * td::GameTimer::GetFrameSpeed();
    }
    curRotateSpeed = maxRotationSpeed < curRotateSpeed ? maxRotationSpeed : curRotateSpeed;
    curRotateSpeed = minRotationSpeed > curRotateSpeed ? minRotationSpeed : curRotateSpeed;
    pack->SetFloat(curRotateSpeed);

    glb::math::Vector rot = transform->GetRotate();
    rot = rot + glb::math::Vector(0.0f, curRotateSpeed, 0.0f) * td::GameTimer::GetFrameSpeed();
    transform->SetRotate(rot);
}

};  // namespace entity