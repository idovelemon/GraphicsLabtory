//-------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2018. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2018/08/30
// Brief: Obstacle's collision handle
//-------------------------------------------------------------
#include "../entity.h"

#include <assert.h>

#include "../../gametimer.h"
#include "../../log.h"
#include "../../pyscript/pyscriptmgr.h"
#include "../../pyscript/topython_function.h"
#include "../datacom.h"
#include "../entitymgr.h"
#include "../roletype.h"
#include "../transformcom.h"
#include "collision.h"

#include "glb.h"

namespace entity {

void EntityObstacleOnCollisionWithBullet(Entity* self, Entity* other) {
    DataCom* selfDataCom = reinterpret_cast<DataCom*>(self->GetComponent(entity::CT_DATA));
    DataCom* otherDataCom = reinterpret_cast<DataCom*>(other->GetComponent(entity::CT_DATA));

    if (selfDataCom->GetDataFloat("ExtraSpeedTimer") <= 0.0f) {
        selfDataCom->SetDataFloat("ExtraSpeed", otherDataCom->GetDataFloat("ExtraSpeed"));
        selfDataCom->SetDataFloat("ExtraSpeedTimer", otherDataCom->GetDataFloat("ExtraSpeedTimer"));
    }
}

void EntityObstacleSpikeCollisionHandle(dynamic::DynamicObject* self, dynamic::DynamicObject* other) {
    int32_t selfEntityID = reinterpret_cast<int32_t>(self->GetUserData());
    int32_t otherEntityID = reinterpret_cast<int32_t>(other->GetUserData());

    Entity* selfEntity = EntityMgr::GetEntity(selfEntityID);
    Entity* otherEntity = EntityMgr::GetEntity(otherEntityID);
    if (!selfEntity || !otherEntity) {
        assert(false);
        return;
    }

    DataCom* data = reinterpret_cast<DataCom*>(otherEntity->GetComponent(entity::CT_DATA));
    if (data && data->GetData("Tag")) {
        if ((!data->GetDataString("Tag").compare("PlayerAccelerateBullet")) || (!data->GetDataString("Tag").compare("PlayerDeaccelerateBullet"))) {
            EntityObstacleOnCollisionWithBullet(selfEntity, otherEntity);
            return;
        }
    }
}

void EntityObstaclePushRockCollisionHandle(dynamic::DynamicObject* self, dynamic::DynamicObject* other) {
    int32_t selfEntityID = reinterpret_cast<int32_t>(self->GetUserData());
    int32_t otherEntityID = reinterpret_cast<int32_t>(other->GetUserData());

    Entity* selfEntity = EntityMgr::GetEntity(selfEntityID);
    Entity* otherEntity = EntityMgr::GetEntity(otherEntityID);
    if (!selfEntity || !otherEntity) {
        assert(false);
        return;
    }

    DataCom* data = reinterpret_cast<DataCom*>(otherEntity->GetComponent(entity::CT_DATA));
    if (data && data->GetData("Tag")) {
        if ((!data->GetDataString("Tag").compare("PlayerAccelerateBullet")) || (!data->GetDataString("Tag").compare("PlayerDeaccelerateBullet"))) {
            EntityObstacleOnCollisionWithBullet(selfEntity, otherEntity);
            return;
        }
    }
}

void EntityObstacleRotateRockCollisionHandle(dynamic::DynamicObject* self, dynamic::DynamicObject* other) {
    int32_t selfEntityID = reinterpret_cast<int32_t>(self->GetUserData());
    int32_t otherEntityID = reinterpret_cast<int32_t>(other->GetUserData());

    Entity* selfEntity = EntityMgr::GetEntity(selfEntityID);
    Entity* otherEntity = EntityMgr::GetEntity(otherEntityID);
    if (!selfEntity || !otherEntity) {
        assert(false);
        return;
    }

    DataCom* data = reinterpret_cast<DataCom*>(otherEntity->GetComponent(entity::CT_DATA));
    if (data && data->GetData("Tag")) {
        if ((!data->GetDataString("Tag").compare("PlayerAccelerateBullet")) || (!data->GetDataString("Tag").compare("PlayerDeaccelerateBullet"))) {
            EntityObstacleOnCollisionWithBullet(selfEntity, otherEntity);
            return;
        }
    }
}

};  // namespace entity