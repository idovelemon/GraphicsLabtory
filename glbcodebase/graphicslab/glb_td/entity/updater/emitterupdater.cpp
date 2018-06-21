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
#include "../transformcom.h"
#include "updater.h"

#include "math/glbmatrix.h"

namespace entity {

void EntityNormalEmitterUpdater(Entity* entity) {
    TransformCom* transform = reinterpret_cast<TransformCom*>(entity->GetComponent(CT_TRANSFORM));
    if (transform == NULL) {
        assert(false);
        return;
    }

    float rotationSpeed = pyscript::PyScriptMgr::GetValueF("ENTITY_NORMAL_EMITTER_ROTATE_SPEED");

    glb::math::Vector rot = transform->GetRotate();
    rot = rot + glb::math::Vector(0.0f, rotationSpeed, 0.0f) * td::GameTimer::GetFrameSpeed();
    transform->SetRotate(rot);

    glb::math::Vector pos = transform->GetPos();
    glb::math::Matrix rotMat;
    rotMat.MakeRotateYMatrix(rotationSpeed * td::GameTimer::GetFrameSpeed());
    pos = rotMat * pos;
    transform->SetPos(pos);
}

};  // namespace entity