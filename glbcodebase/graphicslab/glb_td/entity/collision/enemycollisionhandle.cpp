//-------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2018. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2018/08/13
// Brief: Bullet's collision handle
//-------------------------------------------------------------
#include "../entity.h"

#include <assert.h>

#include "../../gametimer.h"
#include "../../log.h"
#include "../../pyscript/pyscriptmgr.h"
#include "../../pyscript/topython_function.h"
#include "../datacom.h"
#include "../entitymgr.h"
#include "../transformcom.h"
#include "collision.h"

#include "glb.h"

namespace entity {

void EntityEnemyCollisionHandle(dynamic::DynamicObject* self, dynamic::DynamicObject* other) {
    int32_t selfEntityID = reinterpret_cast<int32_t>(self->GetUserData());
    int32_t otherEntityID = reinterpret_cast<int32_t>(other->GetUserData());

    Entity* selfEntity = entity::EntityMgr::GetEntity(selfEntityID);
    Entity* otherEntity = entity::EntityMgr::GetEntity(otherEntityID);

    if (!selfEntity || !otherEntity) return;

    entity::DataCom* selfData = reinterpret_cast<entity::DataCom*>(selfEntity->GetComponent(entity::CT_DATA));
    entity::DataCom* otherData = reinterpret_cast<entity::DataCom*>(otherEntity->GetComponent(entity::CT_DATA));

    if (!selfData || !otherData) return;

    entity::DataPack* selfHP = selfData->GetData("HP");
    entity::DataPack* otherDamage = otherData->GetData("Damage");

    if (!selfHP || !otherDamage) return;

    float hp = selfHP->GetFloat();
    hp = hp - otherDamage->GetFloat();
    selfHP->SetFloat(hp);

    if (hp < 0.0f) {
        selfEntity->SetDead(true);
        tdlog::Print("EntityEnemyCollisionHandle: SelfID:%d killed by OtherID:%d\n", selfEntityID, otherEntityID);
    }
}

};  // namespace entity