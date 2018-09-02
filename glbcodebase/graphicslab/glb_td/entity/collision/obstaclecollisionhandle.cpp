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

void EntityObstacleSpikeCollisionHandle(dynamic::DynamicObject* self, dynamic::DynamicObject* other) {
}

void EntityObstaclePushRockCollisionHandle(dynamic::DynamicObject* self, dynamic::DynamicObject* other) {
}

void EntityObstacleRotateRockCollisionHandle(dynamic::DynamicObject* self, dynamic::DynamicObject* other) {
}

};  // namespace entity