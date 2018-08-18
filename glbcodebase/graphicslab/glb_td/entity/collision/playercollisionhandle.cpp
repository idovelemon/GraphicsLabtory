//-------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2018. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2018/08/18
// Brief: Player's collision handle
//-------------------------------------------------------------
#include "../entity.h"

#include <assert.h>

#include "../../gametimer.h"
#include "../../log.h"
#include "../../pyscript/pyscriptmgr.h"
#include "../../pyscript/topython_function.h"
#include "../datacom.h"
#include "../transformcom.h"
#include "collision.h"

#include "glb.h"

namespace entity {

void EntityPlayerCollisionHandle(dynamic::DynamicObject* self, dynamic::DynamicObject* other) {
    int32_t selfEntityID = reinterpret_cast<int32_t>(self->GetUserData());
    int32_t otherEntityID = reinterpret_cast<int32_t>(other->GetUserData());
}

};  // namespace entity